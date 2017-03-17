#ifndef _PULL_SUPPLIER_IMPL_H
#define _PULL_SUPPLIER_IMPL_H

#include <CosEventComm.hh>

#include <functional>
#include <string>

///
/// \brief Implementation of a Pull Supplier
///
/// \authors Gabriel Bruit
///
/// This implementation allows its user to specify delegate functions
///  called on this Pull Supplier
///
class PullSupplierImpl: virtual public POA_CosEventComm::PullSupplier
{
    public:
        ///
        /// \brief std::function type specialization for the pull
        ///  member function
        ///
        typedef std::function<CORBA::Any *()> PullFn;

        ///
        /// \brief std::function type specialization for the try_pull
        ///  member function
        ///
        typedef std::function<CORBA::Any *(CORBA::Boolean &)> TryPullFn;

        ///
        /// \brief std::function type specialization for the
        ///  disconnect_pull_supplier member function
        ///
        typedef std::function<void()> DisconnectFn;

    private:
        PullFn _pull_delegate;
        TryPullFn _try_pull_delegate;
        DisconnectFn _disconnect_pull_supplier_delegate;

    public:
        ///
        /// \brief Constructor
        ///
        PullSupplierImpl();

        ///
        /// \brief Blocking member function for retrieving an element
        ///
        /// Calls this->try_pull in a loop
        ///
        virtual CORBA::Any *pull() override;
        
        ///
        /// \brief Non-blocking member function for retrieveing an element
        ///
        /// The has_event argument is used by this function to specify
        ///  whether an element was returned
        ///
        virtual CORBA::Any *try_pull(CORBA::Boolean &has_event) override;
        
        ///
        /// \brief Disconnect this Pull Supplier
        ///
        virtual void disconnect_pull_supplier() override;

        ///
        /// \brief Set the delegate to the pull member function
        ///
        void set_pull(PullFn const &pull_delegate);
        
        ///
        /// \brief Set the delegate to the try_pull member function
        ///
        void set_try_pull(TryPullFn const &try_pull_delegate);

        ///
        /// \brief Set the delegate to the disconnect_pull_supplier
        ///  member function
        ///
        void set_disconnect_pull_supplier(DisconnectFn const &disconnect_pull_supplier_delegate);
};

#endif
