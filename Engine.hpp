#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "Object.hpp"
#include "Bullet.hpp"

class Engine
{
    public:
        Object::parameters *params;     // trzymaj wskaznik do parametrow glownego bohatera zeby pamietac pomiedzy poziomami
        unsigned int level;
        Engine(int sx, int sy);
        ~Engine();
};

#endif // ENGINE_HPP_INCLUDED
