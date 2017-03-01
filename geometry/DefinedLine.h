#ifndef _DEFINED_LINE_H
#define _DEFINED_LINE_H

#include "AbstractDefinedLine.h"
#include <memory>

///
/// \brief Shape implemntation for defined lines
///
/// A defined line is defined with an origin point, and a direction vector
///
class DefinedLine: public AbstractDefinedLine
{
    public:
        ///
        /// \brief Origin and direction constructor
        ///
        DefinedLine(Vector const &origin, Vector const &direction);

        ///
        /// \brief Destructor
        ///
        virtual ~DefinedLine();

        virtual _CloneUPtr clone() const override;

        virtual double perimeter() const override;
        virtual double distance(Vector const &) const override;
        virtual bool contains_point(Vector const &) const override;

        virtual void render(RenderEngine &) const override;

        ///
        /// \brief Project a point on this line
        ///
        Vector project(Vector const &) const;

    protected:
        virtual void print_as_expression(std::ostream &) const override;
};

#endif
