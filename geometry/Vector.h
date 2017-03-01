#ifndef _VECTOR_H
#define _VECTOR_H

#include "Transformable.h"
#include "Metric.h"

#include <iostream>

///
/// \brief Defines a two-dimensional vector
///
/// A Vector supports Transformable operations
///
/// A Vector can evaluate its distance to another Vector in double
///
struct Vector : public Transformable, public Metric<Vector, double>
{
    ///
    /// \brief Abscissa
    ///
    double x;

    ///
    /// \brief Ordinate
    ///
    double y;

    ///
    /// \brief Abscissa and ordinate constructor
    ///
    Vector(double x, double y);

    ///
    /// \brief Destructor
    ///
    virtual ~Vector();

    virtual double distance(Vector const &) const override;

    virtual Vector &homothetic(Vector const &, double) override;
    virtual Vector &translate(Vector const &) override;
    virtual Vector &rotate(Vector const &, double) override;
    virtual Vector &symmetry(Vector const &) override;
    virtual Vector &symmetry(Vector const &, Vector const &) override;

    ///
    /// \brief Compute scalar product
    ///
    double scal(Vector const &) const;

    ///
    /// \brief Compute subtraction
    ///
    Vector operator-(Vector const &) const;

    ///
    /// \brief Compute addition
    ///
    Vector operator+(Vector const &) const;
};

///
/// \brief Compute ratio product
///
extern Vector operator*(double ratio, Vector const &v);

///
/// \brief Stream insertion operator
///
extern std::ostream &operator<<(std::ostream &, Vector const&);

#endif
