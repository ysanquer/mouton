#ifndef _ABSTRACT_LINE_H
#define _ABSTRACT_LINE_H

#include "Shape.h"

///
/// \brief Base class for any line object
///
/// A line object has by definition no area
///
class AbstractLine : public Shape
{
    public:
        ///
        /// Destructor
        ///
        virtual ~AbstractLine() = 0;

        ///
        /// Returns zero by definition
        ///
        virtual double area() const override;
};

#endif
