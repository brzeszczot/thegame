#include "Bullet.hpp"
using namespace std;

Bullet::Bullet(Imgs *imgs, enum Imgs::ImgName gt)
{
    show = true;
    shot_pos = 0;
    shot_speed = 3;

    SetImage(*imgs->Get(Imgs::IMG_GUN, gt));
    SetScale(1,1);
    SetFrameSize(GetSize().x/4,GetSize().y);
    SetCenter(0,0);
    SetLoopSpeed(10);
    Play(0,3);
    //Rotate(34);
}

Bullet::~Bullet()
{
}
