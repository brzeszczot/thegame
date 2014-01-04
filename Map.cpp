#include "Map.hpp"
using namespace std;

Map::Map(Imgs *imgs_in, unsigned int sx, unsigned int sy)
{
    imgs = imgs_in;
    wsp = new sf::Vector2f;
    wsp->x = sx;
    wsp->y = sy;

    shapes =  new vector<sf::Shape*>;
    bobjects = new vector<BObject*>;
    objects = new vector<Object*>;
    wobjects = new vector<WObject*>;
    fobjects = new vector<FObject*>;

    // odkrywanie ekranu
#ifdef _DEBUG
    view_switch = false;     // zakrywanie ekranu
#else
    view_switch = true;
#endif
    view_dist = 2;
    view_array_x = (int)sx/VIEW_SIZE+1;
    view_array_y = (int)sy/VIEW_SIZE+1;

    view_array = new bool*[view_array_y];
    for(int y=0;y<view_array_y;y++)
    {
        view_array[y] = new bool[view_array_x];
        for(int x=0;x<view_array_x;x++)
            view_array[y][x] = true;
    }

    /// generuj mape
    this->PlaceObjects();

    /// set background
    imgs->Get(Imgs::IMG_BG)->SetSmooth(false);
    bg.SetImage(*imgs->Get(Imgs::IMG_BG));
    bg.SetPosition(0,0);

    /// set bobjects
    //AddBobject(25);

    /// set fobject
    //for(size_t ii=0;ii<10;ii++)
    //    AddFobject();

    /// set objects
    //AddObject();

    /// set wobjects
    //AddWobject();

/*
    sf::Shape *sp = new sf::Shape;
    sp->AddPoint(400, 400, sf::Color::Blue);
    sp->AddPoint(800, 400, sf::Color::Red);
    sp->AddPoint(800, 800, sf::Color::Green);
    sp->AddPoint(400, 800, sf::Color::Yellow);

    sp->AddPoint(450, 700, sf::Color::Black);
     sp->AddPoint(400, 600, sf::Color::White);
      sp->AddPoint(450, 500, sf::Color::Green);

    sp->EnableOutline( true );
    sp->EnableFill( true );
    sp->SetOutlineWidth( 5 );
    shapes->push_back(sp);
*/

/*
    int points,x,y,temp;
    //top_xs = rand()%50+10;// ile+od_kad
    for(int corner=0;corner<1;corner++)
    {
        sf::Shape *sp = new sf::Shape;

        sp->AddPoint((int)sx/2, PX_TOUCH, sf::Color::Blue);
        sp->AddPoint((int)sx/2, 0, sf::Color::Blue);
        sp->AddPoint(0, 0, sf::Color::Blue);
        sp->AddPoint(0, MAX_BORDER_HEIGHT, sf::Color::Blue);

        if(corner==0 || corner==2)  points = rand()%((int)sx/50)+((int)(sx/100)/2);
        else if(corner==1 || corner==3)  points = rand()%((int)sy/3)+((int)sy/100);

        x = 0;
        y = MAX_BORDER_HEIGHT;
        for(unsigned int pp=0;pp<(unsigned int)points;pp++)
        {
            y -= (int)rand()%((int)MAX_BORDER_HEIGHT/10)+1;
            temp = ((sx/2)/points);
            x += (int)rand()%((int)temp*2)+PX_TOUCH;
            if(x>(int)sx/2||y<=PX_TOUCH)    break;
            sp->AddPoint(x, y, sf::Color::Yellow);
           // cout <<x <<"x"<<y<<endl;
        }
        sp->EnableOutline( false );
        sp->EnableFill( true );
        sp->SetOutlineWidth( 5 );
        //sp->SetBlendMode(sf::Blend::None);

        shapes->push_back(sp);
    }
*/
/*
    sp = new sf::Shape;
    sp->AddPoint( 300, 300, sf::Color::Red );
    sp->AddPoint( 300, 400, sf::Color::Green );
    sp->AddPoint( 400, 400, sf::Color::Blue );
   // sp->AddPoint( 500, 500, sf::Color::White );
    //sp->EnableFill( false );
    //sp->EnableOutline( true );
    //sp->SetOutlineWidth( 50 );
   // sp->SetCenter(550, 550);
   // sp->SetPosition(700,700);
       //sf::Vector2f polozenie = Polygon.GetPointPosition( 2 );
    //sf::Color kolorPunktu = Polygon.GetPointColor( 2 );
    //sf::Color kolorObramowania = Polygon.GetPointOutlineColor( 2 );
    shapes->push_back(sp);
    //delete sp;

    sf::Color col = sp->GetPixel(0,0);
*/
}

