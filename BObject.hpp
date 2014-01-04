#ifndef BOBJECT_HPP_INCLUDED
#define BOBJECT_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Imgs.hpp"
#include "AniSprite.hpp"

class BObject: public AniSprite
{
    public:
        Imgs::obj_types type;       // typ obiektu
        bool wall;
        bool show;
        BObject(Imgs *imgs, sf::Vector2f *wsp);
        ~BObject();
};

#endif // BOBJECT_HPP_INCLUDED
