#include "ShapeELParser.h"

#include "Vector.h"
#include "Shape.h"
#include "DefinedLine.h"
#include "DefinedLineSegment.h"
#include "Polygon.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Image.h"

#include "functions.h"

#include <string>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

typedef unique_ptr<Shape>  ShapeUPtr   ;
typedef unique_ptr<Vector> VecUPtr     ;
typedef uint8_t            UByteIntg   ;
typedef size_t             CardinalIntg;
typedef double             ScalarIntg  ;

struct ShapeELParser::impl
{
    ShapeUPtr    m_parsed_shape;
    VecUPtr      m_parsed_vec;
    CardinalIntg m_parsed_cardinal;
    UByteIntg    m_parsed_ubyte;
    ScalarIntg   m_parsed_scalar;

    impl();
    ~impl();

    bool parse_shape(istream &) throw(ShapeException, ios_base::failure);

    bool parse_line(istream &) throw(ShapeException, ios_base::failure);
    bool parse_polygon(istream &) throw(ShapeException, ios_base::failure);
    bool parse_circle (istream &) throw(ShapeException, ios_base::failure);
    bool parse_ellipse(istream &) throw(ShapeException, ios_base::failure);
    bool parse_image  (istream &) throw(ShapeException, ios_base::failure);

    bool parse_vec     (istream &) throw(ios_base::failure);
    bool parse_cardinal(istream &) throw(ios_base::failure);
    bool parse_ubyte   (istream &) throw(ios_base::failure);
    bool parse_scalar  (istream &) throw(ios_base::failure);
};



// ShapeELParser definition


// ctor
ShapeELParser::ShapeELParser(): _pImpl(new impl)
{
}

// dtor
ShapeELParser::~ShapeELParser()
{
}

// obtain last parsed shape and reset
ShapeELParser::ShapeUPtr ShapeELParser::parsed_shape()
{
    // take last parsed shape
    ShapeUPtr ret = move(_pImpl->m_parsed_shape);

    // reset
    _pImpl->m_parsed_shape.reset();
    
    // and return
    return move(ret);
}

// parse any shape from a character input
void ShapeELParser::parse_shape(istream &in) throw(ShapeException, ios_base::failure)
{
    string line;
    if(!getline(in, line))
    {
        // failed to read line, stream must already be in bad state
        return;
    }
    str_trim_ws(line);
    istringstream is(line);
    // transfer parsing of the line to implementation and receive result
    bool success = _pImpl->parse_shape(is);

    // set failbit on input stream if extraction failed
    if(!success)
    {
        in.setstate(ios_base::failbit);
    }
}

/*
 * extract a Shape represented with the Shape Expression Language
 *  from an input stream
 */
istream &operator>>(istream &in, ShapeELParser &selp) throw(ShapeException, ios_base::failure)
{
    // delegate to ShapeELParser::parse_shape
    selp.parse_shape(in);
    return in;
}



// ShapeELParser::impl definition


// ctor
ShapeELParser::impl::impl(): m_parsed_shape(), m_parsed_vec(), m_parsed_cardinal(0), m_parsed_ubyte(0), m_parsed_scalar(0)
{
}

// dtor
ShapeELParser::impl::~impl()
{
}

bool ShapeELParser::impl::parse_shape(istream &in) throw(ShapeException, ios_base::failure)
{
    string    shape_type;
    bool      shape_ptr_ok = false;
    ShapeUPtr shape_ptr;

    // read shape type
    in >> shape_type;
    
    // get shape pointer based on shape type
    if(shape_type == "line")
    {
        shape_ptr_ok = parse_line(in);
    }
    else if(shape_type == "polygon")
    {
        shape_ptr_ok = parse_polygon(in);
    }
    else if(shape_type == "circle")
    {
        shape_ptr_ok = parse_circle(in);
    }
    else if(shape_type == "ellipse")
    {
        shape_ptr_ok = parse_ellipse(in);
    }
    else if(shape_type == "image")
    {
        shape_ptr_ok = parse_image(in);
    }
    // else: shape_ptr is nullptr, indicates failure

    return shape_ptr_ok;
}

// parse a line and return it
bool ShapeELParser::impl::parse_line(istream &in) throw(ShapeException, ios_base::failure)
{
    ShapeUPtr line;

    // parse "point" and "direction" vectors
    bool    point_ok = parse_vec(in);
    VecUPtr point    = move(m_parsed_vec);

    bool    direction_ok = parse_vec(in);
    VecUPtr direction    = move(m_parsed_vec);
    
    // create line only if both vectors were extracted
    if(point_ok && direction_ok)
    {
        // create line
        DefinedLineSegment line_impl(*point, *direction);
        // then clone to obtain pointer
        line = line_impl.clone();
    }

    // evaluate success
    bool result = (bool) line;

    // set last parsed shape
    m_parsed_shape = move(line);

    return result;
}

