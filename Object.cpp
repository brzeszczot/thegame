#include "Object.hpp"
using namespace std;

Object::Object(Imgs *imgs, sf::Vector2f *wsp, enum Imgs::ImgName en)
{
    type = Imgs::OBJ;
    show = true;

    // ustaw poczatkowe domyslne wartosci dla kazdego z obiektow
    params = new parameters;
    params->life = 50;
    params->gold = 10;
    params->move_speed = LOW_SPEED;
    params->visible_dist = MEDIUM_VISIBLE;
    params->shot_dist = MEDIUM_SHOT;
    params->active_gun = 0;

    w_inside = NULL;
    last_contact.x = last_contact.y = -1;
    move_direction = last_direction = 0;
    status = READY;

    SetScale(1,1);
    if(wsp!=NULL)   SetPosition(wsp->x, wsp->y);
    SetImage(*imgs->Get(Imgs::IMG_OBJECT, en));
    //SetCenter(0, 0);
    SetFrameSize(GetSize().x/4,GetSize().y/4);
    SetLoopSpeed(10);
    SetFrame(8);

    last_position.x = wsp->x;
    last_position.y =  wsp->y;
}

Object::~Object()
{
}

void Object::AddGun(enum gun_type gt)
{
    gun *g = new gun;                   // tworzymy bron
    g->gun = gt;                         // ustawiamy typ broni
    g->ammo = 100;                      // ustaw ilosc amonicji na poczatek
    g->damage = 10;                     // sila uderzenia pocisku
#ifdef _DEBUG
    g->shot_time_allowed = 0.001;         // raz na ile sekund strzal! - np 0.3 sekundy
#else
    // charakterystyka broni
    switch(gt)
    {
        case BASIC: g->shot_time_allowed = 0.3; break;
        case STONE: g->shot_time_allowed = 0.6; break;
        case FIREBALL: g->shot_time_allowed = 1.4; g->damage = 30; break;
        case LASER: g->shot_time_allowed = 0.8; g->damage = 20; break;
    }
#endif
    params->guns.push_back(g);          // wrzuc bron nr 1 do plecaka
}

void Object::MoveByDirection(size_t direction, float speed_unit)
{
    switch(direction)
    {
        case 1: this->Move(0,-speed_unit); break;// gora
        case 5: this->Move(0,+speed_unit); break;// dol
        case 7: this->Move(-speed_unit,0); break;// lewo
        case 3: this->Move(+speed_unit,0); break;// prawo
    }
}

size_t Object::MoveDirectionReverse(size_t direction)
{
    switch(direction)
    {
        case 1: return 5; break;// gora
        case 5: return 1; break;// dol
        case 7: return 3; break;// lewo
        case 3: return 7; break;// prawo
        default: return 0;
    }
}
