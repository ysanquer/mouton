package fr.upem.mouton;

import java.util.LinkedList;
import java.util.Queue;

import org.omg.CORBA.Any;
import org.omg.CosEventComm.Disconnected;
import org.omg.CosEventComm.PushConsumerOperations;

public class AnnotatedDrawingPushConsumer implements PushConsumerOperations {
	private final Queue<AnnotatedDrawing> receptionQueue = new LinkedList<>();
	
	public boolean hasReceived() {
		return !receptionQueue.isEmpty();
	}
	
	public AnnotatedDrawing nextReceived() {
		synchronized (receptionQueue) {
			return receptionQueue.remove();
		}
	}

	@Override
	public void disconnect_push_consumer() {
		System.out.println("ANNOTATED_DRAWING_PUSH_CONSUMER/disconnect");
	}

	@Override
	public void push(Any data) throws Disconnected {
		System.out.println("ANNOTATED_DRAWING_PUSH_CONSUMER/push begin");
		if (data.type().equivalent(AnnotatedDrawingHelper.type())) {
			System.out.println("ANNOTATED_DRAWING_PUSH_CONSUMER/push extract");
			AnnotatedDrawing extract = AnnotatedDrawingHelper.extract(data);
			synchronized (receptionQueue) {
				receptionQueue.add(extract);
			}
		}
		System.out.println("ANNOTATED_DRAWING_PUSH_CONSUMER/push end");
	}
}
