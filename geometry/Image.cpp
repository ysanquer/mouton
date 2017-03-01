#include "Image.h"
#include "Vector.h"
#include "RenderEngine.h"

#include <cassert>
#include <sstream>

using namespace std;

Image::Image(double xmin, double xmax, double ymin, double ymax): _shapes(), _xmin(xmin), _xmax(xmax), _ymin(ymin), _ymax(ymax)
{
    assert((xmin < xmax) && (ymin < ymax));
}

Image Image::create(double xmin, double xmax, double ymin, double ymax) throw(InvalidShapeConstructionException)
{
    if((xmin >= xmax) || (ymin >= ymax))
    {
        throw InvalidShapeConstructionException("bad boundaries for image: min boundary must be < than max boundary");
    }
    return Image(xmin, xmax, ymin, ymax);
}

Image::~Image()
{
}

void Image::insert(Shape const &shape, Color const &color)
{
    _shapes.emplace_back(color, shape.clone());
}

unique_ptr<Shape> Image::clone() const
{
    unique_ptr<Image> clone(new Image(_xmin, _xmax, _ymin, _ymax));
    for(ColoredShape const &cs: _shapes)
    {
        clone->insert(*(cs.shape), cs.color);
    }
    return move(clone);
}

double Image::area() const
{
    double total = 0;
    for(ColoredShape const &cs: _shapes)
    {
        total += cs.shape->area();
    }
    return total;
}

double Image::perimeter() const
{
    return 2 * ((_ymax - _ymin) + (_xmax - _xmin));
}

double Image::distance(Vector const &O) const
{
    double min_distance = 0;
    for(ColoredShape const &cs: _shapes)
    {
        double cur_distance = cs.shape->distance(O);
        if(cur_distance < min_distance)
        {
            min_distance = cur_distance;
        }
    }
    return min_distance;
}

void Image::render(RenderEngine &engine) const
{
    engine.set_frame(_xmin, _xmax, _ymin, _ymax);
    for(ColoredShape const &cs: _shapes)
    {
        engine.set_color(cs.color.r, cs.color.g, cs.color.b);
        cs.shape->render(engine);
        engine.set_frame(_xmin, _xmax, _ymin, _ymax); // reset my frame, because I may just have rendered a sub-image
    }
}

bool Image::contains_point(Vector const &P) const
{
    return 0 == distance(P);
}

Image &Image::homothetic(Vector const &O, double ratio)
{
    for(ColoredShape const &cs: _shapes)
    {
        cs.shape->homothetic(O, ratio);
    }
    return *this;
}

Image &Image::translate(Vector const &D)
{
    for(ColoredShape const &p: _shapes)
    {
        p.shape->translate(D);
    }
    return *this;
}

Image &Image::rotate(Vector const &O, double ratio)
{
    for(ColoredShape const &p: _shapes)
    {
        p.shape->rotate(O, ratio);
    }
    return *this;
}

Image &Image::symmetry(Vector const &O)
{
    for(ColoredShape const &p: _shapes)
    {
        p.shape->symmetry(O);
    }
    return *this;
}

Image &Image::symmetry(Vector const &dA, Vector const &dB)
{
    for(ColoredShape const &p: _shapes)
    {
        p.shape->symmetry(dA,dB);
    }
    return *this;
}

void Image::print_as_expression(ostream &out) const
{
    out << "image " << _xmin << " " << _xmax << " " << _ymin << " " << _ymax << " " << _shapes.size();
    for(ColoredShape const &p: _shapes)
    {
        out << " " << (uint32_t)p.color.r << " " << (uint32_t)p.color.g << " " << (uint32_t)p.color.b << " " << *(p.shape);
    }
}