// parse a polygon and return it
bool ShapeELParser::impl::parse_polygon(istream &in) throw(ShapeException, ios_base::failure)
{
    // parse the polygon's vertex count
    bool         polygon_cardinal_ok = parse_cardinal(in);
    CardinalIntg polygon_cardinal    = m_parsed_cardinal;
    
    // on parsing failure, return false
    if(!polygon_cardinal_ok)
    {
        return false;
    }
    
    vector<Vector> vertices;
    while(vertices.size() != polygon_cardinal)
    {
        // parse a point
        bool    vec_ok = parse_vec(in);
        VecUPtr vec    = move(m_parsed_vec);
        // if parsing failed, return false 
        if(!vec_ok)
        {
            return false;
        }
        // add vertex to collection
        vertices.push_back(*vec);
    }
    
    // construct polygon
    Polygon polygon_impl(Polygon::create(vertices.begin(), vertices.end()));
    // and clone to obtain pointer
    m_parsed_shape = polygon_impl.clone();

    return true;
}

// parse a circle and return it
bool ShapeELParser::impl::parse_circle(istream &in) throw(ShapeException, ios_base::failure)
{
    ShapeUPtr circle;

    // parse vector for the circle's center
    bool    C_ok = parse_vec(in);
    VecUPtr C    = move(m_parsed_vec);
    // parse scalar for the circle's radius
    bool       radius_ok = parse_scalar(in);
    ScalarIntg radius    = m_parsed_scalar;

    if(C_ok && radius_ok)
    {
        Circle circle_impl(*C, radius);
        circle = circle_impl.clone();
    }

    bool result = (bool) circle;

    m_parsed_shape = move(circle);

    return result;
}

bool ShapeELParser::impl::parse_ellipse(istream &in) throw(ShapeException, ios_base::failure)
{
    ShapeUPtr ellipse;

    bool    F1_ok = parse_vec(in);
    VecUPtr F1    = move(m_parsed_vec);

    bool    F2_ok = parse_vec(in);
    VecUPtr F2    = move(m_parsed_vec);

    bool       short_radius_ok = parse_scalar(in);
    ScalarIntg short_radius    = m_parsed_scalar;

    if(F1_ok && F2_ok && short_radius_ok)
    {
        Ellipse ellipse_impl(*F1, *F2, short_radius);
        ellipse = ellipse_impl.clone();
    }

    bool result = (bool) ellipse;
    
    m_parsed_shape = move(ellipse);

    return result;
}

bool ShapeELParser::impl::parse_image(istream &in) throw(ShapeException, ios_base::failure)
{
    ShapeUPtr image;

    bool image_xmin_ok = parse_scalar(in);
    ScalarIntg image_xmin = m_parsed_scalar;

    bool image_xmax_ok = parse_scalar(in);
    ScalarIntg image_xmax = m_parsed_scalar;

    bool image_ymin_ok = parse_scalar(in);
    ScalarIntg image_ymin = m_parsed_scalar;

    bool image_ymax_ok = parse_scalar(in);
    ScalarIntg image_ymax = m_parsed_scalar;

    if(!image_xmin_ok || !image_xmax_ok || !image_ymin_ok || !image_ymax_ok)
    {
        return false;
    }

    Image image_impl(Image::create(image_xmin, image_xmax, image_ymin, image_ymax));
    
    bool         image_cardinal_ok = parse_cardinal(in);
    CardinalIntg image_cardinal    = m_parsed_cardinal;

    if(!image_cardinal_ok)
    {
        return false;
    }
    
    for(CardinalIntg c = 0; c < image_cardinal; c++)
    {
        bool      next_color_r_ok = parse_ubyte(in);
        UByteIntg next_color_r    = m_parsed_ubyte;

        bool      next_color_g_ok = parse_ubyte(in);
        UByteIntg next_color_g    = m_parsed_ubyte;

        bool      next_color_b_ok = parse_ubyte(in);
        UByteIntg next_color_b    = m_parsed_ubyte;

        bool next_shape_ok = parse_shape(in);
        ShapeUPtr next_shape = move(m_parsed_shape);

        if(!next_shape_ok || ! next_color_r_ok || !next_color_g_ok || !next_color_b_ok)
        {
            return false;
        }
        else
        {
            image_impl.insert(*next_shape, Color{next_color_r, next_color_g, next_color_b});
        }
    }

    m_parsed_shape = image_impl.clone();

    return true;
}

bool ShapeELParser::impl::parse_vec(istream &in) throw(ios_base::failure)
{
    VecUPtr vec;

    string vec_cmd;
    in >> vec_cmd;

    if(vec_cmd != "vec")
    {
        return false;
    }

    bool       x_ok = parse_scalar(in);
    ScalarIntg x    = m_parsed_scalar;

    bool       y_ok = parse_scalar(in);
    ScalarIntg y    = m_parsed_scalar;

    if(!x_ok || !y_ok)
    {
        return false;
    }

    m_parsed_vec = unique_ptr<Vector>(new Vector(x,y));

    return true;
}

bool ShapeELParser::impl::parse_cardinal(istream &in) throw(ios_base::failure)
{
    return in >> m_parsed_cardinal;
}

bool ShapeELParser::impl::parse_ubyte(istream &in) throw(ios_base::failure)
{
    uint32_t a;
    uint32_t b;
    bool read_success;
    bool cast_ok;

    read_success = in >> a;
    m_parsed_ubyte = a;
    b = m_parsed_ubyte;
    cast_ok = a == b;
    return read_success && cast_ok;
}

bool ShapeELParser::impl::parse_scalar(istream &in) throw(ios_base::failure)
{
    return in >> m_parsed_scalar;
}


