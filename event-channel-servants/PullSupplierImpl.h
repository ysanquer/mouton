#ifndef _PULL_SUPPLIER_IMPL_H
#define _PULL_SUPPLIER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

class PullSupplierImpl: virtual public POA_CosEventComm::PullSupplier
{
    public:
        typedef std::function<CORBA::Any *()> PullFn;
        typedef std::function<CORBA::Any *(CORBA::Boolean &)> TryPullFn;
        typedef std::function<void()> DisconnectFn;

    private:
        PullFn _pull_delegate;
        TryPullFn _try_pull_delegate;
        DisconnectFn _disconnect_pull_supplier_delegate;

    public:
        PullSupplierImpl();

        virtual CORBA::Any *pull() override;
        virtual CORBA::Any *try_pull(CORBA::Boolean &has_event) override;
        virtual void disconnect_pull_supplier() override;

        void set_pull(PullFn const &pull_delegate);
        void set_try_pull(TryPullFn const &try_pull_delegate);
        void set_disconnect_pull_supplier(DisconnectFn const &disconnect_pull_supplier_delegate);
};

#endif
