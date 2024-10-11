//Monique Carrillo
// 10/8/24


#include "fonts.h"

int monique_show = 0;

void show_my_feature(int x, int y)
{
    //draw a rectangle
    //show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x0000ff00, "Monique's Feature");

}
