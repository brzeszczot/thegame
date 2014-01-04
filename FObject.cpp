#include "FObject.hpp"
using namespace std;

FObject::FObject(Imgs *imgs, sf::Vector2f *wsp)
{
    type = Imgs::FOBJ;
    show = true;
    inside = false;
    w_inside = NULL;

    if(wsp!=NULL)   SetPosition(wsp->x, wsp->y);
    imgs_name = (Imgs::ImgName)imgs->Get2(Imgs::IMG_FOBJECT);
    SetImage(*(*imgs->img_fobject)[imgs_name]);
    //SetImage(*imgs->Get(Imgs::IMG_FOBJECT, Imgs::IMG_GOLD1));
    SetScale(1,1);
    SetFrameSize(GetSize().x/3,GetSize().y);
    SetLoopSpeed(5);
    Play(0,3);
}

FObject::~FObject()
{
}
