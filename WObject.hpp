#ifndef WOBJECT_HPP_INCLUDED
#define WOBJECT_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Imgs.hpp"
#include "AniSprite.hpp"

class WObject: public AniSprite
{
    public:
        Imgs::obj_types type;       // typ obiektu
        bool inside;
        bool show;
        WObject(Imgs *imgs, sf::Vector2f *wsp = NULL);
        ~WObject();
};

#endif // WOBJECT_HPP_INCLUDED
