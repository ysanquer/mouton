#include "VectorRefSegment.h"
#include "Vector.h"
#include "DefinedLine.h"
#include "RenderEngine.h"

#include <algorithm>

using namespace std;

VectorRefSegment::VectorRefSegment(Vector &end1, Vector &end2): _end1(end1), _end2(end2)
{
}

VectorRefSegment::~VectorRefSegment()
{
}

unique_ptr<Shape> VectorRefSegment::clone() const
{
    return unique_ptr<Shape>(new VectorRefSegment(_end1, _end2));
}

double VectorRefSegment::perimeter() const
{
    return 2 * _end1.distance(_end2);
}

double VectorRefSegment::distance(Vector const &O) const
{
    // obtain line from current segment
    // compute orthogonal projection of O on line
    // if projected is in this
    // then compute distance between projected and O
    // else compute distance between _end1 and O
    // and compute distance between _end2 and 0
    // and return lesser of the two
    DefinedLine support(_end1, _end2-_end1);
    Vector H = support.project(O);
    double H1 = _end1.distance(H);
    double H2 = _end2.distance(H);
    double length = _end1.distance(_end2);

    if(length < H1 + H2)
    {
        // out of segment
        double O1 = _end1.distance(O);
        double O2 = _end2.distance(O);
        return min(O1,O2);
    }
    else
    {
        // within segment
        return H.distance(O);
    }
}

bool VectorRefSegment::contains_point(Vector const &P) const
{
    return 0 == distance(P);
}

void VectorRefSegment::render(RenderEngine &engine) const
{
    engine.line(_end1.x, _end1.y, _end2.x, _end2.y);
}

VectorRefSegment &VectorRefSegment::homothetic(Vector const &O, double ratio)
{
    _end1.homothetic(O, ratio);
    _end2.homothetic(O, ratio);
    return *this;
}

VectorRefSegment &VectorRefSegment::translate(Vector const &D)
{
    _end1.translate(D);
    _end2.translate(D);
    return *this;
}

VectorRefSegment &VectorRefSegment::rotate(Vector const &O, double angle_rad)
{
    _end1.rotate(O,angle_rad);
    _end2.rotate(O,angle_rad);
    return *this;
}

VectorRefSegment &VectorRefSegment::symmetry(Vector const &O)
{
    _end1.symmetry(O);
    _end2.symmetry(O);
    return *this;
}

VectorRefSegment &VectorRefSegment::symmetry(Vector const &dA, Vector const &dB)
{
    _end1.symmetry(dA,dB);
    _end2.symmetry(dA,dB);
    return *this;
}

void VectorRefSegment::print_as_expression(ostream &out) const
{
    out << "segment " << _end1 << " " << (_end2 - _end1);
}

