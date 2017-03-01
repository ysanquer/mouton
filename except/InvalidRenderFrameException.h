#ifndef _INVALID_RENDER_FRAME_EXCEPTION
#define _INVALID_RENDER_FRAME_EXCEPTION

#include "RenderEngineException.h"

class InvalidRenderFrameException: public RenderEngineException
{
    public:
        inline
        InvalidRenderFrameException(std::string const &what_arg)
        : RenderEngineException(what_arg)
        {}

        inline virtual ~InvalidRenderFrameException() {}
};

#endif
