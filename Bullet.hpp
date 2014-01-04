#ifndef BULLET_HPP_INCLUDED
#define BULLET_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Imgs.hpp"
#include "AniSprite.hpp"

class Bullet: public AniSprite
{
    public:
        bool show;
        unsigned int shot_speed;         // szybkosc przemieszczania sie pocisku
        unsigned int shot_pos;           // pozycja pocisku
        vector<sf::Vector2f> vec_shot;   // pomocniczy kontener wskaznikami do vector2f
        Bullet(Imgs *imgs, enum Imgs::ImgName gt = Imgs::IMG_RANDOM);
        ~Bullet();
};

#endif
