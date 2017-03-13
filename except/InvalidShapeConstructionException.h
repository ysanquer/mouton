#ifndef _INVALID_SHAPE_CONSTRUCTION_EXCEPTION
#define _INVALID_SHAPE_CONSTRUCTION_EXCEPTION

#include "ShapeException.h"

///
/// \brief A ShapeException thrown when trying to construct a Shape
///  with invalid data
///
/// \authors Yann Sanquer
///
class InvalidShapeConstructionException: public ShapeException
{
    public:
        ///
        /// \brief Constructor
        ///
        inline
        InvalidShapeConstructionException(std::string const &what_arg)
        : ShapeException(what_arg)
        {}

        ///
        /// \brief Destructor
        ///
        inline virtual ~InvalidShapeConstructionException() {}
};

#endif
