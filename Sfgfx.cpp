#include "Sfgfx.hpp"

Sfgfx::Sfgfx()
{
    ini = new Ini("conf.ini");
    App = new sf::RenderWindow;

    switch(ini->get("gfx_mode"))
    {
        case 0: App->Create(sf::VideoMode::GetDesktopMode(), ""); break;
        case 1: App->Create(sf::VideoMode::GetDesktopMode(), "", sf::Style::Fullscreen); break;
        case 2: App->Create(sf::VideoMode(ini->get("res_x"), ini->get("res_y"), ini->get("color_depth")), ""); break;
        case 3: App->Create(sf::VideoMode(ini->get("res_x"), ini->get("res_y"), ini->get("color_depth")), "", sf::Style::Fullscreen); break;
    }
    if(ini->get("gfx_mode")==0 || ini->get("gfx_mode")==1)
    {
        ini->set("res_x",App->GetWidth());
        ini->set("res_y",App->GetHeight());
        if(!ini->save()) throw(411);
    }

    App->UseVerticalSync(true);
    App->SetFramerateLimit(60);
    App->ShowMouseCursor(false);

    // przygotowanie tablicy dwuwymiarowej trzymajacej historie poruszania sie po poziomach
    matrix = new Map**[MATRIX_SIZE];
    for(int ii=0;ii<MATRIX_SIZE;ii++)
    {
        matrix[ii] = new Map*[MATRIX_SIZE];
        for(int jj=0;jj<MATRIX_SIZE;jj++)
            matrix[ii][jj] = NULL;
    }
    matrix_pos.x = MATRIX_SIZE/2;
    matrix_pos.y = MATRIX_SIZE/2;

    user_object = NULL;
    current_map = NULL;

    // przygotowanie glownych zmiennych uzywanych w programie
    engine = new Engine(App->GetWidth(),App->GetHeight());
    imgs = new Imgs;
    snds = new Sndbuff;
    alg = new Algorithms;
    maps = new vector<Map*>;

    // status gry - MENU (menu), WORLD (glowna mapa swiata gry), MAPS (w trakcie gry)
    game_status = WORLD;

    //v_temp = new vector<sf::Vector2f*>;
    game_speed = 0;
}

