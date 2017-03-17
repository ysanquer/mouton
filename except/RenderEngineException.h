#ifndef _RENDER_ENGINE_EXCEPTION
#define _RENDER_ENGINE_EXCEPTION

#include <exception>

///
/// \brief An exception thrown by a RenderEngine
///
/// \authors Jeremy Montanes
///
class RenderEngineException: public std::exception
{
    private:
        std::string _what_arg;

    public:
        ///
        /// \brief Constructor
        ///
        inline
        RenderEngineException(std::string const &what_arg)
        : std::exception(), _what_arg(what_arg)
        {}

        ///
        /// \brief Destructor
        ///
        inline virtual ~RenderEngineException() {}

        ///
        /// \brief Obtain exception message
        ///
        inline virtual char const *what() const throw() {return _what_arg.c_str();}
};

#endif
