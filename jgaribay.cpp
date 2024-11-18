/*
 * name: joshua garibay
 * date: fall 2024
 */

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <string.h>
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
    x = 250;  // cx
    y = 250;  // cy
    vel[0] = vel[1] = 0.0f;
    w = 30;  // width
    h = 40;  // height
};

Platform::Platform() {
    w = 650;
    h = 175;
    x = 0;
    y = 0;
}

Button::Button() {
    r.width = 175;
    r.height = 25;
    r.left = 20;
    r.right = r.left + r.width;;
    r.bot = 100;
    r.top = r.bot + r.height;
    r.centerx = r.left + r.right / 2;
    strcpy(text, "enter - main menu");
    //down = 0;
    //click = 0;
    color[0] = 0.0f;
    color[1] = 1.0f;
    color[2] = 0.0f;
    text_color = 0x00ff0000;
}   

JGlobal::JGlobal() {
    walkframe = 0;
    logo.w = 100;
    logo.h = 75;
    logo.x = 0;
    logo.y = 0;
}

int joshua_features = 0;
const float GRAVITY = -0.1;
JGlobal jg;

void joshua_init()
{
}

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

void joshua_physics()
{
    jg.bg.xc[0] += 0.01;
    jg.bg.xc[1] += 0.01;
    jg.pf.xc[0] += 0.01;
    jg.pf.xc[1] += 0.01;
   
    float avgvel = 0;
    for (int i = 0; i < 3; i++) {
        avgvel += jg.rr[i].vel[0];
    }
    avgvel /= 3;
    

    for (int i = 0; i < 3; i++) {
        //if (jg.rr[i].vel[0] != 0)
            jg.walkframe += 1;

        jg.rr[i].vel[1] += GRAVITY;
        jg.rr[i].last_x = jg.rr[i].x;
        jg.rr[i].last_y = jg.rr[i].y;
        jg.rr[i].vel[0] = (double)rand() / (double)RAND_MAX;
        
        if (jg.rr[i].vel[0] > avgvel)
            jg.rr[i].x += jg.rr[i].vel[0]; // move x pos
        else if (jg.rr[i].vel[0] < avgvel)
            jg.rr[i].x -= jg.rr[i].vel[0]; // move x pos

        jg.rr[i].y += jg.rr[i].vel[1]; // move y pos

        // collision detection
        if (jg.rr[i].y - jg.rr[i].h < jg.pf.y + jg.pf.h &&
            jg.rr[i].y + jg.rr[i].h > jg.pf.y - jg.pf.h &&
            jg.rr[i].x + jg.rr[i].w > jg.pf.x - jg.pf.w &&
            jg.rr[i].x - jg.rr[i].w < jg.pf.x + jg.pf.w) {
            
            // collision happened
            jg.rr[i].y = jg.rr[i].last_y;
            jg.rr[i].vel[1] = -jg.rr[i].vel[1] * 0.5;
        }
    }
}

void joshua_render(int x, int y, int status)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    // POSITION STUFF
    jg.logo.x = x / 2;
    jg.logo.y = y - (y / 4);
    jg.pf.w = x;
    jg.pf.h = y / 3;
    

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
        glTexCoord2f(jg.pf.xc[0], jg.pf.yc[0]); glVertex2i(0, jg.pf.h);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[0]); glVertex2i(jg.pf.w,
                                                            jg.pf.h);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[1]); glVertex2i(jg.pf.w, 0);
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
            switch (i) {
                case 0:
                    glColor4ub(255, 0, 0, 255);
                    break;
                case 1:
                    glColor4ub(0, 255, 0, 255);
                    break;
                case 2:
                    glColor4ub(0, 0, 255, 255);
                    break;
            }
            glTexCoord2f(fx + .166, fy + .5);
            glVertex2i(jg.rr[i].x - jg.rr[i].w, jg.rr[i].y - jg.rr[i].h);
            glTexCoord2f(fx + .166, fy);
            glVertex2i(jg.rr[i].x - jg.rr[i].w, jg.rr[i].y + jg.rr[i].h);
            glTexCoord2f(fx, fy);
            glVertex2i(jg.rr[i].x + jg.rr[i].w, jg.rr[i].y + jg.rr[i].h);
            glTexCoord2f(fx, fy + .5);
            glVertex2i(jg.rr[i].x + jg.rr[i].w, jg.rr[i].y - jg.rr[i].h);
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
        ggprint8b(&r, 16, c, "Roadrunner %i vel: %f", i + 1, jg.rr[i].vel[0]);
    }
    Rect t;
    t.bot = jg.rr[0].y + jg.rr[0].h + 20;
    t.left = jg.rr[0].x;
    t.center = 1;
    ggprint13(&t, 16, 0x00ff0000, "RR 1");
    t.bot = jg.rr[1].y + jg.rr[1].h + 20;
    t.left = jg.rr[1].x;
    ggprint13(&t, 16, 0x0000ff00, "RR 2");
    t.bot = jg.rr[2].y + jg.rr[2].h + 20;
    t.left = jg.rr[2].x;
    ggprint13(&t, 16, 0x000000ff, "RR 3");
    

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
        
        // DRAWING A BUTTON
        for (int i = 0; i < 5; i++) {
            glBegin(GL_QUADS);
                glColor3fv(jg.button[i].color);
                glVertex2i(jg.button[i].r.left,  jg.button[i].r.bot);
                glVertex2i(jg.button[i].r.left,  jg.button[i].r.top);
                glVertex2i(jg.button[i].r.right, jg.button[i].r.top);
                glVertex2i(jg.button[i].r.right, jg.button[i].r.bot);
            glEnd();
            r.left = jg.button[i].r.left;
            r.bot = jg.button[i].r.bot;
            r.center = 0;
            ggprint16(&r, 0, jg.button[i].text_color, jg.button[i].text);
        }

        // LOGO
        glColor3f(1.0f, 1.0f, 1.0f); // reset color
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, jg.logo.texture);
        glTranslatef(jg.logo.x, jg.logo.y, 0.0f);
        glBegin(GL_QUADS);        
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(-jg.logo.w, -jg.logo.h); // bottom left
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(-jg.logo.w,  jg.logo.h); // bottom right
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i( jg.logo.w,  jg.logo.h); // top right
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i( jg.logo.w, -jg.logo.h); // top left
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
