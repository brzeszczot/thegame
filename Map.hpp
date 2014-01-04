#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#define MAX_WOBJECTS_SIZE 200
#define WOBJECT_WALL 10
#define FOBJECT_MAX_SIZE 20
#define MAX_OBJECTS 6
#define MAX_BOBJECT_X_SIZE 51
#define MAX_BOBJECT_Y_SIZE 71
#define MAX_OBJECT_X_SIZE 51
#define MAX_OBJECT_Y_SIZE 71
#define MAX_BORDER_HEIGHT 150
#define PX_TOUCH 5
#define VIEW_SIZE 50

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "WObject.hpp"
#include "BObject.hpp"
#include "FObject.hpp"
#include "Imgs.hpp"
#include "AniSprite.hpp"

class Map
{
    private:
        Imgs *imgs;
    public:
        vector<sf::Shape*> *shapes; // krzywe
        vector<BObject*> *bobjects; // rozne obiekty dekoracyjne
        vector<Object*> *objects;   // postacie (przeciwnicy i inni)
        vector<WObject*> *wobjects; // pomieszczenia
        vector<FObject*> *fobjects; // znajdzki

        bool view_switch;
        int view_dist;
        int view_array_x;
        int view_array_y;
        bool **view_array;

        sf::Sprite bg;
        sf::Vector2f *wsp;
        Map(Imgs *imgs_in, unsigned int sx, unsigned int sy);
        ~Map();
        void PlaceObjects();
        sf::Vector2f CalcVectors(int frame, sf::Vector2f area_max);
        void AddBobject(size_t cc = 1);
        void AddObject();
        void AddWobject();
        void AddFobject(size_t cc = 1);
};

#endif // MAP_HPP_INCLUDED
