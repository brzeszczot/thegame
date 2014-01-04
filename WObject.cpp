#include "WObject.hpp"
using namespace std;

WObject::WObject(Imgs *imgs, sf::Vector2f *wsp)
{
    type = Imgs::WOBJ;
    show = true;
    inside = false;

    if(wsp!=NULL)   SetPosition(wsp->x, wsp->y);
    SetImage(*imgs->Get(Imgs::IMG_WOBJECT));
    SetScale(1,1);
    SetFrameSize(GetSize().x/3,GetSize().y);
    SetFrame(0);
}

WObject::~WObject()
{
}
