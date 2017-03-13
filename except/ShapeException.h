#ifndef _SHAPE_EXCEPTION
#define _SHAPE_EXCEPTION

#include <exception>

///
/// \brief An exception thrown by Shape, its subclasses and systems
///  that work with them
///
/// \authors Yann Sanquer
///
class ShapeException: public std::exception
{
    private:
        std::string _what_arg;

    public:
        ///
        /// \brief Constructor
        ///
        inline
        ShapeException(std::string const &what_arg)
        : std::exception(), _what_arg(what_arg)
        {}

        ///
        /// \brief Destructor
        ///
        inline virtual ~ShapeException() {}

        ///
        /// \brief Obtain explain message
        ///
        inline virtual char const *what() const throw() {return _what_arg.c_str();}
};

#endif
