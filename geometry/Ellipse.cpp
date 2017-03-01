#include "Ellipse.h"
#include "Vector.h"
#include "RenderEngine.h"

#include <cassert>
#include <cmath>

using namespace std;

Ellipse::Ellipse(Vector const &F1, Vector const &F2, double short_radius, double long_radius): _F1(F1), _F2(F2), _short_radius(short_radius), _long_radius(long_radius)
{
}

Ellipse::Ellipse(Vector const &F1, Vector const &F2, double short_radius): _F1(F1), _F2(F2), _short_radius(short_radius), _long_radius(0)
{
    assert(short_radius > 0);

    double srsq = _short_radius * _short_radius;
    double half_ffd = _F1.distance(_F2)/2;
    double half_ffdsq = half_ffd * half_ffd;
    _long_radius = sqrt(half_ffdsq + srsq);
}

Ellipse::~Ellipse()
{
}

unique_ptr<Shape> Ellipse::clone() const
{
    return unique_ptr<Shape>(new Ellipse(_F1, _F2, _short_radius, _long_radius));
}

double Ellipse::area() const
{
    return M_PI * _short_radius * _long_radius;
}

double Ellipse::perimeter() const
{
    double radius_sum = (_long_radius + _short_radius);
    double result = M_PI * radius_sum;
    double radius_diff = (_long_radius - _short_radius);
    double radius_diff_sq = radius_diff * radius_diff;
    double radius_sum_sq = radius_sum * radius_sum;
    double h = radius_diff_sq / radius_sum_sq;
    double half_sqrt_pi = sqrt(M_PI)/2;

    double dl_acc_prev = -1; // accumulator for limit development
    double dl_acc = 0; // accumulator for limit development
    double hn_acc = 1; // accumulator for h to the nth power
    int nfac_acc = 1; // accumulator for n!

    // 100-step-max limit development
    for(int n = 0; (n < 100) && (dl_acc_prev != dl_acc); n++)
    {
        dl_acc_prev = dl_acc;
        double u = (half_sqrt_pi) / (nfac_acc*tgamma(1.5 - n));
        dl_acc += hn_acc * u * u;
        hn_acc *= h;
        nfac_acc *= n+1;
    }

    result *= dl_acc;
    return result;
}

double Ellipse::distance(Vector const &O) const
{
    double u = O.distance(_F1);
    double v = O.distance(_F2);
    double w = (u+v)/2;
    double x = w-_short_radius;
    double y = w-_long_radius;
    return (x+y)/2; // and that's why I didn't go math
}

void Ellipse::render(RenderEngine &engine) const
{
    engine.ellipse(_F1.x, _F1.y, _F2.x, _F2.y, _short_radius);
}

bool Ellipse::contains_point(Vector const &P) const
{
    return 0 == distance(P);
}

Ellipse &Ellipse::homothetic(Vector const &O, double ratio)
{
    _F1.homothetic(O, ratio);
    _F2.homothetic(O, ratio);
    _short_radius = fabs(_short_radius * ratio);
    _long_radius = fabs(_long_radius * ratio);

    return *this;
}

Ellipse &Ellipse::translate(Vector const &D)
{
    _F1.translate(D);
    _F2.translate(D);
    return *this;
}

Ellipse &Ellipse::rotate(Vector const &C, double angle_rad)
{
    _F1.rotate(C, angle_rad);
    _F2.rotate(C, angle_rad);
    return *this;
}

Ellipse &Ellipse::symmetry(Vector const &O)
{
    return homothetic(O, -1);
}

Ellipse &Ellipse::symmetry(Vector const &dA, Vector const &dB)
{
    _F1.symmetry(dA,dB);
    _F2.symmetry(dA,dB);
    return *this;
}

void Ellipse::print_as_expression(ostream &out) const
{
    out << "ellipse " << _F1 << " " << _F2 << " " << _short_radius;
}


