#ifndef _DEFINED_LINE_SEGMENT_H
#define _DEFINED_LINE_SEGMENT_H

#include "AbstractDefinedLine.h"
#include <memory>

///
/// \brief Shape implementation for defined line segments
///
/// Defined line segments are defined with an origin point, and a
///  direction vector which defines where the second point is compared
///  to the origin
///
class DefinedLineSegment: public AbstractDefinedLine
{
    public:
        ///
        /// \brief Origin and direction constructor
        ///
        DefinedLineSegment(Vector const &, Vector const &);

        ///
        /// \brief Destructor
        ///
        virtual ~DefinedLineSegment();

        virtual _CloneUPtr clone() const override;

        virtual double perimeter() const override;
        virtual double distance(Vector const &) const override;
        virtual bool contains_point(Vector const &) const override;

        virtual void render(RenderEngine &) const override;

        ///
        /// Move assignment operator
        ///
        DefinedLineSegment &operator=(DefinedLineSegment &&);
    protected:
        virtual void print_as_expression(std::ostream &) const override;
};

#endif