void Sfgfx::Run()
{
    /// generowanie mapy
    GenMap(true);

    /// obraz zakrywajacy mape
    sf::Image img_view;
    img_view.Create(VIEW_SIZE, VIEW_SIZE, sf::Color::Black);
    img_view.SetSmooth(false);
    sf::Sprite sprite_view;
    sprite_view.SetImage(img_view);
    sprite_view.SetColor(sf::Color::Black);

    /// animowany kursor
    this->cursor.SetImage(*imgs->Get(Imgs::IMG_CURSORS,Imgs::IMG_CURSOR1));
    this->cursor.SetScale(1,1);
    this->cursor.SetFrameSize(this->cursor.GetSize().x,this->cursor.GetSize().y);
    this->cursor.SetCenter(0, 0);
    this->cursor.SetPosition((App->GetWidth()/2)-(this->cursor.GetSize().x/2), (App->GetHeight()/2)-(this->cursor.GetSize().y/2));
    //this->cursor.SetLoopSpeed(5);
    //this->cursor.Play(0,3);
    //Rotate(34);

    /// ustaw parametry dzwikow
    snds->Get(Sndbuff::SND_WALKING)->SetLoop(true);
    snds->Get(Sndbuff::SND_WALKING_INSIDE)->SetLoop(true);
    snds->Get(Sndbuff::SND_WALKING)->SetVolume(40.f);
    snds->Get(Sndbuff::SND_WALKING_INSIDE)->SetVolume(40.f);
    //if(snds->music->GetStatus()!=sf::Music::Playing)
        snds->Music(Sndbuff::MUSIC_MAIN);

    /// glowna petla programu
    while(App->IsOpened())
    {
        //float czas = sfClock_GetTime(zegar); //DO ANIMACJI
        this->framerate = Clock.GetElapsedTime();
        Clock.Reset();

        /// PRZECHWYTYWANIE KLAWIATURY I MYSZKI
        const sf::Input &steerage = App->GetInput();
        sf::Event ev;
        while(App->GetEvent(ev))
        {
            if(ev.Type == sf::Event::Closed )
                App->Close();
            if(ev.Type == sf::Event::KeyPressed && ev.Key.Code == sf::Key::Escape)
                App->Close();
            if(ev.Type == sf::Event::MouseButtonPressed && ev.MouseButton.Button == sf::Mouse::Left)
                Shot(user_object, steerage.GetMouseX(), steerage.GetMouseY());
        }

        if(steerage.IsKeyDown(sf::Key::Num1)) { if(user_object->params->move_speed>=5) user_object->params->move_speed-=5;  }
        if(steerage.IsKeyDown(sf::Key::Num2)) { user_object->params->move_speed+=5;  }
#ifdef _DEBUG
        if(steerage.IsKeyDown(sf::Key::Num3)) { GenMap(); }
#endif
        if(steerage.IsKeyDown(sf::Key::Num4)) { current_map->view_switch=!current_map->view_switch; sf::Sleep(0.1); }
        //if(steerage.IsKeyDown(sf::Key::S)) { File f; f.save(maps); }
        if(steerage.IsKeyDown(sf::Key::F1)) { user_object->params->active_gun = 0; }
        else if(steerage.IsKeyDown(sf::Key::F2)) { user_object->params->active_gun = 1; }
        else if(steerage.IsKeyDown(sf::Key::F3)) { user_object->params->active_gun = 2; }
        else if(steerage.IsKeyDown(sf::Key::F4)) { user_object->params->active_gun = 3; }
        else if(steerage.IsKeyDown(sf::Key::F5)) { user_object->params->active_gun = 4; }

        // jesli gracz niezyje albo jestesmy w menu albo na glownej mapie - skroc petle
        if(user_object->status==DEAD)
            continue;

        user_object->last_direction = user_object->move_direction;
        user_object->move_direction = 0;
        float speed_unit = (user_object->params->move_speed+this->game_speed)*this->framerate;

        if(steerage.IsKeyDown(sf::Key::Left) || steerage.IsKeyDown(sf::Key::A))
        {
            user_object->move_direction = 7;
            user_object->last_position = user_object->GetPosition();
            user_object->Move(-speed_unit, 0);
            if(ObjectCollision(user_object, current_map))
                user_object->SetPosition(user_object->last_position);
        }
        if(steerage.IsKeyDown(sf::Key::Right) || steerage.IsKeyDown(sf::Key::D))
        {
            user_object->move_direction = 3;
            user_object->last_position = user_object->GetPosition();
            user_object->Move(+speed_unit, 0);
            if(ObjectCollision(user_object, current_map))
                user_object->SetPosition(user_object->last_position);
        }
        if(steerage.IsKeyDown(sf::Key::Up) || steerage.IsKeyDown(sf::Key::W))
        {
            //if(user_object->move_direction==7)  user_object->move_direction = 8;
            //else if(user_object->move_direction==3)  user_object->move_direction = 2;
            //else
            user_object->move_direction = 1;
            user_object->last_position = user_object->GetPosition();
            user_object->Move(0, -speed_unit);
            if(ObjectCollision(user_object, current_map))
                user_object->SetPosition(user_object->last_position);
        }
        if(steerage.IsKeyDown(sf::Key::Down) || steerage.IsKeyDown(sf::Key::S))
        {
            //if(user_object->move_direction==7)  user_object->move_direction = 6;
            //else if(user_object->move_direction==3)  user_object->move_direction = 4;
            //else
            user_object->move_direction = 5;
            user_object->last_position = user_object->GetPosition();
            user_object->Move(0, +speed_unit);
            if(ObjectCollision(user_object, current_map))
                user_object->SetPosition(user_object->last_position);
        }

        /// sprawdzenie wejscia do srodka wobject
        if(user_object->move_direction>0)
            for(int kk=0;kk<(int)current_map->wobjects->size();kk++)
            {
                (*current_map->wobjects)[kk]->inside=false;
                user_object->w_inside = NULL;

                sf::Vector2f vpoint2;
                vpoint2 = (*current_map->wobjects)[kk]->TransformToLocal(sf::Vector2f(user_object->GetPosition().x+(user_object->GetSize().x/2),user_object->GetPosition().y+(user_object->GetSize().y/2)));
                if(vpoint2.x >= 0 && vpoint2.y >= 0 && vpoint2.x <= (*current_map->wobjects)[kk]->GetSize().x && vpoint2.y <= (*current_map->wobjects)[kk]->GetSize().y)
                {
                    (*current_map->wobjects)[kk]->inside=true;
                    user_object->w_inside = (*current_map->wobjects)[kk];
                    // zmien odtwarzanie dzwieku chodzenia na INSIDE
                    if(snds->Get(Sndbuff::SND_WALKING_INSIDE)->GetStatus()!=sf::Sound::Playing)
                    {
                        snds->Get(Sndbuff::SND_WALKING)->Stop();
                        snds->Get(Sndbuff::SND_WALKING_INSIDE)->Play();
                    }
                    break;
                }
            }

        /// odtwarzanie dzwiekow
        // odwarzanie dzwieku chodzenia na zewnatrz (jesli wyszlismy z pomieszczenie LUB stalismy i nagle zaczelismy isc)
        if((snds->Get(Sndbuff::SND_WALKING_INSIDE)->GetStatus()==sf::Sound::Playing && user_object->w_inside==NULL)
            || (user_object->last_direction==0 && user_object->last_direction!=user_object->move_direction))
        {
            snds->Get(Sndbuff::SND_WALKING_INSIDE)->Stop();
            snds->Get(Sndbuff::SND_WALKING)->Play();
        }
        else if(user_object->move_direction==0 && user_object->last_direction!=user_object->move_direction)
        {
            snds->Get(Sndbuff::SND_WALKING)->Stop();
            snds->Get(Sndbuff::SND_WALKING_INSIDE)->Stop();
        }

        /// bardzo sztuczna inteligencja dla objektow :)
        Object *obj = NULL;
        for(int ii=0;ii<(int)current_map->objects->size();ii++)
        {
            //cout << "BEGIN" <<endl;
            obj = (*current_map->objects)[ii];

            if(obj->status==DEAD)   continue;

            if(obj!=user_object)    // potrzebne do ustawiania wlasciwej animacji - wyjatek dla user_object
            {
                obj->lldirection = obj->last_direction;
                obj->last_direction = obj->move_direction;
            }
            size_t distance = alg->SectionLength(obj->GetPosition(),obj->last_contact);

            Object::parameters *p = new Object::parameters;
            p = obj->params;

            /// kolizje pociskow i ich AI
            sf::Vector2f shot_size;
            for(int current_gun=0;current_gun<(int)obj->params->guns.size();current_gun++)
            {
                if(obj->params->guns[current_gun]->shots.size()>0)
                {
                    for(size_t nn=0;nn<(size_t)obj->params->guns[current_gun]->shots.size();nn++)
                    {
                        int size = obj->params->guns[current_gun]->shots[nn]->vec_shot.size();
                        unsigned int shot_pos = obj->params->guns[current_gun]->shots[nn]->shot_pos;

                        bool shot_remove = false;
                        if((int)shot_pos<size)
                        {
                            // przesun pocisk o kolejna pozycje
                            obj->params->guns[current_gun]->shots[nn]->shot_pos+=obj->params->guns[current_gun]->shots[nn]->shot_speed;
                            // ustaw nowa pozycje pocisku
                            shot_size = obj->params->guns[current_gun]->shots[nn]->GetSize();
                            obj->params->guns[current_gun]->shots[nn]->SetPosition(obj->params->guns[current_gun]->shots[nn]->vec_shot[shot_pos].x-(shot_size.x/2), obj->params->guns[current_gun]->shots[nn]->vec_shot[shot_pos].y-(shot_size.y/2));
                            int shot_dist = alg->SectionLength(obj->params->guns[current_gun]->shots[nn]->vec_shot[0],obj->params->guns[current_gun]->shots[nn]->vec_shot[shot_pos]);
                            // jesli kolizja lub za daleki dystans - usun strzal i pocisk
                            void *ret = ObjectCollision(obj->params->guns[current_gun]->shots[nn], current_map, obj);
                            if(ret!=NULL)
                            {
                                // niezbyt eleganckie ale rozpoznaje typ obj zwracanego przez ObjectColision i jesli to byl object to rob swoje
                                Object *o = (Object*)ret;
                                if(o->type==Imgs::OBJ)
                                {
                                    if(o->params->life>0)   o->params->life-=obj->params->guns[current_gun]->damage;
                                    if(o->params->life<=0)
                                    {
                                        o->show = false;
                                        //o->move_direction = 0;
                                        obj->move_direction = 0;
                                        o->status = DEAD;
                                        snds->Get(Sndbuff::SND_DIED)->Play();
                                        if(o==user_object)
                                        {
                                            snds->Get(Sndbuff::SND_WALKING)->Stop();
                                            snds->Get(Sndbuff::SND_WALKING_INSIDE)->Stop();
                                        }
                                    }
                                    else
                                    {
                                        if(o==user_object)  snds->Get(Sndbuff::SND_HITED_BODY)->Play();
                                        else                snds->Get(Sndbuff::SND_HITED)->Play();
                                    }
                                }
                                else
                                    snds->Get(Sndbuff::SND_HITED)->Play();
                                shot_remove = true;
                            }
                            if(shot_dist>obj->params->shot_dist)
                                shot_remove = true;
                        }
                        else shot_remove = true;
                        // usun strzal i pocisk
                        if(shot_remove)
                        {
                            delete obj->params->guns[current_gun]->shots[nn];
                            obj->params->guns[current_gun]->shots.erase(obj->params->guns[current_gun]->shots.begin()+nn);// usun strzal z pamieci
                        }
                    }
                }
            }
            /// sprawdzanie widocznosci - czy obiekt widzi obiekt sterowany przez gracza
            if(obj!=user_object && (user_object->move_direction>0 || obj->move_direction>0) && user_object->w_inside==obj->w_inside)
            {
                //cout << "WIDOCZNOSC:" << endl;
                bool view_collision = false;
                sf::Vector2f temp_uo,temp_obj;
                temp_uo = user_object->GetPosition();
                temp_obj = obj->GetPosition();
                temp_uo.x += (int)user_object->GetSize().x/2;
                temp_uo.y += (int)user_object->GetSize().y/2;
                temp_obj.x += (int)obj->GetSize().x/2;
                temp_obj.y += (int)obj->GetSize().y/2;
                for(int kk=0;kk<(int)current_map->wobjects->size();kk++)
                    if(!alg->ObjectVisible(temp_uo,temp_obj,(*current_map->wobjects)[kk]->GetPosition(),(*current_map->wobjects)[kk]->GetSize()))
                    {
                        view_collision = true;
                        break;
                    }
                for(int kk=0;kk<(int)current_map->bobjects->size();kk++)
                    if(!alg->ObjectVisible(temp_uo,temp_obj,(*current_map->bobjects)[kk]->GetPosition(),(*current_map->bobjects)[kk]->GetSize()))
                    {
                        view_collision = true;
                        break;
                    }
                for(int kk=0;kk<(int)current_map->objects->size();kk++)
                    if(!alg->ObjectVisible(temp_uo,temp_obj,(*current_map->objects)[kk]->GetPosition(),(*current_map->objects)[kk]->GetSize()) && obj!=(*current_map->objects)[kk] && (*current_map->objects)[kk]!=user_object)
                    {
                        view_collision = true;
                        break;
                    }
                if(!view_collision && alg->SectionLength(temp_uo,temp_obj)<obj->params->visible_dist)
                {
                    obj->last_contact = user_object->GetPosition();
                    Shot(obj, obj->last_contact.x+(int)obj->GetSize().x/3, obj->last_contact.y+(int)obj->GetSize().y/4);
                }
            }

            /// przemieszczanie sie obiektow (jesli obiekt widzial juz jakis inny obiect)
            if(distance>ZERO_VISIBLE && user_object->status!=DEAD && obj!=user_object && obj->last_contact.x>-1 && obj->last_contact.y>-1)//distance>MIN_VISIBLE &&
            {
                //cout << "PRZEMIESZCZANIE" <<endl;
                float speed_unit = (obj->params->move_speed+this->game_speed)*this->framerate;
                if(obj->last_contact.x>0 && obj->last_contact.y>0)//&& (obj->w_inside==NULL || obj->w_inside->inside)// przemieszczaj obiekt o ile nie znajduje sie w wobject
                {
                    // wyznaczanie kierunkow poruszania - potrzebne do ustawienia animacji
                    int x = max(obj->last_contact.x,obj->GetPosition().x)-min(obj->last_contact.x,obj->GetPosition().x);
                    int y = max(obj->last_contact.y,obj->GetPosition().y)-min(obj->last_contact.y,obj->GetPosition().y);
                    obj->last_direction = obj->move_direction;
                    obj->last_position = obj->GetPosition();
                    if(x>y)
                    {
                        if(obj->last_contact.x>=obj->GetPosition().x)   obj->move_direction = 3;
                        else                                            obj->move_direction = 7;
                    }
                    else
                    {
                        if(obj->last_contact.y>=obj->GetPosition().y)   obj->move_direction = 5;
                        else                                            obj->move_direction = 1;
                    }
                    // przemiesc obiekt (troszke przyspieszmy obiekt 1.2 - chodzac po przekatnej robil to zbyt wolno wzgledem poruszania sie po X i Y)
                    obj->MoveByDirection(obj->move_direction,speed_unit*1.2);

                    // jesli tryb omijania - nie bruzdzic z kierunkami :)
                    if(obj->status==M_AVOID)
                        obj->move_direction = obj->last_direction;

                    /// sprawdzanie kolizji z innymi obiektami (lacznie z ozdobami)
                    if((bool)ObjectCollision(obj, current_map))
                    {
                        obj->SetPosition(obj->last_position);
                        // ustaw kierunek ruchu obiekty do ominiecia przeszkody - ale tylko gdy pierwszy raz obiekt przechodzi w tryb omijania
                        if(obj->status!=M_AVOID)
                        {
                            switch(obj->move_direction)
                            {
                                case 1: if(obj->last_contact.x <= obj->GetPosition().x) { obj->move_direction=7; } else { obj->move_direction=3; } break;// gora
                                case 5: if(obj->last_contact.x <= obj->GetPosition().x) { obj->move_direction=7; } else { obj->move_direction=3; } break;// dol
                                case 7: if(obj->last_contact.y <= obj->GetPosition().y) { obj->move_direction=1; } else { obj->move_direction=5; } break;// lewo
                                case 3: if(obj->last_contact.y <= obj->GetPosition().y) { obj->move_direction=1; } else { obj->move_direction=5; } break;// prawo
                            }
                        }
                        obj->status = M_AVOID;
                    }
                    else obj->status = MOVING;

                    // tryb omijania przeszkody
                    if(obj->status==M_AVOID)
                    {
                        obj->last_position = obj->GetPosition();
                        //if(alg->SectionLength(obj->GetPosition(),obj->last_contact)>obj->visible_dist)
                        //{
                        //        obj->last_direction = obj->move_direction;
                        //    obj->move_direction = obj->MoveDirectionReverse(obj->move_direction);
                        //}
                        obj->MoveByDirection(obj->move_direction,speed_unit);

                        if((bool)ObjectCollision(obj, current_map))
                        {
                            obj->SetPosition(obj->last_position);
                            obj->last_direction = obj->move_direction;
                            obj->move_direction = obj->MoveDirectionReverse(obj->move_direction);
                        }
                    }
                }
            }
            if(distance<=ZERO_VISIBLE)//jak jest blisko celu - na miejscu - zatrzymaj go
            {
                obj->status = READY;
                obj->move_direction = 0;
            }

            /// ustawianie animacji bohatera
            // no wiec: zmien aniamcje jak kierunek sie zmienil, zabiezpieczenie przed zapetleniem w zmianie animiacji oprocz kierunku = 0
            // druga linia: przejscie z powyzszego zabezpieczenia do zwyklego chodzenia w jednym kierunku - wyjatek do zmiany animiacji
            if((obj->last_direction!=obj->move_direction && (obj->lldirection!=obj->move_direction || obj->move_direction==0))
                || (obj->last_direction==obj->move_direction && obj->lldirection!=obj->last_direction && obj!=user_object))
            {
                //cout << "ANIMACJA" <<endl;
                switch(obj->move_direction)
                {
                    case 0: obj->SetFrame(8); obj->Stop(); break;
                    case 7: obj->Play(13,16); break;    // lewo
                    case 3: obj->Play(5,8); break;      // prawo
                    case 1: obj->Play(1,4); break;      // gora
                    case 5: obj->Play(9,12); break;     // dol
                }
            }
        }

        /// odkrywanie widzialnego obszaru
        if(current_map->view_switch && (user_object->move_direction>0 || obj->last_direction==0))
        {
            sf::Vector2f vpoint;
            vpoint = user_object->GetPosition();

            vpoint.x+=(int)(user_object->GetSize().x/2);
            vpoint.y+=(int)(user_object->GetSize().y/2);
            vpoint.x=(int)vpoint.x/VIEW_SIZE;
            vpoint.y=(int)vpoint.y/VIEW_SIZE;
            //current_map->view_array[(int)vpoint.y][(int)vpoint.x]=false;
            int x,y,x_max,y_max;
            // ustaw pole widzenia
            current_map->view_dist = (int)user_object->params->visible_dist/VIEW_SIZE;
            y = vpoint.y-current_map->view_dist;
            if(y<0) y=0;
            y_max = vpoint.y+current_map->view_dist;
            if(y_max>=current_map->view_array_y)  y_max = current_map->view_array_y-1;
            for(;y<=y_max;y++)
            {
                x = vpoint.x-current_map->view_dist;
                if(x<0) x=0;
                x_max = vpoint.x+current_map->view_dist;
                if(x_max>=current_map->view_array_x)  x_max = current_map->view_array_x-1;
                for(;x<=x_max;x++)
                    current_map->view_array[y][x]=false;
            }
        }

        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  POCZATEK WYSWIETLANIA
        App->Clear(sf::Color(100,200,100));

        /// RYSOWANIE TLA
        //bg.SetBlendMode(sf::Blend::None);
        float xx,yy;
        current_map->bg.SetPosition(0,0);
        for(xx=yy=0;yy<(float)App->GetHeight();yy+=current_map->bg.GetSize().y)
        {
            for(xx=0;xx<(float)App->GetWidth();xx+=current_map->bg.GetSize().x)
            {
                current_map->bg.SetPosition(xx,yy);
                App->Draw(current_map->bg);
            }
        }

        /// RYSOWANIE MAPY
        /// shapes
        int ii;
        for(ii=0;ii<(int)current_map->shapes->size();ii++)
        {
            App->Draw(*(*current_map->shapes)[ii]);
        }

        /// bobjects
        for(ii=0;ii<(int)current_map->bobjects->size();ii++)
        {
            App->Draw(*(*current_map->bobjects)[ii]);
        }

        /// wobjects
        for(ii=0;ii<(int)current_map->wobjects->size();ii++)
        {
            if((*current_map->wobjects)[ii]->show)
            {
                if((*current_map->wobjects)[ii]->inside) (*current_map->wobjects)[ii]->SetFrame(2);
                else    (*current_map->wobjects)[ii]->SetFrame(1);
                App->Draw(*(*current_map->wobjects)[ii]);
                (*current_map->wobjects)[ii]->SetFrame(0);
            }
        }

        /// fobjects
        for(ii=0;ii<(int)current_map->fobjects->size();ii++)
        {
            if((*current_map->fobjects)[ii]->show && ((*current_map->fobjects)[ii]->w_inside==NULL || ((*current_map->fobjects)[ii]->w_inside!=NULL && user_object->w_inside==(*current_map->fobjects)[ii]->w_inside)))
            {
                (*current_map->fobjects)[ii]->Update();
                App->Draw(*(*current_map->fobjects)[ii]);
            }
        }

        /// objects
        for(ii=0;ii<(int)current_map->objects->size();ii++)
        {
            if((*current_map->objects)[ii]->show && ((*current_map->objects)[ii]->w_inside==NULL || (*current_map->objects)[ii]->w_inside->inside))
            {
                (*current_map->objects)[ii]->Update();
                App->Draw(*(*current_map->objects)[ii]);

                /// wyswietlanie pociskow
                Object *obj = (*current_map->objects)[ii];
                //vector<Bullet*> t_shots = obj->params->guns[current_gun]->shots;

                // iteruk po wszsytkich broniach
                for(int current_gun=0;current_gun<(int)obj->params->guns.size();current_gun++)
                {
                    if(obj->params->guns[current_gun]->shots.size()>0)
                    {
                        // iteruj po kolejnych strzalach
                        for(int nn=0;nn<(int)obj->params->guns[current_gun]->shots.size();nn++)
                        {
                            int size = obj->params->guns[current_gun]->shots[nn]->vec_shot.size();
                            unsigned int shot_pos = obj->params->guns[current_gun]->shots[nn]->shot_pos;
                            if((int)shot_pos<size)
                            {
        #ifdef _DEBUG
                                    //sf::Shape linex = sf::Shape::Line(obj->params->guns[current_gun]->shots[nn]->vec_shot[shot_pos].x, obj->params->guns[current_gun]->shots[nn]->vec_shot[shot_pos].y,
                                    //        obj->params->guns[current_gun]->shots[nn]->vec_shot[size-1].x, obj->params->guns[current_gun]->shots[nn]->vec_shot[size-1].y, 1, sf::Color::Green,0,sf::Color::Green);
                                    //App->Draw(linex);
        #endif
                                if(obj->params->guns[current_gun]->shots[nn]->show)
                                {
                                    obj->params->guns[current_gun]->shots[nn]->Update();
                                    //obj->params->guns[current_gun]->shots[nn]->Rotate(22);
                                    // jesli strzelamy fireball
                                    if(obj->params->guns[current_gun]->gun==Object::FIREBALL)
                                    {
                                        for(int xx=1;xx<(int)obj->params->guns[current_gun]->shots[nn]->shot_pos;xx++)
                                        {
                                            int ssize = obj->params->guns[current_gun]->shots[nn]->shot_pos-xx;
                                            if(ssize<=15)
                                            {
                                                obj->params->guns[current_gun]->shots[nn]->SetPosition(obj->params->guns[current_gun]->shots[nn]->vec_shot[xx]);
                                                if(xx%2)    obj->params->guns[current_gun]->shots[nn]->SetScale(1,1);
                                                else        obj->params->guns[current_gun]->shots[nn]->SetScale(0.7,0.7);
                                                App->Draw(*obj->params->guns[current_gun]->shots[nn]);
                                            }
                                        }
                                    }
                                    if(obj->params->guns[current_gun]->gun==Object::LASER)
                                    {
                                        const int laser_length = 30;
                                        for(int xx=1;xx<(int)obj->params->guns[current_gun]->shots[nn]->shot_pos;xx++)
                                        {
                                            //sf::Color col(,,,255);
                                            //obj->params->guns[current_gun]->shots[nn]->SetColor(col);
                                            int ssize = obj->params->guns[current_gun]->shots[nn]->shot_pos-xx;
                                            if(ssize<=laser_length)
                                            {
                                                obj->params->guns[current_gun]->shots[nn]->SetPosition(obj->params->guns[current_gun]->shots[nn]->vec_shot[xx]);
                                                obj->params->guns[current_gun]->shots[nn]->SetScale(0.3,0.3);
                                                App->Draw(*obj->params->guns[current_gun]->shots[nn]);
                                            }
                                        }
                                        int temp_pos = obj->params->guns[current_gun]->shots[nn]->shot_pos-laser_length;
                                        if(temp_pos<0)  temp_pos = 0;
                                        sf::Shape line1 = sf::Shape::Line(obj->params->guns[current_gun]->shots[nn]->vec_shot[obj->params->guns[current_gun]->shots[nn]->shot_pos],
                                            obj->params->guns[current_gun]->shots[nn]->vec_shot[temp_pos], 1, sf::Color(rand()%255,rand()%255,rand()%255),0,sf::Color::Red);
                                        App->Draw(line1);
                                    }
                                    // wszsytkie inne bronie
                                    else
                                        App->Draw(*obj->params->guns[current_gun]->shots[nn]);
                                }
                            }
                        }
                    }
                }

/// lines for help
#ifdef _DEBUG
                sf::Vector2f v1,v2;
                v1.x = (*current_map->objects)[ii]->last_contact.x;
                v1.y = (*current_map->objects)[ii]->last_contact.y;
                v2.x = (*current_map->objects)[ii]->GetPosition().x+((*current_map->objects)[ii]->GetSize().x/2);
                v2.y = (*current_map->objects)[ii]->GetPosition().y+((*current_map->objects)[ii]->GetSize().y/2);
                if(ii>0 && v1.x>-1 && v1.y>-1)
                {
                    sf::Shape line1 = sf::Shape::Line((*current_map->objects)[ii]->last_contact.x, (*current_map->objects)[ii]->last_contact.y,
                                     (*current_map->objects)[ii]->GetPosition().x, (*current_map->objects)[ii]->GetPosition().y, 1, sf::Color::Green,0,sf::Color::Green);
                    sf::Shape line2 = sf::Shape::Line((*current_map->objects)[ii]->last_contact.x+((*current_map->objects)[0]->GetSize().x/2), (*current_map->objects)[ii]->last_contact.y+((*current_map->objects)[0]->GetSize().y/2),
                                     (*current_map->objects)[ii]->GetPosition().x+((*current_map->objects)[ii]->GetSize().x/2), (*current_map->objects)[ii]->GetPosition().y+((*current_map->objects)[ii]->GetSize().y/2), 1, sf::Color::Red,0,sf::Color::Red);
                    sf::Shape line3 = sf::Shape::Line((*current_map->objects)[ii]->last_contact.x+((*current_map->objects)[0]->GetSize().x), (*current_map->objects)[ii]->last_contact.y+((*current_map->objects)[0]->GetSize().y),
                                     (*current_map->objects)[ii]->GetPosition().x+((*current_map->objects)[ii]->GetSize().x), (*current_map->objects)[ii]->GetPosition().y+((*current_map->objects)[ii]->GetSize().y), 1, sf::Color::Blue,0,sf::Color::Blue);
                    sf::Shape line4 = sf::Shape::Line((*current_map->objects)[ii]->last_contact.x+((*current_map->objects)[0]->GetSize().x), (*current_map->objects)[ii]->last_contact.y,
                                     (*current_map->objects)[ii]->GetPosition().x+((*current_map->objects)[ii]->GetSize().x), (*current_map->objects)[ii]->GetPosition().y, 1, sf::Color::Yellow,0,sf::Color::Yellow);
                    sf::Shape line5 = sf::Shape::Line((*current_map->objects)[ii]->last_contact.x, (*current_map->objects)[ii]->last_contact.y+((*current_map->objects)[0]->GetSize().y),
                                     (*current_map->objects)[ii]->GetPosition().x, (*current_map->objects)[ii]->GetPosition().y+((*current_map->objects)[ii]->GetSize().y), 1, sf::Color::White,0,sf::Color::White);
                   // App->Draw(line1);
            //App->Draw(line2);
                   // App->Draw(line3);
                   // App->Draw(line4);
                   // App->Draw(line5);
                }
#endif
                // tekst nad glowami
                ostringstream ss;
                string str;
                ss << obj->params->life;
                str = ss.str();
                sf::String tekst;
                tekst.SetSize(12);
                tekst.SetText(str);
                tekst.SetPosition(obj->GetPosition().x,obj->GetPosition().y-16);
                App->Draw(tekst);
            }
        }

        /// wyswietlanie obszaru zakrywajacego widocznosc
        if(current_map->view_switch)
        {
            for(int view_y=0;view_y<current_map->view_array_y;view_y++)
                for(int view_x=0;view_x<current_map->view_array_x;view_x++)
                    if(current_map->view_array[view_y][view_x])
                    {
                        sprite_view.SetPosition(view_x*VIEW_SIZE, view_y*VIEW_SIZE);
                        App->Draw(sprite_view);
                    }
        }

        /// rysowanie kursora
        this->cursor.SetPosition(steerage.GetMouseX()-this->cursor.GetSize().x/2, steerage.GetMouseY()-this->cursor.GetSize().y/2);
        App->Draw(this->cursor);

        /// rysowanie tekstu
        ostringstream ss;
        string str;
        ss << "Gra v" << GAME_VERSION << ", LIFE: " << user_object->params->life << ", ZL: "<< user_object->params->gold << ", AMMO: "<<user_object->params->guns[obj->params->active_gun]->ammo;
        ss << ", speed(1,2): " << user_object->params->move_speed << ", new map(3): " << maps->size() << ", cover on/off(4): "<<current_map->view_switch;
        str = ss.str();

        sf::String tekst;
        tekst.SetSize(15);
        tekst.SetText(str);
        //tekst.SetSize(tekst.GetSize()/2);
       // tekst.SetFont( mojaCzcionka );
        App->Draw(tekst);

        /// wyswietl na ekran
        App->Display();
    }
}

