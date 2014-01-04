#include "Imgs.hpp"

Imgs::Imgs()
{
    img_object = new vector<sf::Image*>;
    ReadIt((size_t)sizeof(img_object_names)/sizeof(string),img_object_names,img_object);
    img_wobject = new vector<sf::Image*>;
    ReadIt((size_t)sizeof(img_wobject_names)/sizeof(string),img_wobject_names,img_wobject);
    img_fobject = new vector<sf::Image*>;
    ReadIt((size_t)sizeof(img_fobject_names)/sizeof(string),img_fobject_names,img_fobject);
    img_bobject = new vector<sf::Image*>;
    ReadIt((size_t)sizeof(img_bobject_names)/sizeof(string),img_bobject_names,img_bobject);
    img_guns = new vector<sf::Image*>;
    ReadIt((size_t)sizeof(img_guns_names)/sizeof(string),img_guns_names,img_guns);
    img_bg = new vector<sf::Image*>;
    ReadIt((size_t)sizeof(img_bg_names)/sizeof(string),img_bg_names,img_bg);
    img_cursors = new vector<sf::Image*>;
    ReadIt((size_t)sizeof(img_cursors_names)/sizeof(string),img_cursors_names,img_cursors);
}

int Imgs::ReadIt(size_t length, const string *names, vector<sf::Image*> *image)
{
    size_t ii;
    //image = new vector<sf::Image*>;
    for(ii=0;ii<length;ii++)
    {
        sf::Image *img = new sf::Image;
        string temp = DIR_RES+names[ii];
        img->LoadFromFile(temp.c_str());
        img->SetSmooth(false);
        image->push_back(img);
    }
    return ii;
}

size_t Imgs::Get2(enum ImgGroup g)
{
    switch(g)
    {
        case IMG_OBJECT: return rand()%img_object->size(); break;
        case IMG_WOBJECT: return rand()%img_wobject->size(); break;
        case IMG_FOBJECT: return rand()%img_fobject->size(); break;
        case IMG_BOBJECT: return rand()%img_bobject->size(); break;
        case IMG_GUN: return rand()%img_guns->size(); break;
        case IMG_BG: return rand()%img_bg->size(); break;
        case IMG_CURSORS: return rand()%img_cursors->size(); break;
    }
    return 0;
}

sf::Image* Imgs::Get(enum ImgGroup g, enum ImgName n)
{
    switch(g)
    {
        case IMG_OBJECT: if(n==IMG_RANDOM) return (*img_object)[rand()%img_object->size()]; else return (*img_object)[n]; break;
        case IMG_WOBJECT: if(n==IMG_RANDOM) return (*img_wobject)[rand()%img_wobject->size()]; else return (*img_wobject)[n]; break;
        case IMG_FOBJECT: if(n==IMG_RANDOM) return (*img_fobject)[rand()%img_fobject->size()]; else return (*img_fobject)[n]; break;
        case IMG_BOBJECT: if(n==IMG_RANDOM) return (*img_bobject)[rand()%img_bobject->size()]; else return (*img_bobject)[n]; break;
        case IMG_GUN: if(n==IMG_RANDOM) return (*img_guns)[rand()%img_guns->size()]; else return (*img_guns)[n]; break;
        case IMG_BG: if(n==IMG_RANDOM) return (*img_bg)[rand()%img_bg->size()]; else return (*img_bg)[n]; break;
        case IMG_CURSORS: if(n==IMG_RANDOM) return (*img_cursors)[rand()%img_cursors->size()]; else return (*img_cursors)[n]; break;
    }
    return (*img_object)[0];
}

Imgs::~Imgs()
{
    DeleteIt(img_object);
    DeleteIt(img_wobject);
    DeleteIt(img_fobject);
    DeleteIt(img_bobject);
    DeleteIt(img_guns);
    DeleteIt(img_bg);
    DeleteIt(img_cursors);
}

void Imgs::DeleteIt(vector<sf::Image*> *image)
{
    for(size_t ii=0;ii<(size_t)image->size();ii++)
        delete (*image)[ii];
    delete image;
}
