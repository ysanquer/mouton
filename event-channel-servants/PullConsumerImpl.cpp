#include "PullConsumerImpl.h"

PullConsumerImpl::PullConsumerImpl() : _disconnect_pull_consumer_delegate()
{
}

void PullConsumerImpl::disconnect_pull_consumer()
{
    if(_disconnect_pull_consumer_delegate)
        _disconnect_pull_consumer_delegate();
}

void PullConsumerImpl::set_disconnect_pull_consumer(DisconnectFn const &disconnect_pull_consumer_delegate)
{
    _disconnect_pull_consumer_delegate = disconnect_pull_consumer_delegate;
}