// adr - jesli nie rozny od NULL oznacza to ze jest to adres obiektu ktory wypuscil pocisk object
void* Sfgfx::ObjectCollision(AniSprite *object, Map *map, void *adr)
{
    unsigned int compare_frame;
    sf::Vector2f vpoint;
    size_t ii;

    // wszelkie sprawdzanie kolizji pixelowej na podstawie 8 klatki z pliku png
    compare_frame = 8;
    // gdy sprawdzamy kolizje z pociskami niech to bedzie klatka nr 0
    compare_frame = 0;

    // przechodzenie miedzy mapami
    if(object==user_object)
    {
        vpoint = object->GetPosition();
        bool change_map = false;
        matrix_last_pos = matrix_pos;
        if(vpoint.x<=0)
        {
            if(matrix_pos.x>0)
            {
                matrix_pos.x--;
                vpoint.x = App->GetWidth()-object->GetSize().x-1;
                change_map = true;
            }
            else    return object;
        }
        else if(vpoint.y<=0)
        {
            if(matrix_pos.y>0)
            {
                matrix_pos.y--;
                vpoint.y = App->GetHeight()-object->GetSize().y-1;
                change_map = true;
            }
            else    return object;
        }
        else if((vpoint.x+object->GetSize().x)>=App->GetWidth())
        {
            if(matrix_pos.x<MATRIX_SIZE-1)
            {
                matrix_pos.x++;
                vpoint.x = 1;
                change_map = true;
            }
            else    return object;
        }
        else if((vpoint.y+object->GetSize().y)>=App->GetHeight())
        {
            if(matrix_pos.y<MATRIX_SIZE-1)
            {
                matrix_pos.y++;
                vpoint.y = 1;
                change_map = true;
            }
            else    return object;
        }
        last_map = current_map;
        if(change_map && matrix[matrix_pos.y][matrix_pos.x]==NULL)
        {
            GenMap();
            matrix[matrix_pos.y][matrix_pos.x] = current_map;
            (*current_map->objects)[0]->SetPosition(vpoint);
            (*current_map->objects)[0]->last_position = vpoint;
        }
        else if(change_map)
        {
            current_map = matrix[matrix_pos.y][matrix_pos.x];
            user_object = (*current_map->objects)[0];
            user_object->params = engine->params;
            (*current_map->objects)[0]->SetPosition(vpoint);
            (*current_map->objects)[0]->last_position = vpoint;
        }
        if(change_map)
        {
            while((size_t)user_object->params->guns[user_object->params->active_gun]->shots.size()>0)
            {
                delete user_object->params->guns[user_object->params->active_gun]->shots[0];
                user_object->params->guns[user_object->params->active_gun]->shots.erase(user_object->params->guns[user_object->params->active_gun]->shots.begin());// usun strzal z pamieci
            }
        }
    }
    else
    {
        vpoint = object->GetPosition();
        if(vpoint.x<=0 || vpoint.y<=0 || (vpoint.x+object->GetSize().x)>=App->GetWidth() || (vpoint.y+object->GetSize().y)>=App->GetHeight())
            return object;
    }


    int curr_object_frame = object->currentFrame;
    object->SetFrame(compare_frame);    // tymczasowo ustaw na jedna konkretna klatke w celu sprawdzania kolizji pixelami
    Collision cc;
    sf::IntRect r1(object->GetPosition().x, object->GetPosition().y, object->GetPosition().x+object->GetSize().x,object->GetPosition().y+object->GetSize().y);
    sf::IntRect r2;

    for(ii=0;ii<current_map->wobjects->size();ii++)
    {
        // sprawdzanie kolizji pixelowej tylko jesli badany obiekt jest glownym bohaterem lub gdy badamy pocisk i znajdujemy sie w srodku jakiegos pomieszczenia
        if(object==user_object || (adr!=NULL && user_object->w_inside!=NULL))
        {
            if(cc.PixelPerfectTest(*object,*(*current_map->wobjects)[ii]))
                return (*current_map->wobjects)[ii];
        }
        else
        {
            sf::IntRect r2((*current_map->wobjects)[ii]->GetPosition().x, (*current_map->wobjects)[ii]->GetPosition().y, (*current_map->wobjects)[ii]->GetPosition().x+(*current_map->wobjects)[ii]->GetSize().x, (*current_map->wobjects)[ii]->GetPosition().y+(*current_map->wobjects)[ii]->GetSize().y);
            if(r2.Intersects(r1))
                return (*current_map->wobjects)[ii];
        }
    }

    for(ii=0;ii<current_map->objects->size();ii++)
    {
        if((*current_map->objects)[ii]->status==DEAD)
            continue;
        // jesli adr jest glownym bohaterem i sprawdzamy aktualenie jego wlasny pocisk - pomin spr kolizji :)
        if(adr!=NULL && adr==(*current_map->objects)[ii])
            continue;
        if(object!=(*current_map->objects)[ii])
        {
            /*
            if(cc.PixelPerfectTest(*object,*(*current_map->objects)[ii]))
                return (*current_map->objects)[ii];
            */
            sf::IntRect r2((*current_map->objects)[ii]->GetPosition().x, (*current_map->objects)[ii]->GetPosition().y, (*current_map->objects)[ii]->GetPosition().x+(*current_map->objects)[ii]->GetSize().x, (*current_map->objects)[ii]->GetPosition().y+(*current_map->objects)[ii]->GetSize().y);
            if(r2.Intersects(r1))
                return (*current_map->objects)[ii];
        }
        //if(cc.PixelPerfectTest(*object,*(*current_map->objects)[ii]) && object!=(*current_map->objects)[ii])
        //    return true;
    }

    for(ii=0;ii<current_map->bobjects->size();ii++)
    {
        if(object!=user_object)
        {
            sf::IntRect r2((*current_map->bobjects)[ii]->GetPosition().x, (*current_map->bobjects)[ii]->GetPosition().y, (*current_map->bobjects)[ii]->GetPosition().x+(*current_map->bobjects)[ii]->GetSize().x, (*current_map->bobjects)[ii]->GetPosition().y+(*current_map->bobjects)[ii]->GetSize().y);
            if(r2.Intersects(r1))
                return (*current_map->bobjects)[ii];
        }
        else if(cc.PixelPerfectTest(*object,*(*current_map->bobjects)[ii]) && (*current_map->bobjects)[ii]->wall)
            return (*current_map->bobjects)[ii];
    }

    for(ii=0;ii<current_map->fobjects->size();ii++)
    {
        if(object==user_object && (*current_map->fobjects)[ii]->show)
        {
            if(cc.PixelPerfectTest(*object,*(*current_map->fobjects)[ii]))
            {
                // apteczke zabieramy tylko jak nasze zycie jest ponizej 100%
                if((*current_map->fobjects)[ii]->imgs_name!=Imgs::IMG_HEALTH1 || (user_object->params->life<100 && (*current_map->fobjects)[ii]->imgs_name==Imgs::IMG_HEALTH1))
                {
                    (*current_map->fobjects)[ii]->show = false;
                    snds->Get(Sndbuff::SND_FTAKE)->Play();
                }
                switch((*current_map->fobjects)[ii]->imgs_name)
                {
                    case Imgs::IMG_GOLD1: user_object->params->gold++; break;
                    case Imgs::IMG_DIA1: user_object->params->gold+=2; break;
                    case Imgs::IMG_AMMO1: user_object->params->guns[user_object->params->active_gun]->ammo+=5; break;
                    case Imgs::IMG_HEALTH1: user_object->params->life+=25; if(user_object->params->life>100) user_object->params->life=100; break;
                    default: ;
                }
            }
        }
    }

    object->SetFrame(curr_object_frame);

/*
    /// kolizja shape tutaj zaczyna
    size_t nn,kk;
    unsigned int shape_wsp_points;
    vpoint.x += (int)object->GetSize().x/2;
    vpoint.y += (int)object->GetSize().y/2;
    nn = (int)object->GetSize().x+object->GetSize().y/5;
    sf::Shape user_circle = sf::Shape::Circle(vpoint.x,vpoint.y,nn,sf::Color::Blue,1,sf::Color::White);
    size_t npoints = user_circle.GetNbPoints();

    for(ii=0;ii<(size_t)map->shapes->size();ii++)
    {
        shape_wsp_points = (*map->shapes)[ii]->GetNbPoints();
        int shape_wsp[shape_wsp_points*2];
        for(nn=kk=0;nn<shape_wsp_points;nn++)
        {
            vpoint = (*map->shapes)[ii]->GetPointPosition(nn);
            shape_wsp[kk] = vpoint.x;
            shape_wsp[kk+1] = vpoint.y;
            kk+=2;
        }

        Polygon::Polygon pc(shape_wsp_points,shape_wsp);
        //App->Draw(user_circle);
        for(nn=0;nn<npoints;nn+=(int)(npoints/12))
        {
            vpoint = user_circle.GetPointPosition(nn);
            if(pc.oblicz((int)vpoint.x,(int)vpoint.y))
            {
                //cout << (int)vpoint.x << "," << (int)vpoint.y << endl;
                //App->Close();
                return true;
            }
        }
    }
*/
    return NULL;
}

