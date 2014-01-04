#ifndef POLYGON_HPP_INCLUDED
#define POLYGON_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
using namespace std;

class Polygon
{
    public:
        struct vertex 			//Wierzcholek
        {
            int x,y;
        };
        vertex 	polygon[2000];	//Tablica wierzcholow wielokata, max: 20
        vertex 	p;				//Dany punkt p
        vertex 	r;				//Drugi koniec odcinka |PR|
        vertex 	tmp;
        int n;			    //Liczba wierzchalkow wielokata
        int k;

        Polygon(int cwsp, int *wsp);
        int min(int a, int b);
        int max(int a, int b);
        int sign(int a);
        int przynaleznosc(vertex x, vertex y, vertex z);
        int det(vertex x, vertex y, vertex z); //Wyznacznik macierzy kwadratowej stopnia 3.
        int przecinanie(vertex a, vertex b);
        bool oblicz(int x, int y);
};

#endif // POLYGON_HPP_INCLUDED
