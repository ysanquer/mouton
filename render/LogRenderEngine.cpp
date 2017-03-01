#include "LogRenderEngine.h"
#include "InvalidRenderFrameException.h"
#include "InvalidRenderDataException.h"

#include <limits>

using namespace std;

LogRenderEngine::LogRenderEngine(): _ss(), _cur_color{0,0,0}, _xmin(-numeric_limits<double>::infinity()), _xmax(numeric_limits<double>::infinity()), _ymin(-numeric_limits<double>::infinity()), _ymax(numeric_limits<double>::infinity())
{
}

LogRenderEngine::~LogRenderEngine()
{}

string LogRenderEngine::result() const
{
    return _ss.str();
}

void LogRenderEngine::clear()
{
    _ss.str(string());
}

LogRenderEngine &LogRenderEngine::set_color(uint8_t r, uint8_t g, uint8_t b) throw(RenderEngineException)
{
    _cur_color.r = r;
    _cur_color.g = g;
    _cur_color.b = b;
    return *this;
}

LogRenderEngine &LogRenderEngine::set_frame(double xmin, double xmax, double ymin, double ymax) throw(RenderEngineException)
{
    if(xmax <= xmin || ymax <= ymin)
    {
        throw InvalidRenderFrameException("upper limits must have higher values than lower limits");
    }
    _xmax = xmax;
    _xmin = xmin;
    _ymax = ymax;
    _ymin = ymin;
    return *this;
}

void LogRenderEngine::render_frame()
{
    _ss << "limit (" << _xmin << "," << _ymin << "), (" << _xmax << "," << _ymax << ")";
}

void LogRenderEngine::render_color()
{
    _ss << "color rgb(" << (uint32_t) _cur_color.r << ", " << (uint32_t) _cur_color.g << ", " << (uint32_t) _cur_color.b << ")";
}

LogRenderEngine &LogRenderEngine::line(double xa, double ya, double xb, double yb) throw(RenderEngineException)
{
    _ss << "line A(" << xa << "," << ya << ") B(" << xb << "," << yb << ")";
    _ss << " "; render_color();
    _ss << " "; render_frame();
    _ss << endl;
    return *this;
}

LogRenderEngine &LogRenderEngine::polygon(double *xit, double *xend, double *yit, double *yend) throw(RenderEngineException)
{
    stringstream ss;
    ss << "polygon";
    for(size_t i = 0; xit != xend && yit != yend; i++)
    {
        ss << " P" << i << "(" << *xit << "," << *yit << ")";
        xit++;
        yit++;
    }
    if(xit != xend || yit != yend)
    {
        throw InvalidRenderDataException("will not render polygon as its provided x-values and y-values have different arities");
    }
    _ss << ss.str();
    _ss << " "; render_color();
    _ss << " "; render_frame();
    _ss << endl;
    return *this;
}

LogRenderEngine &LogRenderEngine::circle(double xc, double yc, double r) throw(RenderEngineException)
{
    _ss << "circle C(" << xc << "," << yc << ") radius " << r;
    _ss << " "; render_color();
    _ss << " "; render_frame();
    _ss << endl;
    return *this;
}

LogRenderEngine &LogRenderEngine::ellipse(double xfa, double yfa, double xfb, double yfb, double sr) throw(RenderEngineException)
{
    _ss << "ellipse Fa(" << xfa << "," << yfa << ") Fb(" << xfb << "," << yfb << ") short radius " << sr;
    _ss << " "; render_color();
    _ss << " "; render_frame();
    _ss << endl;
    return *this;
}

