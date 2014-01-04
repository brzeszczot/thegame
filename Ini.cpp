#include "Ini.hpp"

Ini::Ini(string file_name_in)
{
    file_name = file_name_in;

    ini["res_x"] = 800;
    ini["res_y"] = 600;
    ini["color_depth"] = 32;
    ini["gfx_mode"] = 1; // 0: GFX_AUTODETECT, 1: GFX_AUTODETECT_FULLSCREEN, 2: GFX_WINDOWED, 3: GFX_WINDOWED

    read();
    if(!save()) throw(411);
}

Ini::~Ini()
{

}

short int Ini::get(const string name)
{
     if(ini.count(name)>0)  return ini[name];
     else return 0;
}

bool Ini::set(const string name, short int value)
{
    if(ini.count(name)>0)
    {
        ini[name] = value;
        return true;
    }
    else return false;
}

bool Ini::read()
{
    ifstream file;
    file.open(file_name.c_str(),ios::in);

    if(file.is_open())
    {
        string n,v;
        while(!file.eof())
        {
            file >> n >> v;
            if(ini.count(n)>0)
                ini[n] = atoi(v.c_str());
        }
        file.close();
        return true;
    }
    else return false;
}

bool Ini::save()
{
    ofstream file;
    file.open(file_name.c_str(),ios::out);

    if(file.is_open())
    {
        for(it=ini.begin();it!=ini.end();it++)
            file << (*it).first << "\t" << (*it).second << "\n";
        file.close();
        return true;
    }
    else return false;
}

