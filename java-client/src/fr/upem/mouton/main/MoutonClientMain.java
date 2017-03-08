package fr.upem.mouton.main;

import org.omg.CORBA.ORB;
import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CosEventChannelAdmin.AlreadyConnected;
import org.omg.CosEventChannelAdmin.ConsumerAdmin;
import org.omg.CosEventChannelAdmin.EventChannel;
import org.omg.CosEventChannelAdmin.EventChannelHelper;
import org.omg.CosEventChannelAdmin.ProxyPullConsumer;
import org.omg.CosEventChannelAdmin.ProxyPushSupplier;
import org.omg.CosEventChannelAdmin.SupplierAdmin;
import org.omg.CosEventChannelAdmin.TypeError;
import org.omg.CosEventComm.PullSupplier;
import org.omg.CosEventComm.PullSupplierHelper;
import org.omg.CosEventComm.PullSupplierPOATie;
import org.omg.CosEventComm.PushConsumer;
import org.omg.CosEventComm.PushConsumerHelper;
import org.omg.CosEventComm.PushConsumerPOATie;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.CosNaming.NamingContextPackage.CannotProceed;
import org.omg.CosNaming.NamingContextPackage.NotFound;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;
import org.omg.PortableServer.POAPackage.ServantNotActive;
import org.omg.PortableServer.POAPackage.WrongPolicy;

import fr.upem.mouton.AnnotatedDrawingPushConsumer;
import fr.upem.mouton.NamedDrawingPullSupplier;

public class MoutonClientMain {
	public static void main(String[] args)
			throws InvalidName, NotFound, CannotProceed, org.omg.CosNaming.NamingContextPackage.InvalidName,
			AdapterInactive, ServantNotActive, WrongPolicy, AlreadyConnected, TypeError {
		ORB orb = ORB.init(args, null);
		NamingContextExt nc = NamingContextExtHelper.narrow(orb.resolve_initial_references("NameService"));

		EventChannel ecs = EventChannelHelper.narrow(nc.resolve(nc.to_name("EventChannel")));

		ConsumerAdmin ca = ecs.for_consumers();
		SupplierAdmin sa = ecs.for_suppliers();

		ProxyPushSupplier pps = ca.obtain_push_supplier();
		ProxyPullConsumer ppc = sa.obtain_pull_consumer();

		POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
		rootPOA.the_POAManager().activate();

		PushConsumerPOATie pcTie = new PushConsumerPOATie(new AnnotatedDrawingPushConsumer());
		PullSupplierPOATie psTie = new PullSupplierPOATie(new NamedDrawingPullSupplier(orb::create_any));

		PushConsumer pc = PushConsumerHelper.narrow(rootPOA.servant_to_reference(pcTie));
		PullSupplier ps = PullSupplierHelper.narrow(rootPOA.servant_to_reference(psTie));
		pps.connect_push_consumer(pc);
		ppc.connect_pull_supplier(ps);
		System.out.println("servants registered");
		orb.run();
	}
};
