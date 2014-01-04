#ifndef ALG_HPP_INCLUDED
#define ALG_HPP_INCLUDED
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class Algorithms
{
    public:
        Algorithms();
        ~Algorithms();
        int SectionLength(sf::Vector2f v1, sf::Vector2f v2);
        bool ObjectVisible(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f v1, sf::Vector2f v2);
        sf::Vector2f SectionCalc(int x1, int y1, int x2, int y2, int bytesPerLine, vector<sf::Vector2f> *v, int deep = -1);
};

#endif // ALG_HPP_INCLUDED