void Map::PlaceObjects()
{
    int ii, rnd_wobj;
    sf::Vector2f area, area_max, rnd_wobj_wsp, rnd_wobj_size;

    /// fobjects - ile ma ich byc
    int c_fobjects, c_fobjects_all;
    c_fobjects = c_fobjects_all = 5;

    /// dodawanie wobjects
    rnd_wobj = rand()%(int)(6);

    /// swiat bez pomieszczen
    if(rnd_wobj==0)
    {
        // objects
        area.y = (int)(rand()%((int)wsp->y-MAX_OBJECT_Y_SIZE))+1;
        area.x = (int)(rand()%((int)wsp->x-MAX_OBJECT_X_SIZE))+1;
        Object *ob = new Object(imgs,&area);
        ob->AddGun(Object::BASIC);
        ob->AddGun(Object::STONE);
        ob->AddGun(Object::FIREBALL);
        ob->AddGun(Object::LASER);
        objects->push_back(ob);

        // bobjects
        int c_bobjects = ((int)floor((wsp->x/MAX_BOBJECT_X_SIZE)-1)*(int)floor((wsp->y/MAX_BOBJECT_Y_SIZE)-1))/3;// gestosc otoczenia (drzew itp)
        AddBobject(c_bobjects);
    }

    /// dodawanie fobject - rozrzut losowy po ekranie
    //AddFobject(10);

    /// swiat z pomieszczeniami (wobjects) - oblicz rozmiary siatki
    area_max.y = (int)sqrt(rnd_wobj);
    area_max.x = (int)ceil(rnd_wobj/area_max.y);

    for(ii=0;ii<rnd_wobj;ii++)
    {
        area = this->CalcVectors(ii,area_max);

        // ustaw losowo Wobject
        WObject *wob = new WObject(imgs);
        rnd_wobj_wsp.x = (rand()%(int)(((wsp->x/(int)area_max.x)-wob->GetSize().x)-((MAX_OBJECT_X_SIZE*2)+2)))+(int)(area.x+MAX_OBJECT_X_SIZE)+2;
        rnd_wobj_wsp.y = (rand()%(int)(((wsp->y/(int)area_max.y)-wob->GetSize().y)-((MAX_OBJECT_Y_SIZE*2)+2)))+(int)(area.y+MAX_OBJECT_Y_SIZE)+2;
        rnd_wobj_size.x = ((int)(wsp->x/area_max.x));
        rnd_wobj_size.y = ((int)(wsp->y/area_max.y));
        wob->SetPosition(rnd_wobj_wsp.x, rnd_wobj_wsp.y);

        ///dodawanie objects
        int rnd_obj, rnd_obj_temp, jj;
        sf::Vector2f small_area, ss_size_l, ss_size_r, ss_cur_size;
        small_area = area;
        rnd_obj_temp = rnd_obj = (rand()%3)+1;
        int rnd_array[9];
        for(jj=0;jj<9;jj++) rnd_array[jj] = jj;
        for(jj=0;jj<rnd_obj;jj++)
            rnd_array[jj] = rnd_array[((rand()%9)+jj)];
        ss_size_l.x = (int)rnd_wobj_wsp.x-area.x;
        ss_size_l.y = (int)rnd_wobj_wsp.y-area.y;
        ss_size_r.x = (int)(area.x+(wsp->x/(int)area_max.x))-(rnd_wobj_wsp.x+wob->GetSize().x);
        ss_size_r.y = (int)(area.y+(wsp->y/(int)area_max.y))-(rnd_wobj_wsp.y+wob->GetSize().y);

        sf::Vector2f temp;
        for(jj=0;jj<9;jj++)
        {
            switch(jj)
            {
                case 0: ss_cur_size.x=ss_size_l.x; ss_cur_size.y=ss_size_l.y; break;
                case 1: small_area.x+=ss_size_l.x;
                        ss_cur_size.x=wob->GetSize().x; ss_cur_size.y=ss_size_l.y; break;
                case 2: small_area.x+=wob->GetSize().x;
                        ss_cur_size.x=ss_size_r.x; ss_cur_size.y=ss_size_l.y; break;
                case 3: small_area.x=area.x; small_area.y+=ss_size_l.y;
                        ss_cur_size.x=ss_size_l.x; ss_cur_size.y=wob->GetSize().y; break;
                case 4: small_area.x+=ss_size_l.x;
                        ss_cur_size.x=wob->GetSize().x; ss_cur_size.y=wob->GetSize().y; break;
                case 5: small_area.x+=wob->GetSize().x;
                        ss_cur_size.x=ss_size_r.x; ss_cur_size.y=wob->GetSize().y; break;
                case 6: small_area.x=area.x; small_area.y+=wob->GetSize().y;
                        ss_cur_size.x=ss_size_l.x; ss_cur_size.y=ss_size_r.y; break;
                case 7: small_area.x+=ss_size_l.x;
                        ss_cur_size.x=wob->GetSize().x; ss_cur_size.y=ss_size_r.y; break;
                case 8: small_area.x+=wob->GetSize().x;
                        ss_cur_size.x=ss_size_r.x; ss_cur_size.y=ss_size_r.y; break;
            }
            bool put_object = false;
            if(rnd_array[jj]==jj && rnd_obj_temp>0 && ss_cur_size.x>MAX_OBJECT_X_SIZE && ss_cur_size.y>MAX_OBJECT_Y_SIZE)
            {
                int rnd_temp;
                rnd_temp = (int)ss_cur_size.x-MAX_OBJECT_X_SIZE-(WOBJECT_WALL*2);
                if(rnd_temp<=0) rnd_temp = 1;
                temp.x = (int)small_area.x+(int)(rand()%rnd_temp)+(WOBJECT_WALL);

                rnd_temp = (int)ss_cur_size.y-MAX_OBJECT_Y_SIZE-(WOBJECT_WALL*2);
                if(rnd_temp<=0) rnd_temp = 1;
                temp.y = (int)small_area.y+(int)(rand()%rnd_temp)+(WOBJECT_WALL);

                Object *ob;
                if(objects->size()<=0)
                {
                    ob = new Object(imgs,&temp);
                    ob->AddGun(Object::BASIC);
                    ob->AddGun(Object::STONE);
                    ob->AddGun(Object::FIREBALL);
                    ob->AddGun(Object::LASER);
                }
                else
                {
                    ob = new Object(imgs,&temp,Imgs::IMG_MAIN2);
                    ob->AddGun(Object::STONE);
                    ob->params->move_speed = MIN_SPEED;
                    //ob->params->guns[ob->active_gun]->gun = Object::STONE;
                    ob->params->guns[ob->params->active_gun]->shot_time_allowed = 1.5;
                }
                // srodkowy prostokat z 9
                if(jj==4)
                {
                    continue;   // nie stawiaj przeciwnika w srodku pomieszczen
                    ob->w_inside = wob; // ten object is inside wobject
                    ob->SetPosition(small_area.x+WOBJECT_WALL+1,small_area.y+WOBJECT_WALL+1);
                }
                objects->push_back(ob);
                rnd_obj_temp--;
                put_object = true;
            }

            /// dodawanie bobjects
            temp.x = temp.y = 0;
            int rnd_temp;
            if(!put_object && jj!=4)
            {
                int c_bobjects = ((int)floor((ss_cur_size.x/MAX_BOBJECT_X_SIZE)-1)*(int)floor((ss_cur_size.y/MAX_BOBJECT_Y_SIZE)-1))/2;// gestosc otoczenia (drzew itp)
                for(int kk=0;kk<c_bobjects;kk++)
                {
                    /// dans le future il peux etre ajouter encore posibles
                    if(jj==7)   // jesli obszar od wejscia do pomieszczenia
                    {
                        rnd_temp = (int)ss_cur_size.y-MAX_BOBJECT_Y_SIZE-MAX_BOBJECT_Y_SIZE;
                        if(rnd_temp>MAX_BOBJECT_Y_SIZE)  temp.y = (int)small_area.y+(int)(rand()%rnd_temp)+MAX_BOBJECT_Y_SIZE;
                    }
                    else
                    {
                        rnd_temp = (int)ss_cur_size.y-MAX_BOBJECT_Y_SIZE;
                        if(rnd_temp>MAX_BOBJECT_Y_SIZE)  temp.y = (int)small_area.y+(int)rand()%rnd_temp;
                    }
                    rnd_temp = (int)ss_cur_size.x-MAX_BOBJECT_X_SIZE;
                    if(rnd_temp>MAX_BOBJECT_X_SIZE)  temp.x = (int)small_area.x+(int)rand()%rnd_temp;
                    if(temp.x>0 && temp.y>0)
                    {
                        BObject *sp = new BObject(imgs,&temp);
                        bobjects->push_back(sp);
                    }
                }
            }
            /// dodawanie fobjects
            if(c_fobjects>0 && jj==4)
            {
                // rozdysponuj wszystkie znajdzki w pomieszczeniach
                int rnd_c = (int)(rand()%(int)c_fobjects_all/2)+1;
                if((rnd_wobj-1)==ii)
                    rnd_c = c_fobjects;
                for(int kk=0;kk<rnd_c;kk++)
                {
//                    rnd_temp = (int)ss_cur_size.y-(FOBJECT_MAX_SIZE+WOBJECT_WALL*2);
//                    if(rnd_temp>0)  temp.y = (int)(small_area.y+(int)rand()%rnd_temp)+WOBJECT_WALL;
//                    rnd_temp = (int)ss_cur_size.x-(FOBJECT_MAX_SIZE+WOBJECT_WALL*2);
//                    if(rnd_temp>0)  temp.x = (int)(small_area.x+(int)rand()%rnd_temp)+WOBJECT_WALL;
                    temp.x = (int)small_area.x+WOBJECT_WALL+5+((FOBJECT_MAX_SIZE+2)*kk);
                    temp.y = (int)small_area.y+WOBJECT_WALL+5;
                    if(temp.x>0 && temp.y>0)
                    {
                        FObject *fob = new FObject(imgs,&temp);
                        //if(jj==4 && !put_object)
                        fob->w_inside = wob;
                        fobjects->push_back(fob);
                        c_fobjects--;
                    }
                }
            }
        }

        wobjects->push_back(wob);
    }
}

