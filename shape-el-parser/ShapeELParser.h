#ifndef _SHAPE_EL_PARSER
#define _SHAPE_EL_PARSER

#include <iostream>
#include <memory>

#include "ShapeException.h"

class Shape;

///
/// \brief A parser for the Shape Expression Language
///
/// \authors Yann Sanquer
/// \authors Gabriel Bruit
/// \authors Jérémy Montanes
///
/// The ShapeELParser can decode a well-formed Shape Expression into
///  a corresponding Shape implementation
///
/// The Shape Expression Language can express every shape to be used
///  in the Mouton application
///
class ShapeELParser
{
    ///
    /// \brief Stream extraction operator
    ///
    friend std::istream &operator>>(std::istream &, ShapeELParser &) throw(ShapeException, std::ios_base::failure);

    private:
        struct impl;
        std::unique_ptr<impl> _pImpl;        

    public:
        ///
        /// \brief Unique Pointer to Shape
        ///
        typedef std::unique_ptr<Shape> ShapeUPtr;

        ///
        /// \brief Default Constructor
        ///
        ShapeELParser();

        ///
        /// \brief Destructor
        ///
        ~ShapeELParser();
        
        ///
        /// \brief Obtain last parsed shape and reset
        ///
        /// Returned pointer will be empty if last parsing failed
        ///  for any reason or last parsed shape was already obtained
        ///
        ShapeUPtr parsed_shape();

    private:
        void parse_shape(std::istream &) throw(ShapeException, std::ios_base::failure);
};

///
/// Parse a Shape from the given character input stream, expressed with
///  the Shape Expression Language
///
extern std::istream &operator>>(std::istream &, ShapeELParser &) throw(ShapeException, std::ios_base::failure);

#endif
