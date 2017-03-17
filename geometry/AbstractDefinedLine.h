#ifndef _ABSTRACT_DEFINED_LINE_H
#define _ABSTRACT_DEFINED_LINE_H

#include "AbstractLine.h"
#include "Vector.h"

///
/// \brief Base class for fixed, independent lines
///
/// \authors Yann Sanquer
/// \authors Jeremy Montanes
///
/// Such lines are defined with two points: An origin and a direction
///
class AbstractDefinedLine : public AbstractLine
{
    protected:
        Vector _origin;
        Vector _direction;

    public:
        ///
        /// \brief Origin and Direction constructor
        ///
        AbstractDefinedLine(Vector const &origin, Vector const &direction);

        ///
        /// \brief Destructor
        ///
        virtual ~AbstractDefinedLine() = 0;

        // perimeter is defined for segments and infinite for lines

        virtual AbstractDefinedLine &homothetic(Vector const&, double) override;
        virtual AbstractDefinedLine &translate(Vector const&) override;
        virtual AbstractDefinedLine &rotate(Vector const&, double) override;
        virtual AbstractDefinedLine &symmetry(Vector const &) override;
        virtual AbstractDefinedLine &symmetry(Vector const &, Vector const &) override;
};

#endif
