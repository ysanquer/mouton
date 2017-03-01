#include <iostream>
#include <limits>
#include <unistd.h>

#include "ProcShell.h"
#include "ShapeELParser.h"
#include "Shape.h"
#include "Vector.h"
#include "LogRenderEngine.h"

#include "PullSupplierImpl.h"
#include "PushConsumerImpl.h"

#include <CosEventComm.hh>
#include <CosEventChannelAdmin.hh>
#include <omniEvents/EventChannel.h>
#include "naming.h"

#include "named-drawing.hh"
#include "annotated-drawing.hh"

#include "main_tools.h"
#include "global_verbosity.h"

#include "functions.h"

#include <signal.h>

using namespace std;

void* run_orb(void *);
void render_on_engine(unique_ptr<Shape> const &, LogRenderEngine &);
void show_shape(unique_ptr<Shape> const &);

int main(int argc, char *argv[])
{
    // Named Drawing Pull Supplier
    shared_ptr<PullSupplierImpl> ndps;
    // Annotated Drawing Push Consumer
    shared_ptr<PushConsumerImpl> adpc;

    char const *channelName = "EventChannel";
    string name = "";
    unsigned long pull_retry_delay(1);

    unsigned int verbose_level = 0;

    int c;
    while ((c = getopt(argc,argv,"vc:h")) != EOF)
    {
        switch (c)
        {
            case 'v': verbose_level++;
                      break;
            case 'c': channelName = optarg;
                      break;

            case 'h': usage();
                      exit(0);
            default : usage();
                      exit(-1);
        }
    }
    if(optind >= argc)
    {
        usage();
        exit(-1);
    }

    global::verbose_level1 = verbose_level >= 1;
    global::verbose_level2 = verbose_level >= 2;
    global::verbose_level3 = verbose_level >= 3;

    if(global::verbose_level1)
        clog << "verbose level: " << verbose_level << endl;

    name = argv[optind];

    ndps = make_shared<PullSupplierImpl>();
    adpc = make_shared<PushConsumerImpl>();

    bool program_term = false;
    bool refresh_pull = false;

    unique_ptr<Shape> to_send;
    auto ndps_try_pull = [&name, &to_send, &refresh_pull](CORBA::Boolean &has_event)
    {
        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_SUPPLIER/TRY_PULL begin" << endl;
        CORBA::Any *ret(new CORBA::Any);
        if(to_send && refresh_pull)
        {
            if(global::verbose_level3)
                clog << "NAMED_DRAWING_PULL_SUPPLIER/TRY_PULL to_send set" << endl;

            stringstream ss;
            ss << *to_send;
            string to_send_flat = ss.str();

            if(global::verbose_level3)
                clog << "NAMED_DRAWING_PULL_SUPPLIER/TRY_PULL to_send_flat=" << to_send_flat << endl;

            // compose NamedDrawing

            fr::upem::mouton::NamedDrawing nd;
            nd.drawing_expr = to_send_flat.c_str();
            nd.name = name.c_str();

            if(global::verbose_level3)
                clog << "NAMED_DRAWING_PULL_SUPPLIER/TRY_PULL name=" << name << endl;

            // insert into any

            *ret <<= nd;
            has_event = 1;

            if(global::verbose_level3)
                clog << "NAMED_DRAWING_PULL_SUPPLIER/TRY_PULL has_event=1" << endl;

            refresh_pull = false;
        }
        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_SUPPLIER/TRY_PULL end" << endl;
        return ret;
    };

    ndps->set_pull([ndps_try_pull, &program_term, &pull_retry_delay]()
    {
        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_SUPPLIER/PULL begin" << endl;
        CORBA::Any *ret(nullptr);
        CORBA::Boolean has_event = 0;

        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_SUPPLIER/PULL trying to pull" << endl;
        ret = ndps_try_pull(has_event);

        while(!has_event && !program_term)
        {
            if(global::verbose_level3)
                clog << "NAMED_DRAWING_PULL_SUPPLIER/PULL attempt failed, retry later" << endl;

            // sleep
            omni_thread::sleep(pull_retry_delay);

            if(global::verbose_level3)
                clog << "NAMED_DRAWING_PULL_SUPPLIER/PULL retrying to pull" << endl;
            // and retry
            ret = ndps_try_pull(has_event);
        }

        // any may be empty if program termination was requested

        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_SUPPLIER/PULL end" << endl;
        return ret;
    });
    ndps->set_try_pull(ndps_try_pull);
    ndps->set_disconnect_pull_supplier([]()
    {
        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_SUPPLIER/DISCONNECT begin" << endl;
        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_SUPPLIER/DISCONNECT end" << endl;
    });

    if(global::verbose_level2)
        clog << "MAIN configured NAMED_DRAWING_PULL_SUPPLIER" << endl;

    list< tuple< string, unique_ptr<Shape>, string > > reception_queue;
    map< string, pair< unique_ptr<Shape>, string> > received_drawings_by_name;
    ShapeELParser     selp_recv;

    adpc->set_push([&name, &selp_recv, &reception_queue](CORBA::Any const &any)
    {
        if(global::verbose_level3)
            clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/PUSH begin" << endl;
        fr::upem::mouton::AnnotatedDrawing *ad;
        if(any >>= ad)
        {
            if(global::verbose_level3)
                clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/PUSH received Annotated Drawing" << endl;
            string received_name = (char*)ad->name;
            string received_author_name = (char*)ad->author_name;
            string received_drawing = (char*)ad->drawing_expr;
            string received_annotations = (char*)ad->annotations;

            if(name == received_author_name)
            {
                if(global::verbose_level3)
                    clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/PUSH received Annotated Drawing is mine" << endl;
                istringstream iss(received_drawing);
                iss >> selp_recv;
                unique_ptr<Shape> parsed_shape = selp_recv.parsed_shape();
                if(parsed_shape)
                {
                    if(global::verbose_level3)
                        clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/PUSH done parsing drawing" << endl;
                    reception_queue.push_front(make_tuple(received_name, move(parsed_shape), received_annotations));
                    //received_drawings_by_name[received_name] = make_pair(move(parsed_shape), received_annotations);
                    if(global::verbose_level3)
                        clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/PUSH registered drawing and annotations from " << received_name << endl;
                    cout << "Received annotations from " << received_name << endl;
                }
            }
        }
        if(global::verbose_level3)
            clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/PUSH end" << endl;
    });
    adpc->set_disconnect_push_consumer([]()
    {
        if(global::verbose_level3)
            clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/DISCONNECT begin" << endl;
        if(global::verbose_level3)
            clog << "ANNOTATED_DRAWING_PUSH_CONSUMER/DISCONNECT end" << endl;
    });

    if(global::verbose_level2)
        clog << "MAIN configured ANNOTATED_DRAWING_PUSH_CONSUMER" << endl;

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if(global::verbose_level2)
        clog << "MAIN initialized ORB" << endl;

    CosEventChannelAdmin::EventChannel_var channel;

    const char* action=""; // Use this variable to help report errors.
    try
    {
        CORBA::Object_var obj;

        OmniEvents::Orb::inst()._orb = orb;

        action = "resolve initial references";
        if(global::verbose_level2)
            clog << "MAIN " << action << endl;

        OmniEvents::Orb::inst().resolveInitialReferences();

        action="activate the RootPOA's POAManager";
        if(global::verbose_level2)
            clog << "MAIN " << action << endl;

        PortableServer::POAManager_var pman =OmniEvents::Orb::inst()._RootPOA->the_POAManager();
        pman->activate();

        //
        // Obtain object reference to EventChannel
        // (from command-line argument or from the Naming Service).
        if(optind+1<argc)
        {
            action="convert URI from command line into object reference";
            if(global::verbose_level2)
                clog << "MAIN " << action << endl;

            obj=orb->string_to_object(argv[optind+1]);
        }
        else
        {
            CosNaming::NamingContext_var rootContext=
                OmniEvents::Orb::inst()._NameService;

            action="find EventChannel in NameService";
            if(global::verbose_level2)
                clog << "MAIN " << action << endl;

            obj=rootContext->resolve(str2name(channelName));
        }

        action="narrow object reference to event channel";
        if(global::verbose_level2)
            clog << "MAIN " << action << endl;

        channel=CosEventChannelAdmin::EventChannel::_narrow(obj);
        if(CORBA::is_nil(channel))
        {
            cerr << "Failed to narrow Event Channel reference." << endl;
            exit(1);
        }

    }
    catch(CORBA::ORB::InvalidName& ex)
    { // resolve_initial_references
        cerr<<"Failed to "<<action<<". ORB::InvalidName"<<endl;
        exit(1);
    }
    catch(CosNaming::NamingContext::InvalidName& ex)
    { // resolve
        cerr<<"Failed to "<<action<<". NamingContext::InvalidName"<<endl;
        exit(1);
    }
    catch(CosNaming::NamingContext::NotFound& ex)
    { // resolve
        cerr<<"Failed to "<<action<<". NamingContext::NotFound"<<endl;
        exit(1);
    }
    catch(CosNaming::NamingContext::CannotProceed& ex)
    { // resolve
        cerr<<"Failed to "<<action<<". NamingContext::CannotProceed"<<endl;
        exit(1);
    }
    catch(CORBA::TRANSIENT& ex)
    { // _narrow()
        cerr<<"Failed to "<<action<<". TRANSIENT"<<endl;
        exit(1);
    }
    catch(CORBA::OBJECT_NOT_EXIST& ex)
    { // _narrow()
        cerr<<"Failed to "<<action<<". OBJECT_NOT_EXIST"<<endl;
        exit(1);
    }
    catch(CORBA::SystemException& ex)
    {
        cerr<<"Failed to "<<action<<".";
        cerr<<" "<<ex._name();
        if(ex.NP_minorString())
            cerr<<" ("<<ex.NP_minorString()<<")";
        cerr<<endl;
        exit(1);
    }
    catch(CORBA::Exception& ex)
    {
        cerr<<"Failed to "<<action<<"."
            " "<<ex._name()
            <<endl;
        exit(1);
    }

    //
    // Get Supplier Admin interface - retrying on Comms Failure.

    CosEventChannelAdmin::SupplierAdmin_var supplier_admin =
        obtain< CosEventChannelAdmin::SupplierAdmin_var >(
            [&channel](){ return channel->for_suppliers(); },
            []()
            {
                cerr << "Event Channel returned nil Supplier Admin!" << endl;
            },
            []()
            {
                cerr << "Caught COMM_FAILURE exception "
                    << "obtaining Supplier Admin! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN obtained Supplier Admin from Event Channel" << endl;

    CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        obtain<CosEventChannelAdmin::ConsumerAdmin_var>(
            [&channel](){ return channel->for_consumers(); },
            []()
            {
                cerr << "Event Channel returned nil Consumer Admin!" << endl;
            }, []()
            {
                cerr << "Caught COMM_FAILURE exception "
                    << "obtaining Consumer Admin! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN obtained Consumer Admin from Event Channel" << endl;

    //
    // Get proxy consumer - retrying on Comms Failure.
    CosEventChannelAdmin::ProxyPullConsumer_var proxy_consumer
        = obtain<CosEventChannelAdmin::ProxyPullConsumer_var>(
            [&supplier_admin](){ return supplier_admin->obtain_pull_consumer(); },
            []()
            {
                cerr << "Supplier Admin returned nil proxy_consumer!" << endl;
            }, []()
            {
                cerr << "Caught COMM_FAILURE exception "
                    << "obtaining Proxy Pull Consumer! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN obtained Proxy Pull Consumer from Supplier Admin" << endl;

    //
    // Get proxy supplier - retrying on Comms Failure.
    CosEventChannelAdmin::ProxyPushSupplier_var proxy_supplier
        = obtain<CosEventChannelAdmin::ProxyPushSupplier_var>(
            [&consumer_admin](){ return consumer_admin->obtain_push_supplier(); },
            []()
            {
                cerr << "Consumer Admin returned nil proxy_supplier!" << endl;
            }, []()
            {
                cerr << "Caught COMM_FAILURE Exception "
                    << "obtaining Push Supplier! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN obtained Proxy Push Supplier from Consumer Admin" << endl;

    // Connect Pull Supplier - retrying on Comms Failure.

    connect([&ndps, &proxy_consumer]()
    {
        CosEventComm::PullSupplier_var supplierRef =ndps->_this();
        proxy_consumer->connect_pull_supplier(supplierRef.in());
    }, []()
    {
        cerr<<"Caught BAD_PARAM Exception connecting Pull Supplier!"<<endl;
    },[]()
    {
        cerr << "Pull Supplier already connected!" << endl;
    },[]()
    {
        cerr << "Caught COMM_FAILURE exception "
            << "connecting Pull Supplier! Retrying..."
            << endl;
    });

    if(global::verbose_level1)
        clog << "MAIN connected Pull Supplier to Proxy Pull Consumer" << endl;

    // Connect Push Consumer - retrying on Comms Failure.
    connect([&proxy_supplier, &adpc](){ proxy_supplier->connect_push_consumer(adpc->_this()); },
            []()
            {
                cerr << "Caught BAD_PARAM Exception connecting Push Consumer!" << endl;
            },
            []()
            {
                cerr << "Proxy Push Supplier already connected!" << endl;
            },
            []()
            {
                cerr << "Caught COMM_FAILURE exception "
                    << "connecting Push Consumer! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN connected Push Consumer to Proxy Push Supplier" << endl;

    omni_thread *th = omni_thread::create(run_orb);

    if(global::verbose_level1)
        clog << "MAIN started ORB run thread" << endl;

    ProcShell         shell;
    ShapeELParser     selp;
    unique_ptr<Shape> shape_ptr;
    LogRenderEngine   engine;
    string            my_name;

    shell
        .when("cl",    [](){ cout << "cl xp rcv hello curshow readshape curdisplay publish pubcl pubshow pubdisplay rcvlist rcvdisplay" << endl; }, "show command list")
        .when("hello", [](){ cout << "Hello, world" << endl; }, "say hello")
        .when("pubcl", [&to_send](){ to_send.reset(); }, "clear any published shape")
        .when("curdisplay", [&shape_ptr, &engine](){ render_on_engine(shape_ptr, engine); }, "display current shape")
        .when("pubdisplay",  [&to_send, &engine](){ render_on_engine(to_send, engine); }, "display published shape")
        .when("curshow", [&shape_ptr]() { show_shape(shape_ptr); }, "show current shape")
        .when("pubshow", [&to_send]() { show_shape(to_send); }, "show published shape")
        .when("rcvdisplay",
            [&received_drawings_by_name, &engine]()
            {
                string rcv_name;
                cout << "Whose?" << endl << "mouton|rcvdisplay> ";
                if(getline(cin, rcv_name))
                {
                    str_trim_ws(rcv_name);
                    auto it = received_drawings_by_name.find(rcv_name);
                    if(it == received_drawings_by_name.end())
                        cout << "no one named " << rcv_name << " has sent you anything" << endl;
                    else
                    {
                        unique_ptr<Shape> &ptr = it->second.first;
                        stringstream ss;
                        ss << *ptr;
                        cout << "Shape Expression: " << ss.str() << endl;
                        cout << "Shape Render: " << endl;
                        ptr->render(engine);
                        cout << engine.result() << endl;
                        engine.clear();
                        cout << "Annotations: " << endl;
                        cout << it->second.second << endl;
                    }
                }
            }, "display annotated drawing")
        .when("rcvlist",
            [&received_drawings_by_name]()
            {
                auto it = received_drawings_by_name.begin();
                auto end = received_drawings_by_name.end();
                if(it != end)
                {
                    cout << it->first;
                    it++;
                }
                while(it != end)
                {
                    cout << ", " << it->first;
                    it++;
                }
                cout << endl;
            }, "show who has annotated your drawing")
        .when("rcv",
            [&reception_queue, &engine, &received_drawings_by_name]()
            {
                if(reception_queue.empty())
                {
                    cout << "no pending annotations" << endl;
                }
                else
                {
                    tuple< string, unique_ptr<Shape>, string > &entry = reception_queue.back();
                    
                    string received_name = get<0>(entry);
                    unique_ptr<Shape> drawing = move(get<1>(entry));
                    string annotations = get<2>(entry);

                    reception_queue.pop_back();

                    cout << "From " << received_name << ":" << endl;
                    stringstream ss;
                    ss << *drawing;
                    cout << "Shape Expression: " << ss.str() << endl;
                    cout << "Shape Render: " << endl;
                    drawing->render(engine);
                    cout << engine.result() << endl;
                    engine.clear();
                    cout << "Annotations: " << endl;
                    cout << annotations << endl;

                    received_drawings_by_name[received_name] = make_pair(move(drawing), annotations);
                    cout << "registered (view it again via rcvdisplay)" << endl;
                }
            },"receive annotations from the reception queue")
        .when("xp",
            [&shell]()
            {
                string com;
                cout << "mouton|xp> ";
                if(getline(cin, com))
                {
                    str_trim_ws(com);
                    cout << com << ": " << shell.explain(com) << endl;
                }
            },"explain command")
        .when("readshape",
            [&selp, &shape_ptr]()
            {
                cout << "Input expression for shape" << endl;
                cout << "mouton|readshape> ";
                try
                {
                    cin >> selp;
                    unique_ptr<Shape> parsed_shape;
                    parsed_shape = selp.parsed_shape();
                    if(parsed_shape)
                    {
                        shape_ptr = move(parsed_shape);
                        cout << "successfully parsed shape" << endl;
                    }
                    else
                    {
                        if(cin.eof())
                            cout << "cancelled" << endl;
                        else if(cin.fail())
                            cout << "failed to parse shape" << endl;

                        if(!cin.bad())
                        {    
                            cin.clear();
                            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }
                }
                catch(ShapeException const &ex)
                {
                    cerr << "failed to parse shape: " << ex.what() << endl;
                }
            }, "read new shape")
        .when("publish",
            [&shape_ptr, &to_send, &refresh_pull]()
            {
                if(!shape_ptr)
                    cout << "no shape to publish" << endl;
                else
                {
                    to_send = shape_ptr->clone();
                    refresh_pull = true;
                }
            }, "publish current shape");

    if(global::verbose_level1)
        clog << "MAIN prepared shell" << endl;

    cout << "Mouton Client Shell -- type 'cl' for list of commands, 'xp' to explain a command" << endl;
    while(true)
    {
        do
        {
            cout << "mouton> ";
        } while(cin >> shell);
        if(cin.fail())
        {
            if(!cin.bad() && !cin.eof())
            {
                cerr << "invalid input" << endl;
                cin.clear();
                //cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if(global::verbose_level1)
                clog << "MAIN shell exited" << endl;
            break;
        }
    }
    cout << "quit" << endl;

    program_term = true;
    OmniEvents::Orb::inst().shutdown(0);

    th->join(nullptr);

    if(global::verbose_level1)
        clog << "MAIN joined ORB run thread" << endl;

    // Disconnect - retrying on Comms Failure.
    disconnect([&proxy_consumer]() { proxy_consumer->disconnect_pull_consumer();},
    [](){ cerr << "Caught COMM_FAILURE exception disconnecting Pull Supplier! Retrying..." << endl; });

    if(global::verbose_level1)
        clog << "MAIN disconnected Proxy Pull Consumer" << endl;

    // Disconnect - retrying on Comms Failure.
    disconnect([&proxy_supplier](){ proxy_supplier->disconnect_push_supplier(); },
    [](){ cerr << "Caught COMM_FAILURE Exception disconnecting Push Consumer! Retrying..." << endl; });

    if(global::verbose_level1)
        clog << "MAIN disconnected Proxy Push Supplier" << endl;

    return 0;
}

void* run_orb(void *)
{
    if(global::verbose_level1)
        clog << "ORB_RUN running ORB" << endl;
    OmniEvents::Orb::inst().run();
    if(global::verbose_level1)
        clog << "ORB_RUN done" << endl;
    return nullptr;
}

void render_on_engine(unique_ptr<Shape> const &shape_ptr, LogRenderEngine &engine)
{
    if(!shape_ptr)
        cout << "no shape to display" << endl;
    else
    {
        shape_ptr->render(engine);
        cout << engine.result() << endl;
        engine.clear();
    }
}

void show_shape(unique_ptr<Shape> const &shape_ptr)
{
    if(!shape_ptr)
        cout << "no shape to show" << endl;
    else
        cout << *shape_ptr << endl;
}

