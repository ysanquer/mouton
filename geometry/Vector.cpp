#include "Vector.h"

#include <cmath>

using namespace std;

Vector::Vector(double xx, double yy): x(xx),y(yy)
{
}

Vector::~Vector()
{
}

double Vector::distance(Vector const &O) const
{
    double xd = O.x - x;
    double yd = O.y - y;

    return sqrt(xd*xd+yd*yd);
}

Vector &Vector::homothetic(Vector const &O, double ratio)
{
    x = ((x - O.x) * ratio) + O.x;
    y = ((y - O.y) * ratio) + O.y;
    return *this;
}

Vector &Vector::translate(Vector const &delta)
{
    x += delta.x;
    y += delta.y;
    return *this;
}

Vector &Vector::rotate(Vector const &O, double theta_rad)
{
    x -= O.x;
    y -= O.y;

    double mx = x;
    double my = y;

    x = mx * cos(theta_rad) - my * sin(theta_rad);
    y = mx * sin(theta_rad) + my * cos(theta_rad);

    return *this;
}

Vector &Vector::symmetry(Vector const &O)
{
    return homothetic(O, -1);
}

Vector &Vector::symmetry(Vector const &dA, Vector const &dB)
{
    // droite AB d'equation ax+by = c

    double a = dB.y - dA.y;    

    double b = dA.x - dB.x;

    double c = b * dA.y + a * dA.x;

    double n = a*a+b*b;
    double s = a*x + b*y;

    // TODO garde contre ZDIV (n = 0)
    double xp = x + 2 * a * (c - s) / n;
    double yp = y + 2 * b * (c - s) / n;

    x = xp;
    y = yp;

    return *this;
}

double Vector::scal(Vector const &v) const
{
    return x * v.x + y * v.y;
}

Vector Vector::operator-(Vector const &rhs) const
{
    double rx = rhs.x - x;
    double ry = rhs.y - y;

    return Vector { rx, ry };
}

Vector Vector::operator+(Vector const &rhs) const
{
    double rx = rhs.x + x;
    double ry = rhs.y + y;

    return Vector { rx, ry };
}

Vector operator*(double ratio, Vector const &v)
{
    return Vector(v.x * ratio, v.y * ratio);
}

ostream &operator<<(ostream &out, Vector const &vec)
{
    return out << "vec " << vec.x << " " << vec.y;
}

