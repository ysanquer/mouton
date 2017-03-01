#include "PushSupplierImpl.h"

PushSupplierImpl::PushSupplierImpl(): _disconnect_push_supplier_delegate()
{
}

void PushSupplierImpl::disconnect_push_supplier()
{
    if(_disconnect_push_supplier_delegate)
        _disconnect_push_supplier_delegate();
}

void PushSupplierImpl::set_disconnect_push_supplier(PushSupplierImpl::DisconnectFn const &disconnect_push_supplier_delegate)
{
    _disconnect_push_supplier_delegate = disconnect_push_supplier_delegate;
}
