#ifndef _INVALID_RENDER_DATA_EXCEPTION
#define _INVALID_RENDER_DATA_EXCEPTION

#include "RenderEngineException.h"

///
/// \brief A RenderEngineException thrown when an engine's method is
///  called with invalid arguments
///
/// \authors Gabriel Bruit
///
class InvalidRenderDataException: public RenderEngineException
{
    public:
        ///
        /// \brief Constructor
        ///
        inline
        InvalidRenderDataException(std::string const &what_arg)
        : RenderEngineException(what_arg)
        {}

        ///
        /// \brief Destructor
        ///
        inline virtual ~InvalidRenderDataException() {}
};

#endif
