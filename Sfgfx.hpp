#ifndef SFGFX_HPP_INCLUDED
#define SFGFX_HPP_INCLUDED

#define MATRIX_SIZE 3       // maksymalny rozmiar tablicy z mapami

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <sstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
#include "collision.hpp"
#include "polygon.hpp"
#include "Ini.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "WObject.hpp"
#include "BObject.hpp"
#include "Map.hpp"
#include "Imgs.hpp"
#include "Sndbuff.hpp"
#include "AniSprite.hpp"
#include "Alg.hpp"
#include "File.hpp"

#define GAME_VERSION 0.92
//#define DIR_RES "res/"

class Sfgfx
{
    private:
        enum e_game_stat {MENU, WORLD, MAPS};
        e_game_stat game_status;

        Map ***matrix;              // tablica dwuwymiarowa z historia poruszania sie miedzy ekranami
        sf::Vector2i matrix_pos, matrix_last_pos;     // aktualna pozycja w przestrzeni poziomw "matrix"
        Map *world_map;             // mapa glownego swiata

        Ini *ini;                   // uchwyt do danych z pliku INI
        Engine *engine;             // uchwyt do obiektu z glowna logika gry
        sf::RenderWindow *App;      // uchwyt do silnika grafiki
        Object *user_object;        // wskaznik do obiektu ktory jest sterowany przez gracza
        Map *current_map;           // wskaznik do aktualnej mapy
        Map *last_map;              // ostatnia wczytana mapa
        vector<Map*> *maps;         // kontener z mapami
        sf::Clock Clock;            // zegarek
        float game_speed;           // szybkosc wyswietlania klatek na sekunde
        float framerate;            // wartosc wymnozona przez czas przejscia jednej iteracji w glownej petli
        Imgs *imgs;                 // uchwyt do obiektu z obrazkami
        Sndbuff *snds;                 // uchwyt do dzwiekow
        Algorithms *alg;            // uchwyt do obiektu z algorytmamy pomocniczymi
        AniSprite cursor;
    public:
        Sfgfx();
        ~Sfgfx();
        void Run();
        void* ObjectCollision(AniSprite *object, Map *map, void *adr = NULL);     // sprawdzanie czy object koliduje z jakimkolwiek innym obiektem, wobjectem, bobjectem
        Map* GenMap(bool main = false);
        void Shot(Object *obj, int x, int y);
};

#endif // SFGFX_HPP_INCLUDED
