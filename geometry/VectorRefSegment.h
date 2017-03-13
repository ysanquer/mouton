#ifndef _VECTOR_REF_SEGMENT_H
#define _VECTOR_REF_SEGMENT_H

#include "AbstractLine.h"
#include <memory>
#include <iostream>

///
/// \brief Shape implementation of a non-fixed line segment
///
/// \authors Yann Sanquer
///
/// This line segment is defined with references to its end points
///
class VectorRefSegment : public AbstractLine
{
    protected:
        ///
        /// \brief First end of the segment
        ///
        Vector &_end1;

        ///
        /// \brief Second end of the segment
        ///
        Vector &_end2;

    public:
        ///
        /// \brief End points constructor
        ///
        VectorRefSegment(Vector &, Vector &);

        ///
        /// \brief Destructor
        ///
        virtual ~VectorRefSegment();

        virtual _CloneUPtr clone() const override;

        virtual double perimeter() const override;
        virtual double distance(Vector const &) const override;
        virtual bool contains_point(Vector const &) const override;
        virtual void render(RenderEngine &) const override;
        virtual VectorRefSegment &homothetic(Vector const&, double) override;
        virtual VectorRefSegment &translate(Vector const&) override;
        virtual VectorRefSegment &rotate(Vector const&, double) override;
        virtual VectorRefSegment &symmetry(Vector const &) override;
        virtual VectorRefSegment &symmetry(Vector const &, Vector const &) override;

    protected:
        virtual void print_as_expression(std::ostream &) const override;

};

#endif
