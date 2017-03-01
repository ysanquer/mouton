#include "AbstractDefinedLine.h"

AbstractDefinedLine::AbstractDefinedLine(Vector const &origin, Vector const &direction)
: _origin(origin), _direction(direction)
{
}

AbstractDefinedLine::~AbstractDefinedLine()
{
}

AbstractDefinedLine &AbstractDefinedLine::homothetic(Vector const &O, double ratio)
{
    _origin.homothetic(O, ratio);
    _direction.homothetic(O, ratio);
    return *this;
}

AbstractDefinedLine &AbstractDefinedLine::translate(Vector const &O)
{
    _origin.translate(O);
    return *this;
}

AbstractDefinedLine &AbstractDefinedLine::rotate(Vector const &O, double ratio)
{
    _origin.rotate(O, ratio);
    _direction.rotate(O, ratio);
    return *this;
}

AbstractDefinedLine &AbstractDefinedLine::symmetry(Vector const &O)
{
    _origin.symmetry(O);
    _direction.symmetry(O);
    return *this;
}

AbstractDefinedLine &AbstractDefinedLine::symmetry(Vector const &dA, Vector const &dB)
{
    _origin.symmetry(dA, dB);
    _direction.symmetry(dA, dB);
    return *this;
}

