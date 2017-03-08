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

public class PushConsumerImpl<T> implements PushConsumerOperations {
	private final Queue<T> receptionQueue = new LinkedList<>();
	private final Supplier<TypeCode> typeCodeSupplier;
	private final Function<Any, T> anyExtract;
	
	public PushConsumerImpl(Supplier<TypeCode> typeCodeSupplier, Function<Any, T> anyExtract) {
		this.typeCodeSupplier = Objects.requireNonNull(typeCodeSupplier);
		this.anyExtract = Objects.requireNonNull(anyExtract);
	}
	
	public boolean hasReceived() {
		return !receptionQueue.isEmpty();
	}
	
	public T nextReceived() {
		synchronized (receptionQueue) {
			return receptionQueue.remove();
		}
	}

	@Override
	public void disconnect_push_consumer() {
		System.err.println("wat");
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
