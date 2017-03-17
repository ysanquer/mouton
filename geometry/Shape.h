#ifndef _SHAPE_H
#define _SHAPE_H

#include "Transformable.h"
#include "Metric.h"
#include "Cloneable.h"
#include "Renderable.h"
#include <string>

class RenderEngine;

///
/// \brief Base class for shapes
///
/// \authors Jeremy Montanes
/// \authors Yan Sanquer
///
/// A Shape supports the Transformable operations, can measure its
///  distance to a Vector, is Cloneable and is Renderable on a
///  RenderEngine
///
/// A Shape has an area and a perimeter, and is able to tell whether
///  it contains a point or not
///
class Shape : public Transformable, public Metric<Vector, double>, public Cloneable<Shape>, public Renderable<RenderEngine>
{
        ///
        /// \brief Stream insertion operator
        ///
        /// Write the Shape in Shape Expression Language to the output
        ///  stream
        ///
        friend std::ostream &operator<<(std::ostream &, Shape const &);

    public:
        ///
        /// \brief Destructor
        ///
        virtual ~Shape() = 0;

        ///
        /// \brief Compute the shape's area
        ///
        virtual double area          ()               const = 0;

        ///
        /// \brief Compute the shape's perimeter
        ///
        virtual double perimeter     ()               const = 0;

        ///
        /// \brief Tell whether a point is contained in this
        ///
        virtual bool   contains_point(Vector const &) const = 0;

        virtual Shape &homothetic(Vector const&, double)        override = 0;
        virtual Shape &translate (Vector const&)                override = 0;
        virtual Shape &rotate    (Vector const&, double)        override = 0;
        virtual Shape &symmetry  (Vector const&)                override = 0;
        virtual Shape &symmetry  (Vector const&, Vector const&) override = 0;

    protected:
        ///
        /// \brief Insert a Shape in output stream as Shape Expression
        ///  Language
        ///
        virtual void print_as_expression(std::ostream &out) const = 0;
};

inline Shape::~Shape()
{}

inline std::ostream &operator<<(std::ostream &out, Shape const &shape)
{
    shape.print_as_expression(out);
    return out;
}

#endif
