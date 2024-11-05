#ifndef _JGARIBAY_H_
#define _JGARIBAY_H_

#include "images.h"

extern int joshua_features;
extern int joshua_move;
extern const float GRAVITY;

class Texture {
    public:
        Image *bgImage;
        GLuint bgTexture;
        float xc[2];
        float yc[2];
};

class Roadrunner {
    public:
        float pos[2];
        float last_pos[2];
        float vel[2];
        int dim[2];
        Roadrunner();
};

class Platform {
    public:
        int dim[2];
        int pos[2];
        Platform();
};

class JGlobal {
    public:
        Texture bg;
        Roadrunner rr;
        Platform pf;
};

void joshua_main();
void joshua_init_opengl();
void joshua_physics();
void joshua_render(int, int);

//temporarily added functions from jchicas.cpp
void render_main_menu(int, int); 

#endif
