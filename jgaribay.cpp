/*
 *    name: joshua garibay
 *    date: fall 2024
 * purpose: 3350 personal source file
 */

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <stdio.h>
#include "images.h"
#include "fonts.h"
#include "jgaribay.h"

// roadrunner sprite source:
// https://www.spriters-resource.com/arcade/roadrun/sheet/11658/
Image jimg[4] = {
    "./images/logo.jpg",
    "./images/desert.jpg",
    "./images/roadrunner.jpg",
    "./images/platform.jpg"
};

Roadrunner::Roadrunner() {
    pos[0] = 250;  // cx
    pos[1] = 250;  // cy
    vel[0] = vel[1] = 0.0f;
    dim[0] = 30;  // width
    dim[1] = 40;  // height
};

Platform::Platform() {
    dim[0] = 650;
    dim[1] = 175;
    pos[0] = 0;
    pos[1] = 0;
}

JGlobal::JGlobal() {
    walkframe = 0;
    logo.dim[0] = 100;
    logo.dim[1] = 75;
    logo.pos[0] = 0;
    logo.pos[0] = 0;
}

int joshua_features = 0;
const float GRAVITY = -0.1;
JGlobal jg;

void joshua_init_opengl()
{
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    // LOGO
    jg.logo.image = &jimg[0];
    glGenTextures(1, &jg.logo.texture); // generate & bind texture ID
    glBindTexture(GL_TEXTURE_2D, jg.logo.texture);
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

    // ROADRUNNER SPRITE
    jg.rrsprite.image = &jimg[2];
    glGenTextures(1, &jg.rrsprite.texture);
    glBindTexture(GL_TEXTURE_2D, jg.rrsprite.texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *rrdata = buildAlphaData(&jimg[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jg.rrsprite.image->width,
            jg.rrsprite.image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rrdata);
    free(rrdata);

    glEnable(GL_BLEND);
}

void joshua_physics(int x, int y)
{
    jg.logo.pos[0] = x / 2;
    jg.logo.pos[1] = y - (y / 4);
    
    jg.pf.dim[0] = x;
    jg.pf.dim[1] = y / 3;

    float avgxvel = 0.0;
    for (int i = 0; i < 3; i++) {
        avgxvel += jg.rr[i].vel[0];
    }
    avgxvel /= 3;
    
    // moving the background
    jg.bg.xc[0] += avgxvel * 0.0001;
    jg.bg.xc[1] += avgxvel * 0.0001;
    // moving the platform
    jg.pf.xc[0] += avgxvel * 0.001;
    jg.pf.xc[1] += avgxvel * 0.001;

    for (int i = 0; i < 3; i++) {
        jg.rr[i].vel[1] += GRAVITY;
        // always saving last position
        jg.rr[i].last_pos[0] = jg.rr[i].pos[0];
        jg.rr[i].last_pos[1] = jg.rr[i].pos[1];
        
        if (jg.rr[i].vel[0] > avgxvel)
            jg.rr[i].pos[0] += jg.rr[i].vel[0]; // move x pos
        else
            jg.rr[i].pos[0] -= jg.rr[i].vel[0]; // move x pos
        
        jg.rr[i].pos[1] += jg.rr[i].vel[1]; // move y pos
        // collision detection
        if (jg.rr[i].pos[1] - jg.rr[i].dim[1] < jg.pf.pos[1] + jg.pf.dim[1] &&
            jg.rr[i].pos[1] + jg.rr[i].dim[1] > jg.pf.pos[1] - jg.pf.dim[1] &&
            jg.rr[i].pos[0] + jg.rr[i].dim[0] > jg.pf.pos[0] - jg.pf.dim[0] &&
            jg.rr[i].pos[0] - jg.rr[i].dim[0] < jg.pf.pos[0] + jg.pf.dim[0]) {
            // collision state
            jg.rr[i].pos[1] = jg.rr[i].last_pos[1];
            jg.rr[i].vel[1] = -jg.rr[i].vel[1] * 0.5;
        }
        jg.rr[i].vel[0] += rand() % 2 * 0.1;
    }
    jg.walkframe++;
}

void joshua_render(int x, int y, int status)
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


    // SPRITE
    // source: walk2 framework
    //float cx = x / 2.0;
    //float cy = y / 2.0;
    //float w, h;
    //w = h = 100.0;
    int ix = jg.walkframe % 6;
    int iy = 0;
    if (jg.walkframe >= 6)
        iy = 1;
    float fx = (float)ix / 6.0;
    float fy = (float)iy / 2.0;
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, jg.rrsprite.texture);
        glBegin(GL_QUADS);
            glTexCoord2f(fx + .166, fy + .5);
            glVertex2i(jg.rr[i].pos[0] - jg.rr[i].dim[0],
                    jg.rr[i].pos[1] - jg.rr[i].dim[1]);
            glTexCoord2f(fx + .166, fy);
            glVertex2i(jg.rr[i].pos[0] - jg.rr[i].dim[0],
                    jg.rr[i].pos[1] + jg.rr[i].dim[1]);
            glTexCoord2f(fx, fy);
            glVertex2i(jg.rr[i].pos[0] + jg.rr[i].dim[0],
                    jg.rr[i].pos[1] + jg.rr[i].dim[1]);
            glTexCoord2f(fx, fy + .5);
            glVertex2i(jg.rr[i].pos[0] + jg.rr[i].dim[0],
                    jg.rr[i].pos[1] - jg.rr[i].dim[1]);
        glEnd();
        glPopMatrix();
    }

    Rect r;
    unsigned int c = 0x00003594;
    r.bot = y - 30;
    r.left = 20;
    r.center = 0;
    ggprint13(&r, 16, c, "Leaderboard");
    for (int i = 0; i < 3; i++) {
        ggprint8b(&r, 16, c, "Roadrunner %i speed: %f", i + 1, jg.rr[i].vel[0]);
    }
    
    glDisable(GL_TEXTURE_2D);

    if (status) {
        // paused
        glBegin(GL_QUADS);
            glColor4ub(0, 0, 0, 200);
            glVertex2i(0, 0);
            glVertex2i(0, y);
            glVertex2i(x, y);
            glVertex2i(x, 0);
        glEnd();

        glEnable(GL_TEXTURE_2D);

        // LOGO
        glColor3f(1.0f, 1.0f, 1.0f); // reset color
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, jg.logo.texture);
        glTranslatef(jg.logo.pos[0], jg.logo.pos[1], 0.0f);
        glBegin(GL_QUADS);        
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(-jg.logo.dim[0], -jg.logo.dim[1]); // bottom left
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(-jg.logo.dim[0],  jg.logo.dim[1]); // bottom right
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i( jg.logo.dim[0],  jg.logo.dim[1]); // top right
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i( jg.logo.dim[0], -jg.logo.dim[1]); // top left
        glEnd();
        glPopMatrix();

        Rect s;
        s.bot = y / 2;
        s.left = x / 2;
        s.center = 1;
        ggprint40(&s, 32, 0x00ffc72c, "PAUSED");
        ggprint13(&s, 32, 0x00003594, "enter  - main menu");
        ggprint13(&s, 32, 0x00003594, "esc - unpause");
    }
    /*
    // ROADRUNNER BOX
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(jg.rr[i].pos[0], jg.rr[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
            glColor3ub(0, 53, 148);
            glVertex2i(-jg.rr[i].dim[0], -jg.rr[i].dim[1]);
            glVertex2i(-jg.rr[i].dim[0],  jg.rr[i].dim[1]);
            glVertex2i( jg.rr[i].dim[0],  jg.rr[i].dim[1]);
            glVertex2i( jg.rr[i].dim[0], -jg.rr[i].dim[1]);
        glEnd();
        glPopMatrix();
    }
    */
}
