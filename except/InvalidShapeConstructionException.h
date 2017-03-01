#ifndef _INVALID_SHAPE_CONSTRUCTION_EXCEPTION
#define _INVALID_SHAPE_CONSTRUCTION_EXCEPTION

#include "ShapeException.h"

class InvalidShapeConstructionException: public ShapeException
{
    public:
        inline
        InvalidShapeConstructionException(std::string const &what_arg)
        : ShapeException(what_arg)
        {}

        inline virtual ~InvalidShapeConstructionException() {}
};

#endif
