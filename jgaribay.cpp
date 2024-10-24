/*
 * name: joshua garibay
 * date: fall 2024
 * purpose: 3350 personal source file
 */

#include "fonts.h"
#include "jgaribay.h"
#include <GL/glx.h>
#include <X11/Xlib.h>

int joshua_show = 0;

void show_joshua_feature(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 28, 0x0000ffff, "joshua feature testing print");
}

void temp_title_screen(int x, int y)
{
    unsigned int color = 0x00ff88cc;
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1; // set to 1 to center text
    ggprint16(&r, 28, color, "Temporary Start Screen");
}

void start_game(int x, int y)
{
    // draw a basic rectangle
    glPushMatrix();
    glBegin(GL_QUADS);
        glColor3ub(255, 0, 0);
        glVertex2f(-x, -y);
        glVertex2f(-x,  y);
        glVertex2f( x,  y);
        glVertex2f( x, -y);
    glEnd();
    glPopMatrix();
}
