#ifndef _IMAGE_H
#define _IMAGE_H

#include "Shape.h"
#include "Color.h"
#include "InvalidShapeConstructionException.h"
#include <list>
#include <memory>

class Vector;

///
/// \brief Shape implementation for images
///
/// \authors Yann Sanquer
///
/// Images are a set of colored shapes, within a frame
///
class Image: public Shape
{
    private:
        struct ColoredShape
        {
            Color color;
            _CloneUPtr shape;
            inline ColoredShape(Color c, _CloneUPtr &&p): color(c), shape(std::move(p)) {}
            inline ColoredShape(ColoredShape const &cs): color(cs.color), shape(nullptr)
            {
                if(cs.shape)
                {
                    shape = cs.shape->clone();
                }
            }
        };
        std::list< ColoredShape > _shapes;
        double _xmin, _xmax, _ymin, _ymax;

        Image(double xmin, double xmax, double ymin, double ymax);

    public:
        ///
        /// \brief Create a new Image
        ///
        /// Requires that xmin < xmax and ymin < ymax;
        ///  An InvalidShapeConstructionException is thrown otherwise
        ///
        static Image create(double xmin, double xmax, double ymin, double ymax) throw(InvalidShapeConstructionException);

        ///
        /// \brief Destructor
        ///
        virtual ~Image();

        ///
        /// \brief Insert a new Shape with a Color
        ///
        void insert(Shape const &, Color const &);

        virtual _CloneUPtr clone() const override;

        virtual double area() const override;
        virtual double perimeter() const override;
        virtual bool contains_point(Vector const &) const override;
        virtual double distance(Vector const &) const override;
        virtual void render(RenderEngine &) const override;
        virtual Image &homothetic(Vector const &, double) override;
        virtual Image &translate(Vector const &) override;
        virtual Image &rotate(Vector const &, double) override;
        virtual Image &symmetry(Vector const &) override;
        virtual Image &symmetry(Vector const &, Vector const &) override;

    protected:
        virtual void print_as_expression(std::ostream &) const override;
};

#endif
