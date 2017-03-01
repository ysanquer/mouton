#include "Circle.h"
#include "Vector.h"
#include "RenderEngine.h"

#include <cassert>
#include <cmath>

using namespace std;

Circle::Circle(Vector const &C, double radius): _C(C), _radius(radius)
{
    assert(radius > 0);
}

Circle::~Circle()
{
}

unique_ptr<Shape> Circle::clone() const
{
    return unique_ptr<Shape>(new Circle(_C, _radius));
}

double Circle::area() const
{
    return M_PI * _radius * _radius;
}

double Circle::perimeter() const
{
    return 2 * M_PI * _radius;
}

double Circle::distance(Vector const &O) const
{
    return fabs(O.distance(_C) - _radius);
}

void Circle::render(RenderEngine &engine) const
{
    engine.circle(_C.x, _C.y, _radius);
}

bool Circle::contains_point(Vector const &P) const
{
    return 0 == distance(P);
}

Circle &Circle::homothetic(Vector const &O, double ratio)
{
    _C.homothetic(O, ratio);
    _radius = fabs(_radius * ratio);

    return *this;
}

Circle &Circle::translate(Vector const &D)
{
    _C.translate(D);
    return *this;
}

Circle &Circle::rotate(Vector const &C, double angle_rad)
{
    _C.rotate(C, angle_rad);
    return *this;
}

Circle &Circle::symmetry(Vector const &O)
{
    return homothetic(O, -1);
}

Circle &Circle::symmetry(Vector const &dA, Vector const &dB)
{
    _C.symmetry(dA,dB);
    return *this;
}

void Circle::print_as_expression(ostream &out) const
{
    out << "circle " << _C << " " << _radius;
}

