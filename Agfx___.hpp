#ifndef AGFX_HPP_INCLUDED
#define AGFX_HPP_INCLUDED

#include <allegro.h>
#include <algif.h>
#include <time.h>
#include "Engine.hpp"
#include "Ini.hpp"

#define DIR_RES "res/"

class Agfx
{
    private:
        Engine *engine;
        Ini *ini;
        boost::mutex mutex;
    public:
        Agfx(Engine *engine_in, Ini *ini_in);
        ~Agfx();
        void Run();
};

#endif // AGFX_HPP_INCLUDED
