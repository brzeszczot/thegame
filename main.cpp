#include "main.hpp"
using namespace std;

int main()
{
    srand(time(0));// za duzo srand bruzdzi kod

    Sfgfx *sfgfx = new Sfgfx();
    sfgfx->Run();

    delete sfgfx;

    return 0;
}
