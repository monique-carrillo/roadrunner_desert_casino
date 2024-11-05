#ifndef _JGARIBAY_H_
#define _JGARIBAY_H_

extern int joshua_features;

class Roadrunner {
    public:
        float pos[2];
        float vel[2];
        int w, h;
        Roadrunner();
};

void joshua_main();
void joshua_init_opengl();
void joshua_physics();
void joshua_render(int, int);

//temporarily added functions from jchicas.cpp
void render_main_menu(int, int); 

#endif
