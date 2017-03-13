#ifndef _LOG_RENDER_ENGINE_H
#define _LOG_RENDER_ENGINE_H

#include <cstdint>
#include <sstream>

#include "RenderEngine.h"
#include "Color.h"

///
/// \brief A RenderEngine implementation that logs any drawing it
///  makes
///
/// \authors Yann Sanquer
///
class LogRenderEngine: public RenderEngine
{
    private:
        std::stringstream _ss;
        Color _cur_color;
        double _xmin;
        double _xmax;
        double _ymin;
        double _ymax;

    public:
        ///
        /// \brief Default constructor
        ///
        LogRenderEngine();

        ///
        /// \brief Destructor
        ///
        virtual ~LogRenderEngine();

        ///
        /// \brief Obtain log of issued drawing instructions
        ///
        std::string result() const;

        ///
        /// \brief Clear log of issued drawing instructions
        ///
        void clear();

        virtual LogRenderEngine &set_color(uint8_t, uint8_t, uint8_t) throw(RenderEngineException) override;

        virtual LogRenderEngine &set_frame(double, double, double, double) throw(RenderEngineException) override;

        virtual LogRenderEngine &line(double, double, double, double) throw(RenderEngineException) override;

        virtual LogRenderEngine &polygon(double*, double*, double*, double*) throw(RenderEngineException) override;

        virtual LogRenderEngine &circle(double, double, double) throw(RenderEngineException) override;

        virtual LogRenderEngine &ellipse(double, double, double, double, double) throw(RenderEngineException) override;

    private:
        void render_frame();
        void render_color();
};

#endif
