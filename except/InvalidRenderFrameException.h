#ifndef _INVALID_RENDER_FRAME_EXCEPTION
#define _INVALID_RENDER_FRAME_EXCEPTION

#include "RenderEngineException.h"

///
/// \brief A RenderEngineException thrown when the engine's render
///  frame is set to an invalid state
///
/// \authors Gabriel Bruit
///
class InvalidRenderFrameException: public RenderEngineException
{
    public:
        ///
        /// \brief Constructor
        ///
        inline
        InvalidRenderFrameException(std::string const &what_arg)
        : RenderEngineException(what_arg)
        {}

        ///
        /// \brief Destructor
        ///
        inline virtual ~InvalidRenderFrameException() {}
};

#endif
