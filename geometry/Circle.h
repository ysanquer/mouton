#ifndef _CIRCLE_H
#define _CIRCLE_H

#include "Shape.h"
#include "Vector.h"
#include <memory>

///
/// \brief Shape implementation for circles
///
/// Circles are defined with a center point and a scalar radius
///
class Circle: public Shape
{
    private:
        Vector _C;
        double _radius;

    public:
        ///
        /// \brief Center and radius constructor
        ///
        Circle(Vector const &C, double radius);

        ///
        /// \brief Destructor
        ///
        virtual ~Circle();

        virtual std::unique_ptr<Shape> clone() const override;

        virtual double area() const override;
        virtual double perimeter() const override;
        virtual bool contains_point(Vector const &) const override;
        virtual double distance(Vector const &) const override;
        virtual void render(RenderEngine &) const override;
        virtual Circle &homothetic(Vector const &, double) override;
        virtual Circle &translate(Vector const &) override;
        virtual Circle &rotate(Vector const &, double) override;
        virtual Circle &symmetry(Vector const &) override;
        virtual Circle &symmetry(Vector const &, Vector const &) override;

    protected:
        virtual void print_as_expression(std::ostream &) const override;
};

#endif
