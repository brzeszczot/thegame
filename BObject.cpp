#include "BObject.hpp"
using namespace std;

BObject::BObject(Imgs *imgs, sf::Vector2f *wsp)
{
    type = Imgs::BOBJ;
    show = true;
    wall = true;

    SetImage(*imgs->Get(Imgs::IMG_BOBJECT));
    SetScale(1,1);
    SetPosition(wsp->x, wsp->y);
    //SetCenter(0, 0);
    //Rotate(34);
}

BObject::~BObject()
{
}
