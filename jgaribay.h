#ifndef _JGARIBAY_H_
#define _JGARIBAY_H_

#include "images.h"

#define GRAVITY -0.1
#define COLOR_SIZE 3

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

class Theme {
    public:
        float primary1[3];
        unsigned int primary2;
        float secondary1[3];
        unsigned int secondary2;
        Theme(Theme &);
        Theme(const float *, unsigned int, const float *, unsigned int);
};

class JGlobal {
    public:
        Flt camera[2];
        int xres, yres;
        int frame;
        int win;
        int setup;
        int chosen;
        Theme theme;
        Texture lg, bg, pf, rr_texture, shop;
        Roadrunner rr[3];
        Button button[MAXBUTTONS];
        JGlobal();
};

extern int jg_nbuttons;
extern JGlobal jg;
extern float csub_p1[3];
extern float csub_s1[3];
extern Theme csub;
extern float light_p[3];
extern float light_s[3];
extern Theme light;
extern float dark_p[3];
extern float dark_s[3];
extern Theme dark;
extern float strawberry_p[3];
extern float strawberry_s[3];
extern Theme strawberry;
extern float charcoal_p[3];
extern float charcoal_s[3];
extern Theme charcoal;
extern float mint_p[3];
extern float mint_s[3];
extern Theme mint;
extern float royal_p[3];
extern float royal_s[3];
extern Theme royal;
extern float peach_p[3];
extern float peach_s[3];
extern Theme peach;
extern float slate_p[3];
extern float slate_s[3];
extern Theme slate;

void joshua_init();
void joshua_init_opengl();
void joshua_physics();
void render_pause_screen();
void render_racing();
void render_setup();
void render_shop();

#endif
