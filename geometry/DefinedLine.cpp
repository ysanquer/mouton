#include "DefinedLine.h"
#include "Vector.h"
#include "RenderEngine.h"

#include <limits>
#include <cassert>
#include <iostream>

using namespace std;

DefinedLine::DefinedLine(Vector const &origin, Vector const &direction): AbstractDefinedLine(origin, direction)
{
}

DefinedLine::~DefinedLine()
{
}

unique_ptr<Shape> DefinedLine::clone() const
{
    return unique_ptr<Shape>(new DefinedLine(_origin, _direction));
}

double DefinedLine::perimeter() const
{
    return numeric_limits<double>::infinity();
}

double DefinedLine::distance(Vector const &O) const
{
    Vector H = project(O);
    return O.distance(H);
}

bool DefinedLine::contains_point(Vector const &P) const
{
    return distance(P) == 0;
}

void DefinedLine::render(RenderEngine &) const
{
    // not to be rendered!
    assert(false && "DefinedLine::render called; Do not allow DefinedLine to be used with the Shape EL Parser, so it can not be rendered later on");
}

Vector DefinedLine::project(Vector const &A) const
{
    // _origin, _direction
    // -OA> = A
    // -OB> = _origin
    // -v> = _direction
    // -BA> = -OA> - -OB>
    // -BH> = (-BA> scal -v>) / (-v> scal -v>) * (-v>)
    // -OH> = H
    // -OH> = -OB> + -BH>

    Vector BA(A - _origin);
    Vector BH((BA.scal(_direction) / (_direction.scal(_direction))) * _direction);
    return _origin+BH;
}

void DefinedLine::print_as_expression(std::ostream &) const
{
    // not to be parsed, no point in printing it
}


