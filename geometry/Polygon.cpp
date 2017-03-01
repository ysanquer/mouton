#include "Polygon.h"
#include "DefinedLine.h"
#include "RenderEngine.h"

#include <cassert>
#include <iterator>
#include <deque>
#include <algorithm>
#include <cmath>

using namespace std;

Polygon::Polygon(Vertices const &vertices, _Edges const &edges): _vertices(vertices), _edges(edges)
{
}

Polygon::Polygon(Vertices const &vertices): _vertices(vertices), _edges()
{
    assert(_vertices.size() >= 3);
    // do not throw within ctor (uninitialized object problem)
    // polygons will be created with the Shape Expression Language Parser
    // and through factory methods for shapes

    auto it(_vertices.begin());
    auto it1(_vertices.begin());
    it1++;

    auto end(_vertices.end());    

    for(; it1 != end; it++, it1++)
    {
        Vector &A(*it);
        Vector &B(*it1);
        _edges.emplace_back(A, B);
    }
    _edges.emplace_back(*(_vertices.end() - 1), *(_vertices.begin()));
}

Polygon::~Polygon()
{
}

unique_ptr<Shape> Polygon::clone() const
{
    return unique_ptr<Shape>(new Polygon(_vertices, _edges));
}

double Polygon::area() const
{
    // if has 3 vertices
    // compute triangle area (if not right, split into two)
    // else
    // split into triangles then compute triangles area
    double result = 0;

    if(_vertices.size() == 3)
    {
        // if triangle is not right, split into two right triangles
        // compute orthogonal projection of any vertex onto the
        //  opposite base
        // if projection is not on the segment, use another
        //  vertex and base pair

        // project A onto edge BC as H
        Vector const &A(_vertices[0]);
        Vector const &B(_vertices[1]);
        Vector const &C(_vertices[2]);
        Vector BC(C-B);
        DefinedLine base_line(B, BC);
        Vector H = base_line.project(A);

        // created triangles ABH, AHC
        // find h = || -AH> ||
        Vector AH(H-A);
        double h = sqrt(AH.scal(AH));
        Vector HB(B-H);
        Vector HC(C-H);

        // area of ABH
        double lABH = sqrt(HB.scal(HB));
        double area_ABH = (h * lABH) / 2;

        // area of AHC
        double lAHC = sqrt(HC.scal(HC));
        double area_AHC = (h * lAHC) / 2;

        double lABC = sqrt(BC.scal(BC));

        // if H out of [BC]
        // then BH + HC != BC
        // else BH + HC = BC

        // TODO use float comparison
        if(lABH + lAHC > lABC)
        {
            // H out of [BC]
            // either BH > HC
            // then remove AHC
            // or BH < HC
            // then remove ABH

            if(lABH > lAHC)
            {
                result = area_ABH - area_AHC;
            }
            else
            {
                result = area_AHC - area_ABH;
            }
        }
        else
        {
            result = area_ABH + area_AHC;
        }
    }
    else
    {
        list<Polygon> triangles = to_triangles();
        for(Polygon const &triangle: triangles)
        {
            result += triangle.area();
        }
    }
    return result;
}

double Polygon::perimeter() const
{
    // sum distances between all vectors
    double result(0);
    auto it(_vertices.cbegin());
    auto it1(_vertices.cbegin());
    it1++;
    auto end(_vertices.cend());

    while(it1 != end)
    {
        result += it->distance(*it1);
        it1++, it++;
    }
    result += it->distance(_vertices[0]);
    return result;
}

double Polygon::distance(Vector const &O) const
{
    // compute distance to origin for all segments
    // then return lowest one

    std::vector<double> distances;
    for(VectorRefSegment const &edge: _edges)
    {
        distances.push_back(edge.distance(O));
    }
    return *(min_element(distances.begin(), distances.end()));
}

bool Polygon::contains_point(Vector const &P) const
{
    return 0 == distance(P);
}

void Polygon::render(RenderEngine &engine) const
{
    size_t n_vertices(_vertices.size());
    double x_values[n_vertices];
    double y_values[n_vertices];
    for(size_t i = 0; i < n_vertices; i++)
    {
        x_values[i] = _vertices[i].x;
        y_values[i] = _vertices[i].y;
    }
    engine.polygon(x_values, x_values+n_vertices, y_values, y_values+n_vertices);
}

Polygon &Polygon::homothetic(Vector const &O, double ratio)
{
    // dispatch homothetic transform to all vertices
    for(Vector &vertex: _vertices)
    {
        vertex.homothetic(O, ratio);
    }
    return *this;
}

Polygon &Polygon::translate(Vector const &v)
{
    // dispatch to all vertices
    for(Vector &vertex: _vertices)
    {
        vertex.translate(v);
    }
    return *this;
}

Polygon &Polygon::rotate(Vector const &O, double angle_rad)
{
    // dispatch to all vertices
    for(Vector &vertex: _vertices)
    {
        vertex.rotate(O, angle_rad);
    }
    return *this;
}

Polygon &Polygon::symmetry(Vector const &O)
{
    // dispatch to all vertices
    for(Vector &vertex: _vertices)
    {
        vertex.symmetry(O);
    }
    return *this;
}

Polygon &Polygon::symmetry(Vector const &dA, Vector const &dB)
{
    // dispatch to all vertices
    for(Vector &vertex: _vertices)
    {
        vertex.symmetry(dA,dB);
    }
    return *this;
}

list<Polygon> Polygon::to_triangles() const
{
    list<Polygon> ret;
    if(_vertices.size() == 3)
    {
        ret.push_back(*this);
    }
    else
    {
        // get first two vertices, and last one, as one triangle
        // effectively, second and last vertices have edges with
        //  the first one, so we just add a base
        // second vertex, last vertex and second-last vertex make
        //  another triangle... Just pop one until we have 3 vertices

        // create deque
        deque<Vector> rem_vertices(_vertices.begin(), _vertices.end());
        deque<Vector>::size_type sz(rem_vertices.size());

        // iteration ends when we have three elements remaining
        deque<Vector>::iterator it(rem_vertices.begin());
        deque<Vector>::iterator end(rem_vertices.end() - 3);

        while(it != end)
        {
            Vector &A(*it);
            Vector &B(*(it+1));
            Vector &C(*(end+2));
            initializer_list<Vector> vecs{A,B,C};

            ret.push_back(Polygon(vecs));
            it++;
        }
    }
    return ret;
}

void Polygon::print_as_expression(ostream &out) const
{
    out << "polygon " << _vertices.size();
    for(Vector const &v: _vertices)
    {
        out << " " << v;
    }
}


