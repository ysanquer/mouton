#include <iostream>
#include <limits>
#include <unistd.h>

#include "ProcShell.h"
#include "ShapeELParser.h"
#include "Shape.h"
#include "Vector.h"
#include "LogRenderEngine.h"

#include "PullConsumerImpl.h"
#include "PushSupplierImpl.h"

#include <CosEventComm.hh>
#include <CosEventChannelAdmin.hh>
#include <omniEvents/EventChannel.h>
#include "naming.h"

#include "named-drawing.hh"
#include "annotated-drawing.hh"

#include "main_tools.h"
#include "global_verbosity.h"

#include "functions.h"
#include "Image.h"

#include <signal.h>

using namespace std;

void* run_orb(void *);
void render_on_engine(unique_ptr<Shape> const &, LogRenderEngine &);
void show_shape(unique_ptr<Shape> const &);

/** /author Jeremy Montanes */
int main(int argc, char *argv[])
{
    // Named Drawing Pull Consumer
    shared_ptr<PullConsumerImpl> ndpc;
    // Annotated Drawing Push Supplier
    shared_ptr<PushSupplierImpl> adps;

    char const *channelName = "EventChannel";
    string name = "";

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

    ndpc = make_shared<PullConsumerImpl>();
    adps = make_shared<PushSupplierImpl>();

    unique_ptr<Shape> to_send;

    ndpc->set_disconnect_pull_consumer([]()
    {
        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_CONSUMER/DISCONNECT begin" << endl;
        if(global::verbose_level3)
            clog << "NAMED_DRAWING_PULL_CONSUMER/DISCONNECT end" << endl;
    });

    if(global::verbose_level2)
        clog << "MAIN configured NAMED_DRAWING_PULL_CONSUMER" << endl;

    adps->set_disconnect_push_supplier([]()
    {
        if(global::verbose_level3)
            clog << "ANNOTATED_DRAWING_PUSH_SUPPLIER/DISCONNECT begin" << endl;
        if(global::verbose_level3)
            clog << "ANNOTATED_DRAWING_PUSH_SUPPLIER/DISCONNECT end" << endl;
    });

    if(global::verbose_level2)
        clog << "MAIN configured ANNOTATED_DRAWING_PUSH_SUPPLIER" << endl;

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
    CosEventChannelAdmin::ProxyPushConsumer_var proxy_consumer
        = obtain<CosEventChannelAdmin::ProxyPushConsumer_var>(
            [&supplier_admin](){ return supplier_admin->obtain_push_consumer(); },
            []()
            {
                cerr << "Supplier Admin returned nil proxy_consumer!" << endl;
            }, []()
            {
                cerr << "Caught COMM_FAILURE exception "
                    << "obtaining Proxy Push Consumer! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN obtained Proxy Push Consumer from Supplier Admin" << endl;

    //
    // Get proxy supplier - retrying on Comms Failure.
    CosEventChannelAdmin::ProxyPullSupplier_var proxy_supplier
        = obtain<CosEventChannelAdmin::ProxyPullSupplier_var>(
            [&consumer_admin](){ return consumer_admin->obtain_pull_supplier(); },
            []()
            {
                cerr << "Consumer Admin returned nil proxy_supplier!" << endl;
            }, []()
            {
                cerr << "Caught COMM_FAILURE Exception "
                    << "obtaining Pull Supplier! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN obtained Proxy Pull Supplier from Consumer Admin" << endl;

    // Connect Push Supplier - retrying on Comms Failure.

    connect([&adps, &proxy_consumer]()
    {
        CosEventComm::PushSupplier_var supplierRef =adps->_this();
        proxy_consumer->connect_push_supplier(supplierRef.in());
    }, []()
    {
        cerr<<"Caught BAD_PARAM Exception connecting Push Supplier!"<<endl;
    },[]()
    {
        cerr << "Push Supplier already connected!" << endl;
    },[]()
    {
        cerr << "Caught COMM_FAILURE exception "
            << "connecting Push Supplier! Retrying..."
            << endl;
    });

    if(global::verbose_level1)
        clog << "MAIN connected Push Supplier to Proxy Pull Consumer" << endl;

    // Connect Pull Consumer - retrying on Comms Failure.
    connect([&proxy_supplier, &ndpc](){ proxy_supplier->connect_pull_consumer(ndpc->_this()); },
            []()
            {
                cerr << "Caught BAD_PARAM Exception connecting Pull Consumer!" << endl;
            },
            []()
            {
                cerr << "Proxy Pull Supplier already connected!" << endl;
            },
            []()
            {
                cerr << "Caught COMM_FAILURE exception "
                    << "connecting Pull Consumer! Retrying..."
                    << endl;
            });

    if(global::verbose_level1)
        clog << "MAIN connected Pull Consumer to Proxy Pull Supplier" << endl;

    omni_thread *th = omni_thread::create(run_orb);

    if(global::verbose_level1)
        clog << "MAIN started ORB run thread" << endl;

    ProcShell         shell;
    ShapeELParser     selp;
    unique_ptr<Shape> shape_ptr;
    LogRenderEngine   engine;
    map< string, unique_ptr<Shape> > drawings_by_name;
    list< string > whose_drawings_on_fresco;

    shell
        .when("cl", [](){ cout << "cl xp hello rcv rcvlist rcvdisplay annotate select frescolist drop createfresco" << endl; }, "show command list")
        .when("createfresco",
            [&whose_drawings_on_fresco, &drawings_by_name, &engine]()
            {
                Image im = Image::create(- numeric_limits<double>::infinity(), numeric_limits<double>::infinity(), - numeric_limits<double>::infinity(), numeric_limits<double>::infinity());
                Color default_color{0,0,0};
                for(auto it = whose_drawings_on_fresco.begin(); it != whose_drawings_on_fresco.end(); it++)
                {
                    unique_ptr<Shape> &shape_ptr = drawings_by_name[*it];
                    im.insert(*shape_ptr, default_color);
                }
                cout << "fresco expression: " << im << endl;
                im.render(engine);
                cout << "fresco render:" << endl << engine.result() << endl;
                engine.clear();
            }, "create fresco from collected drawings of authors on the fresco list")
        .when("drop",
            [&whose_drawings_on_fresco]()
            {
                string author_name;
                cout << "Who?" << endl << "moutond|drop> ";
                if(getline(cin, author_name))
                {
                    str_trim_ws(author_name);
                    auto it = find(whose_drawings_on_fresco.begin(), whose_drawings_on_fresco.end(), author_name);
                    if(it == whose_drawings_on_fresco.end())
                        cout << author_name << " not on the fresco list" << endl;
                    else
                    {
                        whose_drawings_on_fresco.erase(it);
                        cout << author_name << " removed from fresco list" << endl;
                    }
                }
            }, "drop an author's name from the fresco list")
        .when("frescolist",
            [&whose_drawings_on_fresco]()
            {
                if(whose_drawings_on_fresco.empty())
                    cout << "no one yet" << endl;
                else
                {
                    auto it = whose_drawings_on_fresco.begin();
                    cout << *it;
                    it++;
                    while(it != whose_drawings_on_fresco.end())
                    {
                        cout << " " << *it;   
                        it++;
                    }
                    cout << endl;
                }
            }, "list authors selected to contribute to the fresco")
        .when("select",
            [&whose_drawings_on_fresco, &drawings_by_name]()
            {
                string author_name;
                cout << "Who?" << endl << "moutond|select> ";
                if(getline(cin, author_name))
                {
                    str_trim_ws(author_name);
                    auto shape_it = drawings_by_name.find(author_name);
                    if(shape_it == drawings_by_name.end())
                        cout << "no one named " << author_name << " sent you anything" << endl;
                    else
                    {
                        auto fresco_it = find(whose_drawings_on_fresco.begin(), whose_drawings_on_fresco.end(), author_name);
                        if(fresco_it != whose_drawings_on_fresco.end())
                            cout << author_name << " already on the fresco list" << endl;
                        else
                        {
                            whose_drawings_on_fresco.push_back(author_name);
                            cout << author_name << " selected for the fresco" << endl;
                        }
                    }
                }
            }, "select an author whose drawing goes on the fresco")
        .when("annotate",
            [&drawings_by_name, &proxy_consumer, &name]()
            {
                string author_name;
                cout << "Whose drawing?" << endl << "moutond|annotate> ";
                if(getline(cin, author_name))
                {
                    str_trim_ws(author_name);
                    auto shape_it = drawings_by_name.find(author_name);
                    if(shape_it == drawings_by_name.end())
                        cout << "no one named " << author_name << " sent you anything" << endl;
                    else
                    {
                        unique_ptr<Shape> &shape(shape_it->second);
                        string annotations;
                        cout << "Enter your comments (one line)" << endl << "moutond|annotate> ";
                        if(!getline(cin, annotations))
                        {
                            cout << "could not read annotations" << endl;
                        }
                        else
                        {
                            str_trim_ws(annotations);
                            fr::upem::mouton::AnnotatedDrawing ad;
                            ad.drawing_expr = dynamic_cast<stringstream&>(stringstream() << *shape).str().c_str();
                            ad.annotations = annotations.c_str();
                            ad.author_name = author_name.c_str();
                            ad.name = name.c_str();

                            CORBA::Any any;
                            any <<= ad;

                            proxy_consumer->push(any);
                        }
                    }
                }
            }, "send annotation about a drawing")
        .when("rcvdisplay",
            [&drawings_by_name, &engine]()
            {
                string author_name;
                cout << "Whose drawing?" << endl << "moutond|rcvdisplay> ";
                if(getline(cin, author_name))
                {
                    str_trim_ws(author_name);
                    auto shape_it = drawings_by_name.find(author_name);
                    if(shape_it == drawings_by_name.end())
                        cout << "no one named " << author_name << " sent you anything" << endl;
                    else
                    {
                        unique_ptr<Shape> &shape(shape_it->second);
                        cout << "shape expression: " << *shape << endl;
                        shape->render(engine);
                        cout << "render result:" << endl;
                        cout << engine.result() << endl;
                        engine.clear();
                    }
                }
            }, "display a drawing sent by a given author")
        .when("rcvlist",
            [&drawings_by_name]()
            {
                if(drawings_by_name.size() == 0)
                {
                    cout << "no one to list" << endl;
                }
                else
                {
                    auto it = drawings_by_name.begin();
                    while(it != drawings_by_name.end())
                    {
                        cout << " " << it->first;
                        it++;
                    }
                    cout << endl;
                }
            }, "list whose drawings you have")
        .when("hello", [](){ cout << "Hello, world" << endl; }, "say hello")
        .when("xp",
            [&shell]()
            {
                string com;
                cout << "moutond|xp> ";
                if(getline(cin, com))
                {
                    str_trim_ws(com);
                    cout << com << ": " << shell.explain(com) << endl;
                }
            },"explain command")
        .when("rcv",
            [&proxy_supplier, &selp, &drawings_by_name, &engine]()
            {
                CORBA::Any* data(nullptr);
                CORBA::Boolean has_event(false);

                while(true)
                {
                    if(global::verbose_level2)
                        clog << "MAIN trying to pull for a Named Drawing" << endl;

                    data = proxy_supplier->try_pull(has_event);
                    if(!has_event)
                    {
                        cout << "nothing collected" << endl;
                        break;
                    }
                    else
                    {
                        if(global::verbose_level2)
                            clog << "MAIN trying to extract Named Drawing" << endl;

                        fr::upem::mouton::NamedDrawing *nd(nullptr);
                        if(!(*data >>= nd))
                        {
                            cout << "nothing collected" << endl;
                        }
                        else
                        {
                            string name = (char*) nd->name;
                            string drawing_expr = (char*) nd->drawing_expr;
                            istringstream iss(drawing_expr);

                            cout << "from " << name << ": ";

                            if(!(iss >> selp))
                            {
                                cout << "bad shape expression" << endl;
                            }
                            else
                            {
                                unique_ptr<Shape> parsed_shape = selp.parsed_shape();
                                cout << "received new shape, see it? [y to display]" << endl << "moutond|rcv> ";
                                string choice;
                                if(!getline(cin, choice))
                                    cout << "failed to read choice, dropped received shape" << endl;
                                else
                                {
                                    str_trim_ws(choice);
                                    if(choice != "y")
                                        cout << "dropped received shape" << endl;
                                    else
                                    {
                                        cout << "shape expression: " << *parsed_shape << endl;
                                        parsed_shape->render(engine);
                                        cout << "shape render:" << endl;
                                        cout << engine.result() << endl;
                                        engine.clear();
                                        cout << "register " << name << "'s shape? [y to register]" << endl << "moutond|rcv> ";
                                        if(!getline(cin, choice))
                                            cout << "failed to read choice, dropped received shape" << endl;
                                        else
                                        {
                                            if(choice != "y")
                                            {
                                                cout << "dropped received shape" << endl;
                                            }
                                            else
                                            {
                                                drawings_by_name[name] = move(parsed_shape);
                                                cout << "registered" << endl;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        delete data;
                    }
                }
            }, "receive pending shapes");

    if(global::verbose_level1)
        clog << "MAIN prepared shell" << endl;

    cout << "Mouton Server Shell -- type 'cl' for list of commands, 'xp' to explain a command" << endl;
    while(true)
    {
        do
        {
            cout << "moutond> ";
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

    OmniEvents::Orb::inst().shutdown(0);

    th->join(nullptr);

    if(global::verbose_level1)
        clog << "MAIN joined ORB run thread" << endl;

    // Disconnect - retrying on Comms Failure.
    disconnect([&proxy_consumer]() { proxy_consumer->disconnect_push_consumer();},
    [](){ cerr << "Caught COMM_FAILURE exception disconnecting Push Supplier! Retrying..." << endl; });

    if(global::verbose_level1)
        clog << "MAIN disconnected Proxy Push Consumer" << endl;

    // Disconnect - retrying on Comms Failure.
    disconnect([&proxy_supplier](){ proxy_supplier->disconnect_pull_supplier(); },
    [](){ cerr << "Caught COMM_FAILURE Exception disconnecting Pull Consumer! Retrying..." << endl; });

    if(global::verbose_level1)
        clog << "MAIN disconnected Proxy Pull Supplier" << endl;

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