Map* Sfgfx::GenMap(bool main)
{
    Map *map;

    // odkomentowac jesli nie chchemy zachowywac historii map
    //if(current_map!=NULL)
    //    delete current_map;
    map = new Map(imgs,(unsigned int)App->GetWidth(),(unsigned int)App->GetHeight());
    // zakomentowac ponizsza linie jesli nie chcemy przechowywac historii map
    maps->push_back(map);

    current_map = map;
    user_object = (*map->objects)[0];
    //user_object->params->move_speed = MEDIUM_SPEED; // glowny bohater niech bedzie szybszy - tymczasowo ;)
    // przekazanie parametrow glownego bohatera miedzy poziomami
    if(engine->params==NULL)    engine->params = user_object->params;
    else                        user_object->params = engine->params;

    if(matrix[matrix_pos.y][matrix_pos.x]==NULL)
        matrix[matrix_pos.y][matrix_pos.x] = current_map;

    return map;
}

void Sfgfx::Shot(Object *obj,  int x, int y)
{
    obj->params->guns[obj->params->active_gun]->shot_clock = obj->params->guns[obj->params->active_gun]->clock.GetElapsedTime();
    if((1+this->game_speed)*obj->params->guns[obj->params->active_gun]->shot_clock > obj->params->guns[obj->params->active_gun]->shot_time_allowed)
    {
        obj->params->guns[obj->params->active_gun]->clock.Reset();
        if(obj->params->guns[obj->params->active_gun]->ammo>0)
        {
            snds->Get(Sndbuff::SND_THROW)->Play();

            Bullet *b;
            switch(obj->params->guns[obj->params->active_gun]->gun)
            {
                case Object::BASIC: b = new Bullet(imgs,Imgs::IMG_GUN1); break;
                case Object::STONE: b = new Bullet(imgs,Imgs::IMG_GUN2); b->shot_speed = 2; break;  // ustaw szybkosc pociskow - kamien wolniej np
                case Object::FIREBALL: b = new Bullet(imgs,Imgs::IMG_GUN3); b->shot_speed = 2; break;
                case Object::LASER: b = new Bullet(imgs,Imgs::IMG_GUN4); b->shot_speed = 10; break;
                default: b = new Bullet(imgs,Imgs::IMG_GUN1); break;
            }
            alg->SectionCalc(obj->GetPosition().x+(int)obj->GetSize().x/2,obj->GetPosition().y+(int)obj->GetSize().y/2, x, y, 1, &b->vec_shot); // oblicz wspolrzedne pocisku
            obj->params->guns[obj->params->active_gun]->shots.push_back(b);                                  // przypisz pocisk do broni z plecaka user_object
            obj->params->guns[obj->params->active_gun]->ammo--;
        }
    }
}

Sfgfx::~Sfgfx()
{
    delete App;
    delete imgs;

    for(size_t ii=0;ii<(size_t)maps->size();ii++)
        delete (*maps)[ii];
    delete maps;
    delete ini;
    delete engine;
    delete alg;
//    for(int vv=0;vv<(int)v_temp->size();vv++)
//        delete (*v_temp)[vv];
//    delete v_temp;
}

