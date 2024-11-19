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
        float color[3];
        unsigned int alt_color;
        Roadrunner();
};

class Platform : public Texture {
    public:
        Platform();
};

// source: bship.cpp framework
typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} Button;

class JGlobal {
    public:
        int outline;
        int walkframe;
        Texture bg;
        Texture logo;
        Texture rrsprite;
        Roadrunner rr[3];
        Platform pf;
        Button button[5];
        JGlobal();
};

extern int joshua_features;
extern const float GRAVITY;
extern JGlobal jg;

void joshua_init();
void joshua_init_opengl();
void joshua_physics();
void joshua_render_pause(int, int);
void joshua_render_rr();
void joshua_render(int, int, int);

//temporarily added functions from jchicas.cpp
void render_main_menu(int, int, int); 
Image background_time();
#endif
