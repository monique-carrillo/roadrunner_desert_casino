#ifndef _JGARIBAY_H_
#define _JGARIBAY_H_

#include "images.h"

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
        Roadrunner();
};

class Platform : public Texture {
    public:
        Platform();
};

class JGlobal {
    public:
        int walkframe;
        Texture bg;
        Texture logo;
        Texture rrsprite;
        Roadrunner rr[3];
        Platform pf;
        JGlobal();
};

extern int joshua_features;
extern const float GRAVITY;

void joshua_init_opengl();
void joshua_physics(int, int);
void joshua_render(int, int, int);

//temporarily added functions from jchicas.cpp
void render_main_menu(int, int, int); 
Image background_time();
#endif
