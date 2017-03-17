#ifndef _POLYGON_H
#define _POLYGON_H

#include "Shape.h"
#include "VectorRefSegment.h"
#include "Vector.h"
#include "InvalidShapeConstructionException.h"

#include <vector>
#include <list>
#include <memory>

///
/// \brief Shape implementation for polygons
///
/// \authors Gabriel Bruit
/// \authors Yann Sanquer
///
/// Polygons are defined as a collection of vertices
///
class Polygon: public Shape
{
    public:
        ///
        /// \brief Vertices are Vectors
        ///
        typedef std::vector<Vector> Vertices;

    private:
        typedef std::vector<VectorRefSegment> _Edges;
        typedef std::list<Polygon> _Triangles;

        Polygon(Vertices const &, _Edges const &);
        Polygon(Vertices const &);

        Vertices _vertices;
        _Edges _edges;

    public:
        ///
        /// \brief Create a new Polygon from iterators to Vector
        ///
        /// Requires that end - begin >= 3;
        ///  An InvalidShapeConstructionException is thrown otherwise
        ///
        template<typename VerticesIterator>
        static inline
        Polygon create(VerticesIterator begin, VerticesIterator end)
        throw(InvalidShapeConstructionException)
        {
            size_t vertx_cnt = 0;
            Vertices init_vertx;
            for(auto it = begin; it != end; it++)
            {
                init_vertx.emplace_back(*it);
                vertx_cnt++;
            }
            if(vertx_cnt < 3)
                throw InvalidShapeConstructionException("Polygon with less than 3 vertices");
            return Polygon(init_vertx);
        }

        ///
        /// \brief Destructor
        ///
        virtual ~Polygon();

        virtual _CloneUPtr clone() const override;

        virtual double area() const override;
        virtual double perimeter() const override;
        virtual bool contains_point(Vector const &) const override;
        virtual double distance(Vector const &) const override;
        virtual void render(RenderEngine &) const override;
        virtual Polygon &homothetic(Vector const&, double) override;
        virtual Polygon &translate(Vector const&) override;
        virtual Polygon &rotate(Vector const&, double) override;
        virtual Polygon &symmetry(Vector const &) override;
        virtual Polygon &symmetry(Vector const &, Vector const &) override;

    private:
        _Triangles to_triangles() const;

    protected:
        virtual void print_as_expression(std::ostream &) const override;
};

#endif
