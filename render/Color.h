#ifndef _COLOR_H
#define _COLOR_H

#include <cstdint>

///
/// \brief Object that groups RGB 3x8-bits components together
///
/// \authors Jeremy Montanes
///
struct Color
{
    ///
    /// \brief Red
    ///
    uint8_t r;

    ///
    /// \brief Green
    ///
    uint8_t g;

    ///
    /// \brief Blue
    ///
    uint8_t b;   
};

#endif
