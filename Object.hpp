#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#define MIN_SPEED 50
#define LOW_SPEED 100
#define MEDIUM_SPEED 150
#define HIGH_SPEED 200
#define MAX_SPEED 300

#define MAX_VISIBLE 300
#define MEDIUM_VISIBLE 200
#define MIN_VISIBLE 100
#define ZERO_VISIBLE 10

#define NEAR_SHOT 100
#define MEDIUM_SHOT 200
#define FAR_SHOT 300

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Imgs.hpp"
#include "AniSprite.hpp"
#include "WObject.hpp"
#include "Bullet.hpp"

enum e_stat {READY, MOVING, M_AVOID, DEAD};
class Object: public AniSprite
{
    public:
        enum gun_type {BASIC, STONE, FIREBALL, LASER};
        struct gun
        {
            gun_type gun;
            vector<Bullet*> shots;
            int ammo;
            int damage;
            sf::Clock clock;        // zegar do odmierzania czasu miedzy strzalami
            float shot_clock;
            float shot_time_allowed;
        };
        struct parameters
        {
            int life;               // zycie
            int gold;               // zloto :)
            int visible_dist;       // na jaka odleglosc objekt potrafi widziec
            int move_speed;         // szybkosc poruszania sie
            int shot_dist;          // jak daleko strzela
            unsigned int active_gun;    // aktywna bron
            vector<gun*> guns;  // plecak z bronia
        };
        Imgs::obj_types type;       // typ obiektu
        parameters *params;         // parametry bohatera
        e_stat status;              // MOVING status
        sf::Vector2f last_position; // przy ustawianiu kazdej nowej wspolzednej najpierw zapisz ostatnia pozycje
        sf::Vector2f last_contact;  // wsp glownego bohatera w momencie ostatniego kontaktu wzrokowego
        unsigned int lldirection;
        size_t last_direction;      // LAST 1-8 zaczynajac od gory jak wskazowki zegara
        size_t move_direction;      // 1-8 zaczynajac od gory jak wskazowki zegara
        WObject *w_inside;          // jesli jestesmy w srodku wobject - wez wskaznik do tego wobject
        bool show;                  // widac obiekt na mapie lub nie widac :)
        Object(Imgs *imgs, sf::Vector2f *wsp = NULL, enum Imgs::ImgName en = Imgs::IMG_MAIN1);
        void MoveByDirection(size_t direction, float speed_unit);
        size_t MoveDirectionReverse(size_t direction);
        void AddGun(enum gun_type gt = BASIC);
        ~Object();
};

#endif // OBJECT_HPP_INCLUDED
