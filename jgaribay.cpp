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

JGlobal::JGlobal() {
    outline = 0;
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
    // TODO: make colors random
    // roadrunner colors
    /*
    for (int i = 0; i < 3; i++) {
        jg.rr[i].alt_color = rand();
    }
    */
    jg.rr[0].color[0] = 1.0f;
    jg.rr[0].color[1] = 0.0f;
    jg.rr[0].color[2] = 0.0f;
    jg.rr[1].color[0] = 0.0f;
    jg.rr[1].color[1] = 1.0f;
    jg.rr[1].color[2] = 0.0f;
    jg.rr[2].color[0] = 0.0f;
    jg.rr[2].color[1] = 0.0f;
    jg.rr[2].color[2] = 1.0f;
    jg.rr[0].alt_color = 0x00ff0000;
    jg.rr[1].alt_color = 0x0000ff00;
    jg.rr[2].alt_color = 0x000000ff;

    jg.button[0].r.width = 200;
    jg.button[0].r.height = 50;
    jg.button[0].r.left = 10;
    jg.button[0].r.bot = 10;
    jg.button[0].r.right =
        jg.button[0].r.left + jg.button[0].r.width;
    jg.button[0].r.top = jg.button[0].r.bot + jg.button[0].r.height;
    jg.button[0].r.centerx = (jg.button[0].r.left + jg.button[0].r.right) / 2;
    jg.button[0].r.centery = (jg.button[0].r.bot + jg.button[0].r.top) / 2;
    strcpy(jg.button[0].text, "Exit");
    jg.button[0].down = 0;
    jg.button[0].click = 0;
    jg.button[0].color[0] = 1.0f;
    jg.button[0].color[1] = 0.7f;
    jg.button[0].color[2] = 0.5f;
    jg.button[0].dcolor[0] = jg.button[0].color[0] * 0.5f;
    jg.button[0].dcolor[1] = jg.button[0].color[1] * 0.5f;
    jg.button[0].dcolor[2] = jg.button[0].color[2] * 0.5f;
    jg.button[0].text_color = 0x00ff00ff;
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

void joshua_render_pause(int x, int y)
{
    Rect r;

    // dim screen
    glBegin(GL_QUADS);
    glColor4ub(0, 0, 0, 200);
        glVertex2i(0, 0);
        glVertex2i(0, y);
        glVertex2i(x, y);
        glVertex2i(x, 0);
    glEnd();

    glEnable(GL_TEXTURE_2D);

    // BUTTONS
    for (int i = 0; i < 5; i++) {
        if (jg.button[i].over) {
            glColor3f(1.0f, 0.0f, 1.0f);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(jg.button[i].r.left - 2,  jg.button[i].r.bot - 2);
                glVertex2i(jg.button[i].r.left - 2,  jg.button[i].r.top + 2);
                glVertex2i(jg.button[i].r.right + 2, jg.button[i].r.top + 2);
                glVertex2i(jg.button[i].r.right + 2, jg.button[i].r.bot - 2);
                glVertex2i(jg.button[i].r.left - 2,  jg.button[i].r.bot - 2);
            glEnd();
            glLineWidth(1);
        }
        if (jg.button[i].down) {
            glColor3fv(jg.button[i].dcolor);
        } else {
            glColor3fv(jg.button[i].color);
        }
        glBegin(GL_QUADS);
            glVertex2i(jg.button[i].r.left,  jg.button[i].r.bot);
            glVertex2i(jg.button[i].r.left,  jg.button[i].r.top);
            glVertex2i(jg.button[i].r.right, jg.button[i].r.top);
            glVertex2i(jg.button[i].r.right, jg.button[i].r.bot);
        glEnd();
        r.left = jg.button[i].r.centerx;
        r.bot  = jg.button[i].r.centery-8;
        r.center = 1;
        if (jg.button[i].down) {
            ggprint16(&r, 0, jg.button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, jg.button[i].text_color, jg.button[i].text);
        }
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

    r.bot = y / 2;
    r.left = x / 2;
    r.center = 1;
    ggprint40(&r, 0, 0x00ffc72c, "PAUSED");
}

void joshua_render_rr()
{
    Rect r;
  
    // TODO: fix dark blue color
    // box outline
    if (jg.outline) {
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glColor3fv(jg.rr[i].color);
            glLineWidth(6);
            glTranslatef(jg.rr[i].x, jg.rr[i].y, 0.0f);
            glBegin(GL_LINE_LOOP);
                glVertex2i(-jg.rr[i].w, -jg.rr[i].h);
                glVertex2i(-jg.rr[i].w,  jg.rr[i].h);
                glVertex2i( jg.rr[i].w,  jg.rr[i].h);
                glVertex2i( jg.rr[i].w, -jg.rr[i].h);
            glEnd();
            glPopMatrix();
            glLineWidth(1);
        }
    }

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
            glColor3fv(jg.rr[i].color);
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

    r.center = 1;
    for (int i = 0; i < 3; i++) {
        r.bot = jg.rr[i].y + jg.rr[i].h + 20;
        r.left = jg.rr[i].x;
        ggprint13(&r, 16, jg.rr[i].alt_color, "Roadrunner %i", i + 1);
    }
}

void joshua_render(int x, int y, int status)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    
    Rect r;

    // POSITION STUFF
    jg.logo.x = x / 2;
    jg.logo.y = y - (y / 4);
    jg.pf.w = x;
    //jg.pf.h = y / 3;

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
        glTexCoord2f(jg.pf.xc[0], jg.pf.yc[1]); glVertex2i(0,       0);
        glTexCoord2f(jg.pf.xc[0], jg.pf.yc[0]); glVertex2i(0,       jg.pf.h);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[0]); glVertex2i(jg.pf.w, jg.pf.h);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[1]); glVertex2i(jg.pf.w, 0);
    glEnd();

    joshua_render_rr();

    unsigned int c = 0x00003594;
    r.bot = y - 30;
    r.left = 20;
    r.center = 0;
    ggprint13(&r, 16, c, "Leaderboard");
    for (int i = 0; i < 3; i++) {
        ggprint8b(&r, 16, jg.rr[i].alt_color, "Roadrunner %i vel: %f", i + 1,
                jg.rr[i].vel[0]);
    }
    ggprint13(&r, 16, c, "Controls");
    ggprint8b(&r, 16, c, "o - toggle outline");
    
    glDisable(GL_TEXTURE_2D);

    // paused
    if (status)
        joshua_render_pause(x, y);
  }
