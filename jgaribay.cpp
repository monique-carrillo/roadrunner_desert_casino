/*
 * name: joshua garibay
 * date: fall 2024
 * purpose: 3350 personal source file
 */

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include "images.h"
#include "fonts.h"
#include "jgaribay.h"

int joshua_features = 0;
int joshua_move = 0;
const float GRAVITY = -0.1;

GLuint textureID;

Image jimg[3] = {
    "./images/roadrunner.jpg",
    "./images/desert.jpg",
    "./images/rrsprite.jpg"
};

Roadrunner::Roadrunner() {
    pos[0] = 100;
    pos[1] = 300;
    vel[0] = vel[1] = 0.0f;
    dim[0] = 50;
    dim[1] = 100;
};

Platform::Platform() {
    dim[0] = 500;
    dim[1] = 150;
    pos[0] = 0;
    pos[1] = 0;
}
JGlobal jg;

void joshua_main()
{
}

void joshua_init_opengl()
{
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    // generate and bind a new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // build new set of data
    unsigned char *imgData = buildAlphaData(&jimg[0]);

    // load and speciy texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jimg[0].width, jimg[0].height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, imgData);

    free(imgData);

    //--------------------------------------------------------------
    // BACKGROUND
    jg.bg.bgImage = &jimg[1];
    glGenTextures(1, &jg.bg.bgTexture);
    glBindTexture(GL_TEXTURE_2D, jg.bg.bgTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, jg.bg.bgImage->width,
            jg.bg.bgImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
            jg.bg.bgImage->data);
    jg.bg.xc[0] = 0.0;
    jg.bg.xc[1] = 0.25;
    jg.bg.yc[0] = 0.0;
    jg.bg.yc[1] = 1.0;

    glEnable(GL_BLEND);
}

void joshua_physics()
{
    jg.bg.xc[0] += 0.001;
    jg.bg.xc[1] += 0.001;

    jg.rr.vel[1] += GRAVITY;
    // always saving last position
    jg.rr.last_pos[0] = jg.rr.pos[0];
    jg.rr.last_pos[1] = jg.rr.pos[1];
    jg.rr.pos[0] += jg.rr.vel[0];
    jg.rr.pos[1] += jg.rr.vel[1];

    // collision detection
    if (jg.rr.pos[1] < jg.pf.pos[1] + jg.pf.dim[1] &&
        jg.rr.pos[1] > jg.pf.pos[1] - jg.pf.dim[1] &&
        jg.rr.pos[0] > jg.pf.pos[0] - jg.pf.dim[0] &&
        jg.rr.pos[0] < jg.pf.pos[0] + jg.pf.dim[0]) {
        // collision state
        jg.rr.pos[1] = jg.rr.last_pos[1];
        jg.rr.vel[1] = -jg.rr.vel[1] * 0.5;
    }

    if (joshua_move)
        jg.rr.pos[0]++;
}

void joshua_render(int x, int y)
{
    int speed = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_2D);

    // background
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glBindTexture(GL_TEXTURE_2D, jg.bg.bgTexture);
    glBegin(GL_QUADS);        
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[0]); glVertex2i(0, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[0]); glVertex2i(x, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[1]); glVertex2i(x, 0);
    glEnd();

    /*
    // ROADRUNNER IMAGE    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);        
        glTexCoord2f(0.0f, 1.0f); glVertex2i(100, 100);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x - 100, 100);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x - 100, y - 100);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(100, y - 100);
    glEnd();
    */

    Rect r;
    unsigned int c = 0x00ffffff;
    r.bot = 20;
    r.left = 20;
    r.center = 0;
    for (int i = 2; i >= 0; i--) {
        ggprint8b(&r, -16, c, "Roadrunner %i speed: %i", i + 1, speed);
    }

    /*
    Rect s;
    c = 0x00003594;
    s.bot = y - 100;
    s.left = x / 2;
    s.center = 1;
    ggprint40(&s, 16, c, "My Game Title");
    */

    glDisable(GL_TEXTURE_2D);

    // drawing platform
    glPushMatrix();
    glTranslatef(jg.pf.pos[0], jg.pf.pos[1], 0.0f);
    glBegin(GL_QUADS);
        glColor3ub(0, 255, 255);
        glVertex2i(-jg.pf.dim[0], -jg.pf.dim[1]);
        glVertex2i(-jg.pf.dim[0],  jg.pf.dim[1]);
        glVertex2i( jg.pf.dim[0],  jg.pf.dim[1]);
        glVertex2i( jg.pf.dim[0], -jg.pf.dim[1]);
    glEnd();
    glPopMatrix();

    // drawing roadrunner
    glPushMatrix();
    glTranslatef(jg.rr.pos[0], jg.rr.pos[1], 0.0f);
    glBegin(GL_QUADS);
        glColor3ub(0, 53, 148);
        glVertex2i(-jg.rr.dim[0], -jg.rr.dim[1]);
        glVertex2i(-jg.rr.dim[0],  jg.rr.dim[1]);
        glVertex2i( jg.rr.dim[0],  jg.rr.dim[1]);
        glVertex2i( jg.rr.dim[0], -jg.rr.dim[1]);
    glEnd();
    glPopMatrix();
}
