#ifndef _PULL_CONSUMER_IMPL_H
#define _PULL_CONSUMER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

class PushConsumerImpl: virtual public POA_CosEventComm::PushConsumer
{
    public:
        typedef std::function<void(CORBA::Any const &)> PushFn;
        typedef std::function<void()> DisconnectFn;

    private:
        PushFn _push_delegate;
        DisconnectFn _disconnect_push_consumer_delegate;

    public:
        PushConsumerImpl();

        virtual void push(CORBA::Any const &data) override;
        virtual void disconnect_push_consumer() override;

        void set_push(PushFn const &push_delegate);
        void set_disconnect_push_consumer(DisconnectFn const &disconnect_push_consumer_delegate);
};

#endif
