#ifndef _CLONEABLE_H
#define _CLONEABLE_H

#include <memory>

///
/// \brief Makes an object cloneable
/// \authors Yann Sanquer
/// \authors Gabriel Bruit
/// \authors Jérémy Montanes
/// 
/// A cloneable object of type T typically derives Cloneable<T> and
///  redefines the method clone() to return copies of itself at call
///  time
///
/// A factory object F of T can also be created by deriving Cloneable<T>
///  and redefining the method clone() to return a clone of a template
///  object, for example
///
template<typename T>
class Cloneable
{
    public:
        ///
        /// \brief Unique pointer type
        ///
        typedef std::unique_ptr<T> _CloneUPtr;

        ///
        /// \brief Destructor
        ///
        virtual ~Cloneable() = 0;

        ///
        /// \brief Clones the current object, returning an instance of T
        ///  wrapped in a unique pointer type
        ///
        virtual _CloneUPtr clone() const = 0;
};

template<typename T>
inline Cloneable<T>::~Cloneable()
{
}

#endif
