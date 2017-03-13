#ifndef _ELLIPSE_H
#define _ELLIPSE_H

#include "Shape.h"
#include "Vector.h"
#include <memory>

///
/// \brief Shape implementation for ellipses
///
/// \authors Yann Sanquer
///
/// Ellipses are defined with two focal points F1 and F2, and the
///  short-axis radius
///
class Ellipse: public Shape
{
    private:
        Ellipse(Vector const &, Vector const &, double, double);
        Vector _F1;
        Vector _F2;
        double _short_radius;
        double _long_radius;

    public:
        ///
        /// \brief Focal points and short radius constructor
        ///
        Ellipse(Vector const &F1, Vector const &F2, double short_radius);

        ///
        /// \brief Destructor
        ///
        virtual ~Ellipse();

        virtual _CloneUPtr clone() const override;

        virtual double area() const override;
        virtual double perimeter() const override;
        virtual bool contains_point(Vector const &) const override;
        virtual double distance(Vector const &) const override;
        virtual void render(RenderEngine &) const override;
        virtual Ellipse &homothetic(Vector const &, double) override;
        virtual Ellipse &translate(Vector const &) override;
        virtual Ellipse &rotate(Vector const &, double) override;
        virtual Ellipse &symmetry(Vector const &) override;
        virtual Ellipse &symmetry(Vector const &, Vector const &) override;

    protected:
        virtual void print_as_expression(std::ostream &) const override;
};

#endif
