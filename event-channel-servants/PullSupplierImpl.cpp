#include "PullSupplierImpl.h"

#include "Shape.h"
#include "named-drawing.hh"
#include <stdexcept>

#include <sstream>

using namespace std;

PullSupplierImpl::PullSupplierImpl() : _pull_delegate(), _try_pull_delegate(), _disconnect_pull_supplier_delegate()
{
}

CORBA::Any *PullSupplierImpl::pull()
{
    if(_pull_delegate)
        return _pull_delegate();
    return nullptr;
}

CORBA::Any *PullSupplierImpl::try_pull(CORBA::Boolean &has_event)
{
    if(_try_pull_delegate)
        return _try_pull_delegate(has_event);
    return nullptr;
}

void PullSupplierImpl::disconnect_pull_supplier()
{
    if(_disconnect_pull_supplier_delegate)
        _disconnect_pull_supplier_delegate();
}

void PullSupplierImpl::set_pull(PullSupplierImpl::PullFn const &pull_delegate)
{
    _pull_delegate = pull_delegate;
}

void PullSupplierImpl::set_try_pull(PullSupplierImpl::TryPullFn const &try_pull_delegate)
{
    _try_pull_delegate = try_pull_delegate;
}

void PullSupplierImpl::set_disconnect_pull_supplier(PullSupplierImpl::DisconnectFn const &disconnect_pull_supplier_delegate)
{
    _disconnect_pull_supplier_delegate = disconnect_pull_supplier_delegate;
}
