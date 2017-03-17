#ifndef _PULL_CONSUMER_IMPL_H
#define _PULL_CONSUMER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

///
/// \brief Implementation of a Push Consumer
///
/// \authors Gabriel Bruit
///
/// This implementation lets its user specify delegates to its
///  member functions.
///
class PushConsumerImpl: virtual public POA_CosEventComm::PushConsumer
{
    public:
        ///
        /// \brief std::function type specialization for the push
        ///  member function
        ///
        typedef std::function<void(CORBA::Any const &)> PushFn;

        ///
        /// \brief std::function type specialization for the
        ///  disconnect_push_consumer member function
        ///
        typedef std::function<void()> DisconnectFn;

    private:
        PushFn _push_delegate;
        DisconnectFn _disconnect_push_consumer_delegate;

    public:
        ///
        /// \brief Constructor
        ///
        PushConsumerImpl();

        ///
        /// \brief Push data to this Push Consumer
        ///
        virtual void push(CORBA::Any const &data) override;

        ///
        /// \brief Disconnect this Push Consumer
        ///
        virtual void disconnect_push_consumer() override;

        ///
        /// \brief Set the delegate to the push member function
        ///
        void set_push(PushFn const &push_delegate);

        ///
        /// \brief Set the delegate to the disconnect_push_consumer
        ///  member function
        ///
        void set_disconnect_push_consumer(DisconnectFn const &disconnect_push_consumer_delegate);
};

#endif
