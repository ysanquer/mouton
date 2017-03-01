#include "PushConsumerImpl.h"

#include "Shape.h"
#include "annotated-drawing.hh"

#include <sstream>

using namespace std;

PushConsumerImpl::PushConsumerImpl() : _push_delegate(), _disconnect_push_consumer_delegate()
{
}

void PushConsumerImpl::push(CORBA::Any const &data)
{
    if(_push_delegate)
        _push_delegate(data);
}

void PushConsumerImpl::disconnect_push_consumer()
{
    if(_disconnect_push_consumer_delegate)
        _disconnect_push_consumer_delegate();
}

void PushConsumerImpl::set_push(PushConsumerImpl::PushFn const &push_delegate)
{
    _push_delegate = push_delegate;
}

void PushConsumerImpl::set_disconnect_push_consumer(PushConsumerImpl::DisconnectFn const &disconnect_push_consumer_delegate)
{
    _disconnect_push_consumer_delegate = disconnect_push_consumer_delegate;
}
