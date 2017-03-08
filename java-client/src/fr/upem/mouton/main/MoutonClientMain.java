package fr.upem.mouton.main;

import java.util.Properties;
import java.util.Scanner;

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

import fr.upem.mouton.AnnotatedDrawing;
import fr.upem.mouton.AnnotatedDrawingHelper;
import fr.upem.mouton.NamedDrawing;
import fr.upem.mouton.NamedDrawingHelper;
import fr.upem.mouton.PullSupplierImpl;
import fr.upem.mouton.PushConsumerImpl;

public class MoutonClientMain {
	public static void main(String[] args)
			throws InvalidName, NotFound, CannotProceed, org.omg.CosNaming.NamingContextPackage.InvalidName,
			AdapterInactive, ServantNotActive, WrongPolicy, AlreadyConnected, TypeError, InterruptedException {
		Properties properties = new Properties();
		properties.put("ORBInitRef.NameService", "corbaloc::localhost:2809/NameService");
		properties.put("org.omg.CORBA.ORBClass", "org.jacorb.orb.ORB");
		properties.put("org.omg.CORBA.ORBSingletonClass", "org.jacorb.orb.ORBSingleton");
		ORB orb = ORB.init(args, properties);
		NamingContextExt nc = NamingContextExtHelper.narrow(orb.resolve_initial_references("NameService"));

		EventChannel ecs = EventChannelHelper.narrow(nc.resolve(nc.to_name("EventChannel")));

		ConsumerAdmin ca = ecs.for_consumers();
		SupplierAdmin sa = ecs.for_suppliers();

		ProxyPushSupplier namedDrawingPPS = ca.obtain_push_supplier();
		ProxyPullConsumer namedDrawingPPC = sa.obtain_pull_consumer();

		ProxyPushSupplier annotatedDrawingPPS = ca.obtain_push_supplier();
		ProxyPullConsumer annotatedDrawingPPC = sa.obtain_pull_consumer();

		POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
		rootPOA.the_POAManager().activate();

		PushConsumerImpl<NamedDrawing> namedDrawingPushConsumer = new PushConsumerImpl<NamedDrawing>(
				NamedDrawingHelper::type, NamedDrawingHelper::extract);
		PushConsumerPOATie namedDrawingPCTie = new PushConsumerPOATie(namedDrawingPushConsumer);
		PullSupplierImpl<NamedDrawing> namedDrawingPullSupplier = new PullSupplierImpl<NamedDrawing>(orb::create_any,
				NamedDrawingHelper::insert);
		PullSupplierPOATie namedDrawingPSTie = new PullSupplierPOATie(namedDrawingPullSupplier);

		PushConsumerImpl<AnnotatedDrawing> annotatedDrawingPushConsumer = new PushConsumerImpl<AnnotatedDrawing>(
				AnnotatedDrawingHelper::type, AnnotatedDrawingHelper::extract);
		PushConsumerPOATie annotatedDrawingPCTie = new PushConsumerPOATie(annotatedDrawingPushConsumer);
		PullSupplierImpl<AnnotatedDrawing> annotatedDrawingPullSupplier = new PullSupplierImpl<AnnotatedDrawing>(
				orb::create_any, AnnotatedDrawingHelper::insert);
		PullSupplierPOATie annotatedDrawingPSTie = new PullSupplierPOATie(annotatedDrawingPullSupplier);

		PushConsumer namedDrawingPC = PushConsumerHelper.narrow(rootPOA.servant_to_reference(namedDrawingPCTie));
		PullSupplier namedDrawingPS = PullSupplierHelper.narrow(rootPOA.servant_to_reference(namedDrawingPSTie));

		PushConsumer annotatedDrawingPC = PushConsumerHelper
				.narrow(rootPOA.servant_to_reference(annotatedDrawingPCTie));
		PullSupplier annotatedDrawingPS = PullSupplierHelper
				.narrow(rootPOA.servant_to_reference(annotatedDrawingPSTie));

		namedDrawingPPS.connect_push_consumer(namedDrawingPC);
		namedDrawingPPC.connect_pull_supplier(namedDrawingPS);

		annotatedDrawingPPS.connect_push_consumer(annotatedDrawingPC);
		annotatedDrawingPPC.connect_pull_supplier(annotatedDrawingPS);
		System.out.println("servants registered");

		Thread orbRunThread = new Thread(orb::run);
		orbRunThread.start();

		try (Scanner sc = new Scanner(System.in)) {
			while (sc.hasNextLine()) {
				String next = sc.nextLine();
				switch (next) {
				case "cl":
					System.out.println("cl ndsend adsend ndrcv adrcv");
					break;
				case "ndsend":
					System.out.println("Author name: ");
					if (sc.hasNextLine()) {
						String name = sc.nextLine();
						System.out.println("Drawing expression: ");
						if (sc.hasNextLine()) {
							String drawing_expr = sc.nextLine();
							NamedDrawing nd = new NamedDrawing(drawing_expr, name);
							try {
								namedDrawingPullSupplier.send(nd);
							} catch (Exception e) {
								System.out.println("cannot send named drawing: " + e.getMessage());
							}
						}
					}
					break;
				case "adsend":
					System.out.println("Author name: ");
					if (sc.hasNextLine()) {
						String author_name = sc.nextLine();
						System.out.println("Drawing expression: ");
						if (sc.hasNextLine()) {
							String drawing_expr = sc.nextLine();
							System.out.println("Annotator name: ");
							if (sc.hasNextLine()) {
								String name = sc.nextLine();
								System.out.println("Annotations: ");
								if (sc.hasNextLine()) {
									String annotations = sc.nextLine();
									AnnotatedDrawing ad = new AnnotatedDrawing(drawing_expr, annotations, author_name,
											name);
									try {
										annotatedDrawingPullSupplier.send(ad);
									} catch (Exception e) {
										System.err.println("cannot send annotated drawing: " + e.getMessage());
									}
								}
							}
						}
					}
					break;
				case "ndrcv":
					try {
						NamedDrawing nd = namedDrawingPushConsumer.nextReceived();
						System.out.println("named drawing of " + nd.name + ":");
						System.out.println(nd.drawing_expr);
					} catch (Exception e) {
						System.err.println("cannot receive named drawing: " + e.getMessage());
					}
					break;
				case "adrcv":
					try {
						AnnotatedDrawing ad = annotatedDrawingPushConsumer.nextReceived();
						System.out.println("named drawing of " + ad.name + ":");
						System.out.println(ad.drawing_expr);
					} catch (Exception e) {
						System.err.println("cannot receive annotated drawing: " + e.getMessage());
					}
					break;
				}
			}
		}

		namedDrawingPPC.disconnect_pull_consumer();
		namedDrawingPPS.disconnect_push_supplier();
		annotatedDrawingPPC.disconnect_pull_consumer();
		annotatedDrawingPPS.disconnect_push_supplier();
		orb.shutdown(false);
		orb.destroy();
		orbRunThread.join();
		System.exit(0);
	}
};
