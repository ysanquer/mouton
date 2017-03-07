package fr.upem.mouton;

import org.omg.CORBA.Any;
import org.omg.CORBA.BooleanHolder;
import org.omg.CosEventComm.Disconnected;
import org.omg.CosEventComm.PullSupplierOperations;

public class NamedDrawingPullSupplier extends Thread implements PullSupplierOperations {

	@Override
	public void disconnect_pull_supplier() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Any pull() throws Disconnected {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Any try_pull(BooleanHolder arg0) throws Disconnected {
		// TODO Auto-generated method stub
		return null;
	}
}
