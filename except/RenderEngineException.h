#ifndef _RENDER_ENGINE_EXCEPTION
#define _RENDER_ENGINE_EXCEPTION

#include <exception>

class RenderEngineException: public std::exception
{
    private:
        std::string _what_arg;

    public:
        inline
        RenderEngineException(std::string const &what_arg)
        : std::exception(), _what_arg(what_arg)
        {}

        inline virtual ~RenderEngineException() {}

        inline virtual char const *what() const throw() {return _what_arg.c_str();}
};

#endif
