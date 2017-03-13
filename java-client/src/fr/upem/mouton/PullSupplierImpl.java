package fr.upem.mouton;

import java.util.Objects;
import java.util.function.BiConsumer;
import java.util.function.Supplier;

import org.omg.CORBA.Any;
import org.omg.CORBA.BooleanHolder;
import org.omg.CosEventComm.Disconnected;
import org.omg.CosEventComm.PullSupplierOperations;

/**
 * @brief Implementation of {@link PullSupplierOperations}.
 * 
 *        This implementation yields instances of T serialized into {@link Any}.
 * 
 * @authors Yann Sanquer
 *
 * @param <T>
 *            type of sent data
 */
public class PullSupplierImpl<T> implements PullSupplierOperations {
	private final int sleepTimeMillis;
	private final Supplier<Any> createAny;
	private final BiConsumer<Any, T> anyInsert;
	private Any toSend = null;
	private boolean sending = false;
	private final Object sync = new Object();

	/**
	 * @brief Is this instance sending something?
	 * 
	 *        A {@link PullSupplierImpl} is sending something if the
	 *        {@link PullSupplierImpl#send(T)} method was called on it,
	 *        completed, but no call to {@link PullSupplierImpl#pull()} has
	 *        happened yet.
	 * 
	 * @return whether this instance is sending something
	 */
	public boolean isSending() {
		return sending;
	}

	/**
	 * @brief Offer an object for pulling.
	 * 
	 *        After a completed call to this method, this instance is in the
	 *        sending state.
	 * 
	 * @param obj
	 *            the object to be pulled
	 * 
	 * @see PullSupplierImpl#isSending()
	 */
	public void send(T obj) {
		synchronized (sync) {
			if (sending) {
				throw new IllegalStateException("already sending");
			}
			this.toSend = this.createAny.get();
			this.anyInsert.accept(this.toSend, obj);
			this.sending = true;
		}
	}

	/**
	 * @brief Create a new {@link PullSupplierImpl} with given {@link Any}
	 *        supplier, and T to {@link Any} inserter.
	 * 
	 *        The sleep time defaults to one second.
	 * 
	 * @param createAny
	 *            {@link Any} object generator
	 * @param anyInsert
	 *            the function that inserts objects of type T into an
	 *            {@link Any}
	 */
	public PullSupplierImpl(Supplier<Any> createAny, BiConsumer<Any, T> anyInsert) {
		this(createAny, anyInsert, 1000);
	}

	/**
	 * @brief Create a new {@link PullSupplierImpl} with given {@link Any}
	 *        supplier, and T to {@link Any} inserter; The
	 *        {@link PullSupplierImpl#pull()} method will wait for the given
	 *        time in milliseconds before trying to pull again.
	 * 
	 * @param createAny
	 *            {@link Any} object generator
	 * @param anyInsert
	 *            the function that inserts objects of type T into an
	 *            {@link Any}
	 * @param sleepTimeMillis
	 *            the time, in milliseconds, that
	 *            {@link PullSupplierImpl#pull()} should wait before trying to
	 *            call {@link PullSupplierImpl#try_pull(BooleanHolder)} again
	 *            after it yielded nothing
	 */
	public PullSupplierImpl(Supplier<Any> createAny, BiConsumer<Any, T> anyInsert, int sleepTimeMillis) {
		this.createAny = Objects.requireNonNull(createAny);
		this.anyInsert = Objects.requireNonNull(anyInsert);
		this.sleepTimeMillis = sleepTimeMillis;
	}

	@Override
	public void disconnect_pull_supplier() {
	}

	@Override
	public Any pull() throws Disconnected {
		BooleanHolder has_event = new BooleanHolder(false);
		Any ret = this.try_pull(has_event);
		for (;;) {
			if (has_event.value) {
				// any not empty
				break;
			}

			// otherwise wait before going again
			ret = null;
			try {
				Thread.sleep(sleepTimeMillis);
			} catch (InterruptedException e) {
				// restore interrupted status then break with empty any
				Thread.currentThread().interrupt();
				break;
			}
			ret = this.try_pull(has_event);
		}
		return ret;
	}

	@Override
	public Any try_pull(BooleanHolder has_event) throws Disconnected {
		has_event.value = false;
		synchronized (sync) {
			Any ret = this.createAny.get();
			if (sending) {
				has_event.value = true;
				ret = toSend;
				sending = false;
				toSend = null;
			}
			// else ret is empty by definition
			return ret;
		}
	}
}