sf::Vector2f Map::CalcVectors(int frame, sf::Vector2f area_max)
{
    sf::Vector2f area;

    // oblicz X i Y w tablicy dla danej komorki ii
    bool rest = false;
    if((frame+1)%(int)area_max.x!=0)    rest = true;
    area.y = (int)floor((frame+1)/area_max.x);
    area.x = (int)(frame+1)-(int)(area.y*area_max.x);
    if(rest)    area.y++;
    if(area.x==0)   area.x = area_max.x;

    // naloz wsp X i Y na rzeczywisty rozmiar okna
    area.x = (int)(area.x-1)*(wsp->x/(int)area_max.x);
    area.y = (int)(area.y-1)*(wsp->y/(int)area_max.y);

    return area;
}

void Map::AddBobject(size_t cc)
{
    sf::Vector2f temp, rnd;
    temp = (*objects)[0]->GetPosition();
    for(size_t ii=0;ii<cc;ii++)
    {
        rnd.x = rand()%((int)wsp->x-MAX_BOBJECT_X_SIZE);
        rnd.y = rand()%((int)wsp->y-MAX_BOBJECT_Y_SIZE);
        if(((rnd.x+MAX_BOBJECT_X_SIZE>=temp.x)&&(rnd.x<temp.x+MAX_OBJECT_X_SIZE))&&(rnd.y+MAX_BOBJECT_Y_SIZE>=temp.y)&&(rnd.y<temp.y+MAX_OBJECT_Y_SIZE));
        else
        {
            BObject *sp = new BObject(imgs,&rnd);
            bobjects->push_back(sp);
        }
    }
}

