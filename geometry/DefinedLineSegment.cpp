#include "DefinedLineSegment.h"
#include "DefinedLine.h"
#include "Vector.h"
#include "RenderEngine.h"

#include <algorithm>

using namespace std;

DefinedLineSegment::DefinedLineSegment(Vector const &origin, Vector const &direction): AbstractDefinedLine(origin, direction)
{
}

DefinedLineSegment::~DefinedLineSegment()
{
}

unique_ptr<Shape> DefinedLineSegment::clone() const
{
    return unique_ptr<Shape>(new DefinedLineSegment(_origin, _direction));
}

double DefinedLineSegment::perimeter() const
{
    return 2 * _origin.distance(_direction + _origin);
}

double DefinedLineSegment::distance(Vector const &O) const
{
    Vector H = DefinedLine(_origin, _direction).project(O);
    return min({O.distance(H), O.distance(_origin), O.distance(_direction + _origin)});
}

bool DefinedLineSegment::contains_point(Vector const &P) const
{
    return 0 == distance(P);
}

void DefinedLineSegment::render(RenderEngine &engine) const
{
    double xa(_origin.x);
    double ya(_origin.y);
    Vector B(_direction + _origin);
    double xb(B.x);
    double yb(B.y);
    engine.line(xa, ya, xb, yb);
}

DefinedLineSegment &DefinedLineSegment::operator=(DefinedLineSegment &&rhs)
{
    _origin = rhs._origin;
    _direction = rhs._direction;
    return *this;
}

void DefinedLineSegment::print_as_expression(ostream &out) const
{
    out << "line " << _origin << " " << _direction;
}

