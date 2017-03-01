#ifndef _SHAPE_EXCEPTION
#define _SHAPE_EXCEPTION

#include <exception>

class ShapeException: public std::exception
{
    private:
        std::string _what_arg;

    public:
        inline
        ShapeException(std::string const &what_arg)
        : std::exception(), _what_arg(what_arg)
        {}

        inline virtual ~ShapeException() {}

        inline virtual char const *what() const throw() {return _what_arg.c_str();}
};

#endif
