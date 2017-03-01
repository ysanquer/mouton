#ifndef _INVALID_RENDER_DATA_EXCEPTION
#define _INVALID_RENDER_DATA_EXCEPTION

#include "RenderEngineException.h"

class InvalidRenderDataException: public RenderEngineException
{
    public:
        inline
        InvalidRenderDataException(std::string const &what_arg)
        : RenderEngineException(what_arg)
        {}

        inline virtual ~InvalidRenderDataException() {}
};

#endif
