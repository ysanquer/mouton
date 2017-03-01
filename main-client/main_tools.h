#ifndef _MAIN_TOOLS_H
#define _MAIN_TOOLS_H

#include <functional>
#include <omniORB4/CORBA.h>

typedef std::function<void()> Proc;

template<typename T, typename TSupplier>
T obtain(TSupplier ts, Proc nil_case, Proc comm_failure_case);
void connect(Proc do_connect, Proc on_bad_param, Proc on_already_connected, Proc on_comm_failure);
void disconnect(Proc do_disconnect, Proc on_comm_failure);
void usage();

template<typename T, typename TSupplier>
T obtain(TSupplier ts, Proc nil_case, Proc comm_failure_case)
{
    T t;
    while (1)
    {
        try
        {
            t = ts();
            if (CORBA::is_nil(t))
            {
                nil_case();
                exit(1);
            }
            break;
        }
        catch (CORBA::COMM_FAILURE& ex)
        {
            comm_failure_case();
            continue;
        }
    }
    return t;
}

void connect(Proc do_connect, Proc on_bad_param, Proc on_already_connected, Proc on_comm_failure)
{
    // Connect - retrying on Comms Failure.
    while (1)
    {
        try
        {
            do_connect();
            break;
        }
        catch (CORBA::BAD_PARAM& ex)
        {
            on_bad_param();
            exit(1);
        }
        catch (CosEventChannelAdmin::AlreadyConnected& ex)
        {
            on_already_connected();
            break;
        }
        catch (CORBA::COMM_FAILURE& ex)
        {
            on_comm_failure();
            continue;
        }
    }
}

void disconnect(Proc do_disconnect, Proc on_comm_failure)
{
    // Disconnect - retrying on Comms Failure.
    while (1)
    {
        try
        {
            do_disconnect();
            break;
        }
        catch (CORBA::COMM_FAILURE& ex)
        {
            on_comm_failure();
            continue;
        }
    }
}

void usage()
{
    cerr << "usage: " 
         << "mouton [-hv] [-c CHANNEL_NAME] NAME [EVENT_CHANNEL_REF]" << endl 
         << "NAME              your name" << endl  
         << "EVENT_CHANNEL_REF reference to event channel" << endl 
         << "-h                display this" << endl 
         << "-v                be more verbose (once: log what is being done;" << endl 
         << "                   twice: log details; more: log information" << endl 
         << "                   from periodic events)" << endl 
         << "-c CHANNEL_NAME   try to find event channel of name CHANNEL_NAME" << endl 
         << "                   in name service if EVENT_CHANNEL_REF not" << endl 
         << "                   specified" << endl;
}


#endif
