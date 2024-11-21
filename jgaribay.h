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
    float hcolor[3];
    float dcolor[3];
    unsigned int text_color;
} Button;

#define GRAVITY -0.1

class JGlobal {
    public:
        int outline;
        int walkframe;
        Texture bg;
        Texture logo;
        Texture rrsprite;
        Roadrunner rr[3];
        Platform pf;
        Button button[MAXBUTTONS];
        JGlobal();
};

extern int jg_nbuttons;
extern JGlobal jg;

void joshua_init(int, int);
void joshua_init_opengl();
void joshua_physics();
void render_pause_screen(int, int);
void render_racing(int, int);

//temporarily added functions from jchicas.cpp
void render_main_menu(int, int, int); 
Image background_time();
#endif
