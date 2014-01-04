#ifndef IMGS_HPP_INCLUDED
#define IMGS_HPP_INCLUDED
#define DIR_RES "res/"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

const string img_object_names[] = {"character_e1.png","character_e2.png"};
const string img_wobject_names[] = {"house1.png","house2.png","house3.png"};
const string img_fobject_names[] = {"gold1.png","diament1.png","ammo1.png","health1.png"};
const string img_bobject_names[] = {"tree1.png","tree2.png","tree3.png","tree4.png","tree5.png","stone1.png","stone2.png","stone3.png"};
const string img_guns_names[] = {"gun1.png","gun2.png","gun3.png","gun4.png"};
const string img_bg_names[] = {"bg1.png","bg2.png","bg3.png"};
const string img_cursors_names[] = {"cursor1.png"};

class Imgs
{
    private:
        int ReadIt(size_t length, const string *names, vector<sf::Image*> *image);
        void DeleteIt(vector<sf::Image*> *image);
    public:
        enum obj_types {OBJ, WOBJ, BOBJ, FOBJ};
        vector<sf::Image*> *img_object;
        vector<sf::Image*> *img_wobject;
        vector<sf::Image*> *img_fobject;
        vector<sf::Image*> *img_bobject;
        vector<sf::Image*> *img_guns;
        vector<sf::Image*> *img_bg;
        vector<sf::Image*> *img_cursors;
        enum ImgName {  IMG_MAIN1 = 0,
                        IMG_MAIN2 = 1,
                        IMG_HOUSE1 = 0,
                        IMG_HOUSE2 = 1,
                        IMG_HOUSE3 = 2,
                        IMG_GOLD1 = 0,
                        IMG_DIA1 = 1,
                        IMG_AMMO1 = 2,
                        IMG_HEALTH1 = 3,
                        IMG_TREE1 = 0,
                        IMG_TREE2 = 1,
                        IMG_TREE3 = 2,
                        IMG_TREE4 = 3,
                        IMG_TREE5 = 4,
                        IMG_STONE1 = 5,
                        IMG_STONE2 = 6,
                        IMG_STONE3 = 7,
                        IMG_GUN1 = 0,
                        IMG_GUN2 = 1,
                        IMG_GUN3 = 2,
                        IMG_GUN4 = 3,
                        IMG_BG1 = 0,
                        IMG_BG2 = 1,
                        IMG_BG3 = 2,
                        IMG_CURSOR1 = 0,
                        IMG_RANDOM = 999
                        };
        enum ImgGroup {IMG_OBJECT, IMG_WOBJECT, IMG_FOBJECT, IMG_BOBJECT, IMG_GUN, IMG_BG, IMG_CURSORS};  // should match to below "img_groups_c" variable index
        Imgs();
        ~Imgs();
        sf::Image* Get(enum ImgGroup g, enum ImgName n = IMG_RANDOM);
        size_t Get2(enum ImgGroup g);
};

#endif // IMGS_HPP_INCLUDED
