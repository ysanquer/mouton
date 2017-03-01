#ifndef _RENDER_ENGINE_H
#define _RENDER_ENGINE_H

#include <cstdint>
#include "RenderEngineException.h"

///
/// \brief A rendering engine interface for the Mouton application
///
/// Can draw lines, polygons, circles and ellipses, as well as
///  change the rendering color and set drawing frame
///
class RenderEngine
{
    public:
        ///
        /// \brief Destructor
        ///
        virtual ~RenderEngine() = 0;

        ///
        /// \brief Set rendering color, as RGB 3x8-bits
        ///  representation
        ///
        /// Use this to change the color of the next drawn shapes
        ///
        virtual RenderEngine &set_color(uint8_t r, uint8_t g, uint8_t b) throw(RenderEngineException) = 0;

        ///
        /// \brief Set rendering frame
        ///
        /// The rendering frame defines a window that limits drawing
        ///  actions
        ///
        virtual RenderEngine &set_frame(double xmin, double xmax, double ymin, double ymax) throw(RenderEngineException) = 0;

        ///
        /// \brief Render a line given points A(xa, ya) and
        ///  B(xb, yb)
        ///
        virtual RenderEngine &line(double xa, double ya, double xb, double yb) throw(RenderEngineException) = 0;

        ///
        /// \brief Render a polygon given points
        ///  Ai(xbegin[i], ybegin[i]) with 0 <= i < xend - xbegin
        ///
        virtual RenderEngine &polygon(double *xbegin, double *xend, double *ybegin, double *yend) throw(RenderEngineException) = 0;

        ///
        /// \brief Render a circle given point C(xc, yc) and
        ///  radius r
        ///
        virtual RenderEngine &circle(double xc, double yc, double r) throw(RenderEngineException) = 0;

        ///
        /// \brief Render an ellipse given points Fa(xfa, yfa)
        ///  and Fb(xfb, yfb) and short radius sr
        ///
        virtual RenderEngine &ellipse(double xfa, double yfa, double xfb, double yfb, double sr) throw(RenderEngineException) = 0;
};

inline RenderEngine::~RenderEngine()
{}

#endif
