package fr.upem.mouton;

import java.util.Objects;
import java.util.function.Supplier;

import org.omg.CORBA.Any;
import org.omg.CORBA.BooleanHolder;
import org.omg.CORBA.ORB;
import org.omg.CosEventComm.Disconnected;
import org.omg.CosEventComm.PullSupplierOperations;

public class NamedDrawingPullSupplier implements PullSupplierOperations {
	private final Supplier<Any> createAny;
	private NamedDrawing toSend = null;
	private boolean sending = false;
	private final Object sync = new Object();

	public boolean isSending() {
		return sending;
	}

	public void sendNamedDrawing(NamedDrawing nd) {
		synchronized (sync) {
			if (isSending()) {
				throw new IllegalStateException("already sending");
			}
			this.toSend = Objects.requireNonNull(nd);
			this.sending = true;
		}
	}

	public NamedDrawingPullSupplier(Supplier<Any> createAny) {
		this.createAny = Objects.requireNonNull(createAny);
	}

	@Override
	public void disconnect_pull_supplier() {
		System.out.println("NAMED_DRAWING_PULL_SUPPLIER/disconnect");
	}

	@Override
	public Any pull() throws Disconnected {
		System.out.println("NAMED_DRAWING_PULL_SUPPLIER/pull begin");
		BooleanHolder has_event = new BooleanHolder(false);
		Any ret;
		do {
			ret = this.try_pull(has_event);
		} while (!has_event.value);
		System.out.println("NAMED_DRAWING_PULL_SUPPLIER/pull end");
		return ret;
	}

	@Override
	public Any try_pull(BooleanHolder has_event) throws Disconnected {
		System.out.println("NAMED_DRAWING_PULL_SUPPLIER/try_pull begin");
		Any ret = this.createAny.get();
		has_event.value = false;
		synchronized (sync) {
			if (sending) {
				NamedDrawingHelper.insert(ret, toSend);
				has_event.value = true;
				sending = false;
				toSend = null;
			}
		}
		System.out.println("NAMED_DRAWING_PULL_SUPPLIER/try_pull end");
		return ret;
	}
}
