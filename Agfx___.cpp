#include "Agfx.hpp"

volatile long global_speed;
void increment_speed()
{
    global_speed++;
}
END_OF_FUNCTION(increment_speed);

Agfx::Agfx(Engine *engine_in, Ini *ini_in)
{
    //int autores_x, autores_y;
    ini = ini_in;

	allegro_init();
	algif_init();
	/*
	if(get_desktop_resolution(&autores_x, &autores_y) == 0)
    {
        ini->set("res_x",autores_x);
        ini->set("res_y",autores_y);
        if(!ini->save()) throw(411);
    }
	desktop_color_depth();
	*/
	set_color_depth(ini->get("color_depth"));

    int gfx_mode = 0;
    switch(ini->get("gfx_mode"))
    {
        case 0: gfx_mode = GFX_AUTODETECT; break;
        case 1: gfx_mode = GFX_AUTODETECT_FULLSCREEN; break;
        case 2: gfx_mode = GFX_AUTODETECT_WINDOWED; break;
    }
	set_gfx_mode(gfx_mode, ini->get("res_x"), ini->get("res_y"), 0, 0);

	install_timer();
	install_keyboard();
	install_mouse();
    //enable_hardware_cursor();
    //select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
    //show_mouse(screen);
    //set_mouse_sprite(cursor);
    //unscare_mouse();

    // reset game speed counter
    global_speed = 0;
    LOCK_VARIABLE(global_speed);
    LOCK_FUNCTION(increment_speed);
    install_int_ex(increment_speed, BPS_TO_TIMER(100)); // 100

    // take engine pointer
    engine = engine_in;
}

void Agfx::Run()
{

    char const *name = "res/gif.gif";
   BITMAP *n = load_gif (name, default_palette);







    BITMAP *bufor = NULL;
    BITMAP *ludek = NULL;

    bufor = create_bitmap(ini->get("res_x"), ini->get("res_y"));
    if(!bufor)
    {
        set_gfx_mode(GFX_TEXT,0,0,0,0);
        allegro_message("Creating bufor error!");
        allegro_exit();
    }
    string dir = DIR_RES;
    dir += "ludek.bmp";
    ludek = load_bmp(dir.c_str(), default_palette);
    if(!ludek)
    {
        set_gfx_mode(GFX_TEXT,0,0,0,0);
        allegro_message("Creating bitmap error!");
        allegro_exit();
    }
    int ludek_x = 100, ludek_y = 100;

    while(!key[KEY_ESC])
    {
        while(global_speed>0)
        {
            if(key[KEY_LEFT]) ludek_x--;
            if(key[KEY_RIGHT]) ludek_x++;
            if(key[KEY_UP]) ludek_y--;
            if(key[KEY_DOWN]) ludek_y++;
            global_speed--;
        }

        clear_to_color(bufor, makecol(150,150,150));
        //rest(10);
        textout_ex(bufor, font, engine->str.c_str(), 10, 10, makecol(255,255,255), -1);
       // masked_blit(ludek, bufor, 0, 0, mouse_x+(mouse_b*5), mouse_y, ludek->w, ludek->h);
        masked_blit(n, bufor, 0, 0, mouse_x+(mouse_b*5), mouse_y+(mouse_b*5), n->w, n->h);

        blit(bufor, screen, 0, 0, 0, 0, ini->get("res_x"), ini->get("res_y"));
    }
    remove_int(increment_speed);
    destroy_bitmap(ludek);
    destroy_bitmap(bufor);

    // set engine to stop working
    engine->status = END;
}

Agfx::~Agfx()
{
    clear_keybuf();
	allegro_exit();
}

