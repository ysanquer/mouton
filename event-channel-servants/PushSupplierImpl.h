#ifndef _PULL_SUPPLIER_IMPL_H
#define _PULL_SUPPLIER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

///
/// \brief Implementation of a Push Supplier
///
/// \authors Yann Sanquer
///
/// This implementation lets its user specify delegates to its member
///  functions
///
class PushSupplierImpl: virtual public POA_CosEventComm::PushSupplier
{
    public:
        ///
        /// \brief std::function type specialization to the
        ///  disconnect_push_supplier member function
        ///
        typedef std::function<void()> DisconnectFn;

    private:
        DisconnectFn _disconnect_push_supplier_delegate;

    public:
        ///
        /// \brief Constructor
        ///
        PushSupplierImpl();

        ///
        /// \brief Disconnect this Push Supplier
        ///
        virtual void disconnect_push_supplier() override;

        ///
        /// \brief Set the delegate to the disconnect_push_supplier
        ///  member function
        ///
        void set_disconnect_push_supplier(DisconnectFn const &disconnect_push_supplier_delegate);
};

#endif