void Map::AddObject()
{
    Object *ob;
    if(objects->size()<=0)
        ob = new Object(imgs,wsp);
    else
        ob = new Object(imgs,wsp,Imgs::IMG_MAIN2);
    objects->push_back(ob);
}

void Map::AddWobject()
{
    WObject *wob = new WObject(imgs,wsp);
    wobjects->push_back(wob);
}

void Map::AddFobject(size_t cc)
{
    sf::Vector2f rnd;
    for(size_t ii=0;ii<cc;ii++)
    {
        rnd.x = rand()%((int)wsp->x)+1;
        rnd.y = rand()%((int)wsp->y)+1;
        FObject *fob = new FObject(imgs,&rnd);
        fobjects->push_back(fob);
    }
}

Map::~Map()
{
    for(size_t ii=0;ii<(size_t)shapes->size();ii++)
        delete (*shapes)[ii];
    delete shapes;
    for(size_t ii=0;ii<(size_t)bobjects->size();ii++)
        delete (*bobjects)[ii];
    delete bobjects;
    for(size_t ii=0;ii<(size_t)objects->size();ii++)
        delete (*objects)[ii];
    delete objects;
    for(size_t ii=0;ii<(size_t)wobjects->size();ii++)
        delete (*wobjects)[ii];
    delete wobjects;
    for(size_t ii=0;ii<(size_t)fobjects->size();ii++)
        delete (*fobjects)[ii];
    delete fobjects;
    for(int ii=0;ii<view_array_y;ii++)
        delete view_array[ii];
    delete view_array;
}
