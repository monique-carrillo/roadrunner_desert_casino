#ifndef _JGARIBAY_H_
#define _JGARIBAY_H_

#include "images.h"

#define GRAVITY -0.1
typedef double Flt;

class Texture {
    public:
        Image *image;
        GLuint texture;
        float xc[2];
        float yc[2];
        float x, y;
        float w, h;
};

class Roadrunner : public Texture {
    public:
        float last_x, last_y;
        float vel[2];
        float color[3];
        unsigned int alt_color;
        Roadrunner();
};

class JGlobal {
    public:
        Flt camera[2];
        int xres, yres;
        int frame;
        int win;
        Texture lg, bg, pf, rr_texture, pause;
        Roadrunner rr[3];
        Button button[MAXBUTTONS];
        JGlobal();
};

extern int jg_nbuttons;
extern JGlobal jg;

void joshua_init();
void joshua_init_opengl();
void joshua_physics();
void render_pause_screen();
void render_racing();

#endif
