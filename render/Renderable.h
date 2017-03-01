#ifndef _RENDERABLE_H
#define _RENDERABLE_H

///
/// \brief Defines an object as being able to be rendered on a
///  given Engine type
///
/// This is made so you can define an object that can be rendered
///  on different engines
///
template<typename Engine>
class Renderable
{
    public:
        ///
        /// \brief Destructor
        ///
        virtual ~Renderable() = 0;

        ///
        /// \brief Render object on given engine
        ///
        virtual void render(Engine &engine) const = 0;
};

template<typename Engine>
inline Renderable<Engine>::~Renderable()
{
}

#endif
