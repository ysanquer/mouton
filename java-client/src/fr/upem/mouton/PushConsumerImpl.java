package fr.upem.mouton;

import java.util.LinkedList;
import java.util.Objects;
import java.util.Queue;
import java.util.function.Function;
import java.util.function.Supplier;

import org.omg.CORBA.Any;
import org.omg.CORBA.TypeCode;
import org.omg.CosEventComm.Disconnected;
import org.omg.CosEventComm.PushConsumerOperations;

/**
 * @brief Implementation of {@link PushConsumerOperations}.
 * 
 *        This implementation receives instances of type T serialized into
 *        {@link Any}.
 * 
 * @authors Yann Sanquer
 *
 * @param <T>
 *            type of received data
 */
public class PushConsumerImpl<T> implements PushConsumerOperations {
	private final Queue<T> receptionQueue = new LinkedList<>();
	private final Supplier<TypeCode> typeCodeSupplier;
	private final Function<Any, T> anyExtract;

	/**
	 * @brief Create a new {@link PushConsumerImpl} with a given
	 *        {@link TypeCode} supplier for the type T, and {@link Any} to T
	 *        extraction function.
	 * 
	 * @param typeCodeSupplier
	 *            {@link TypeCode} supplier function for type T
	 * @param anyExtract
	 *            {@link Any} to T extraction function
	 */
	public PushConsumerImpl(Supplier<TypeCode> typeCodeSupplier, Function<Any, T> anyExtract) {
		this.typeCodeSupplier = Objects.requireNonNull(typeCodeSupplier);
		this.anyExtract = Objects.requireNonNull(anyExtract);
	}

	/**
	 * @brief Has this consumer received any push?
	 * 
	 * @return whether this consumer has received data
	 */
	public boolean hasReceived() {
		return !receptionQueue.isEmpty();
	}

	/**
	 * @brief Obtain next received T object.
	 * 
	 * @return the next T object
	 */
	public T nextReceived() {
		synchronized (receptionQueue) {
			return receptionQueue.remove();
		}
	}

	@Override
	public void disconnect_push_consumer() {
	}

	@Override
	public void push(Any data) throws Disconnected {
		if (data.type().equivalent(typeCodeSupplier.get())) {
			T extract = anyExtract.apply(data);
			synchronized (receptionQueue) {
				receptionQueue.add(extract);
			}
		}
	}
}
