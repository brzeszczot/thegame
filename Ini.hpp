#ifndef INI_HPP_INCLUDED
#define INI_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>

using namespace std;

class Ini
{
    private:
        string file_name;
        map<string,short int> ini;
        map<string,short int>::iterator it;
    public:
        Ini(string file_name_in);
        ~Ini();
        bool save();
        bool read();
        short int get(const string name);
        bool set(const string name, short int value);
};

#endif // INI_HPP_INCLUDED
