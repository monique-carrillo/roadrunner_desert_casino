/*
 *    name: joshua garibay
 *    date: fall 2024
 * purpose: 3350 personal source file
 */

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include "images.h"
#include "fonts.h"
#include "jgaribay.h"

Image jimg[4] = {
    "./images/roadrunner.jpg",
    "./images/desert.jpg",
    "./images/rrsprite.jpg",
    "./images/platform.jpg"
};

Roadrunner::Roadrunner() {
    pos[0] = 100;
    pos[1] = 300;
    vel[0] = vel[1] = 0.0f;
    dim[0] = 10; // width
    dim[1] = 20; // height
};

Platform::Platform() {
    dim[0] = 650;
    dim[1] = 175;
    pos[0] = 0;
    pos[1] = 0;
}

int joshua_features = 0;
const float GRAVITY = -0.1;
JGlobal jg;

void joshua_main()
{
}

void joshua_init_opengl()
{
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    // ROADRUNNER
    jg.rrtext.image = &jimg[0];
    glGenTextures(1, &jg.rrtext.texture); // generate & bind texture ID
    glBindTexture(GL_TEXTURE_2D, jg.rrtext.texture);
    // texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *imgData = buildAlphaData(&jimg[0]); // build new set of data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jimg[0].width, jimg[0].height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, imgData); // load & specify texture data
    free(imgData);

    // BACKGROUND
    jg.bg.image = &jimg[1];
    glGenTextures(1, &jg.bg.texture);
    glBindTexture(GL_TEXTURE_2D, jg.bg.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, jg.bg.image->width,
            jg.bg.image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
            jg.bg.image->data);
    jg.bg.xc[0] = 0.0;
    jg.bg.xc[1] = 0.25;
    jg.bg.yc[0] = 0.0;
    jg.bg.yc[1] = 1.0;

    // PLATFORM
    jg.pf.image = &jimg[3];
    glGenTextures(1, &jg.pf.texture);
    glBindTexture(GL_TEXTURE_2D, jg.pf.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, jg.pf.image->width,
            jg.pf.image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
            jg.pf.image->data);
    jg.pf.xc[0] = 0.0;
    jg.pf.xc[1] = 0.25;
    jg.pf.yc[0] = 0.0;
    jg.pf.yc[1] = 1.0;


    glEnable(GL_BLEND);
}

void joshua_physics()
{
    if (jg.rr.pos[0] != jg.rr.last_pos[0]) {
        // moving the background
        jg.bg.xc[0] += 0.0001;
        jg.bg.xc[1] += 0.0001;
        // moving the platform
        jg.pf.xc[0] += 0.001;
        jg.pf.xc[1] += 0.001;
    }

    jg.rr.vel[1] += GRAVITY;
    // always saving last position
    jg.rr.last_pos[0] = jg.rr.pos[0];
    jg.rr.last_pos[1] = jg.rr.pos[1];
    jg.rr.pos[0] += jg.rr.vel[0];
    jg.rr.pos[1] += jg.rr.vel[1];

    // collision detection
    if (jg.rr.pos[1] - jg.rr.dim[1] < jg.pf.pos[1] + jg.pf.dim[1] &&
        jg.rr.pos[1] + jg.rr.dim[1] > jg.pf.pos[1] - jg.pf.dim[1] &&
        jg.rr.pos[0] + jg.rr.dim[0] > jg.pf.pos[0] - jg.pf.dim[0] &&
        jg.rr.pos[0] - jg.rr.dim[0] < jg.pf.pos[0] + jg.pf.dim[0]) {
        // collision state
        jg.rr.pos[1] = jg.rr.last_pos[1];
        jg.rr.vel[1] = -jg.rr.vel[1] * 0.5;
    }
    jg.rr.pos[0]++;
}

void joshua_render(int x, int y)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    // BACKGROUND
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glBindTexture(GL_TEXTURE_2D, jg.bg.texture);
    glBegin(GL_QUADS);        
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[0]); glVertex2i(0, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[0]); glVertex2i(x, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[1]); glVertex2i(x, 0);
    glEnd();

    // PLATFORM
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, jg.pf.texture);
    glBegin(GL_QUADS);        
        glTexCoord2f(jg.pf.xc[0], jg.pf.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(jg.pf.xc[0], jg.pf.yc[0]); glVertex2i(0, jg.pf.dim[1]);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[0]); glVertex2i(jg.pf.dim[0],
                                                            jg.pf.dim[1]);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[1]); glVertex2i(jg.pf.dim[0], 0);
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
        ggprint8b(&r, -16, c, "Roadrunner %i speed: %f", i + 1, jg.rr.vel[0]);
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

    // ROADRUNNER
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
