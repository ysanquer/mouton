#ifndef _PULL_CONSUMER_IMPL_H
#define _PULL_CONSUMER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

class PullConsumerImpl: virtual public POA_CosEventComm::PullConsumer
{
    public:
        typedef std::function<void()> DisconnectFn;

    private:
        DisconnectFn _disconnect_pull_consumer_delegate;

    public:
        PullConsumerImpl();

        virtual void disconnect_pull_consumer() override;

        void set_disconnect_pull_consumer(DisconnectFn const &disconnect_pull_consumer_delegate);
};

#endif
