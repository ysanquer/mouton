#ifndef _PULL_CONSUMER_IMPL_H
#define _PULL_CONSUMER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

///
/// \brief Implementation of a Pull Consumer
///
/// \authors Yann Sanquer
///
/// This implementation allows its user to provide it with the
///  usual functions for a Pull Consumer
///
class PullConsumerImpl: virtual public POA_CosEventComm::PullConsumer
{
    public:
        ///
        /// \brief The std::function specialization of
        ///  the disconnect_pull_consumer member function
        ///
        typedef std::function<void()> DisconnectFn;

    private:
        DisconnectFn _disconnect_pull_consumer_delegate;

    public:
        ///
        /// \brief Constructor
        ///
        PullConsumerImpl();

        ///
        /// \brief Disconnect this pull consumer
        ///
        virtual void disconnect_pull_consumer() override;

        ///
        /// \brief Set the disconnect_pull_consumer member function delegate
        ///
        void set_disconnect_pull_consumer(DisconnectFn const &disconnect_pull_consumer_delegate);
};

#endif
