#ifndef _TRANSFORMABLE_H
#define _TRANSFORMABLE_H

class Vector;

///
/// \brief Defines an object that supports some geometrical transformations
///
/// \authors Yann Sanquer
///
class Transformable
{
    public:
        ///
        /// \brief Destructor
        ///
        virtual ~Transformable() = 0;

        ///
        /// \brief Homothetic transformation
        ///
        virtual Transformable &homothetic(Vector const&, double) = 0;

        ///
        /// \brief Translation transformation
        ///
        virtual Transformable &translate(Vector const&) = 0;

        ///
        /// \brief Rotation transformation
        ///
        virtual Transformable &rotate(Vector const&, double) = 0;

        ///
        /// \brief Central symmetry transformation
        ///
        virtual Transformable &symmetry(Vector const&) = 0;

        ///
        /// \brief Axial symmetry transformation
        ///
        virtual Transformable &symmetry(Vector const&, Vector const&) = 0;
};

inline Transformable::~Transformable()
{}

#endif
