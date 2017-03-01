#ifndef _PULL_SUPPLIER_IMPL_H
#define _PULL_SUPPLIER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

class PushSupplierImpl: virtual public POA_CosEventComm::PushSupplier
{
    public:
        typedef std::function<void()> DisconnectFn;

    private:
        DisconnectFn _disconnect_push_supplier_delegate;

    public:
        PushSupplierImpl();

        virtual void disconnect_push_supplier() override;

        void set_disconnect_push_supplier(DisconnectFn const &disconnect_push_supplier_delegate);
};

#endif
