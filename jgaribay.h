#ifndef _JGARIBAY_H_
#define _JGARIBAY_H_

#include "images.h"

class Texture {
    public:
        Image *image;
        GLuint texture;
        float xc[2];
        float yc[2];
};

class Roadrunner : public Texture {
    public:
        float pos[2];
        float last_pos[2];
        float vel[2];
        int dim[2];
        Roadrunner();
};

class Platform : public Texture {
    public:
        int dim[2];
        int pos[2];
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

void joshua_main();
void joshua_init_opengl();
void joshua_physics(int, int);
void joshua_render(int, int);

//temporarily added functions from jchicas.cpp
void render_main_menu(int, int); 
Image background_time();
#endif
