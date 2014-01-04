#ifndef FOBJECT_HPP_INCLUDED
#define FOBJECT_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Imgs.hpp"
#include "AniSprite.hpp"
#include "WObject.hpp"

class FObject: public AniSprite
{
    public:
        Imgs::obj_types type;       // typ obiektu
        Imgs::ImgName imgs_name;
        WObject *w_inside;
        bool inside;
        bool show;
        FObject(Imgs *imgs, sf::Vector2f *wsp);
        ~FObject();
};

#endif // FOBJECT_HPP_INCLUDED
