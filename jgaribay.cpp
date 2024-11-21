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

int jg_nbuttons = 0;
JGlobal jg;

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
    logo.w = 75;
    logo.h = 50;
    logo.x = 0;
    logo.y = 0;
}

void joshua_init(int x, int y)
{
    int offset = 0;
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

    jg_nbuttons=0;
    // roadrunner racing
    jg.button[jg_nbuttons].r.width = 200;
    jg.button[jg_nbuttons].r.height = 50;
    jg.button[jg_nbuttons].r.left = x / 2 - jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = y / 2 - jg.button[jg_nbuttons].r.height / 2 + offset;
    jg.button[jg_nbuttons].r.right = jg.button[jg_nbuttons].r.left +
                               jg.button[jg_nbuttons].r.width;
    jg.button[jg_nbuttons].r.top = jg.button[jg_nbuttons].r.bot +
                             jg.button[jg_nbuttons].r.height;
    jg.button[jg_nbuttons].r.centerx = (jg.button[jg_nbuttons].r.left +
                                  jg.button[jg_nbuttons].r.right) / 2;
    jg.button[jg_nbuttons].r.centery = (jg.button[jg_nbuttons].r.bot +
                                  jg.button[jg_nbuttons].r.top) / 2;
    strcpy(jg.button[jg_nbuttons].text, "Resume");
    jg.button[jg_nbuttons].down = 0;
    jg.button[jg_nbuttons].click = 0;
    jg.button[jg_nbuttons].color[0] = 0.0f;
    jg.button[jg_nbuttons].color[1] = 0.2f;
    jg.button[jg_nbuttons].color[2] = 0.6f;
    jg.button[jg_nbuttons].hcolor[0] = 1.0f;
    jg.button[jg_nbuttons].hcolor[1] = 0.78f;
    jg.button[jg_nbuttons].hcolor[2] = 0.17f;
    jg.button[jg_nbuttons].dcolor[0] = jg.button[jg_nbuttons].color[0] * 0.5f;
    jg.button[jg_nbuttons].dcolor[1] = jg.button[jg_nbuttons].color[1] * 0.5f;
    jg.button[jg_nbuttons].dcolor[2] = jg.button[jg_nbuttons].color[2] * 0.5f;
    jg.button[jg_nbuttons].text_color = 0x00ffc72c;
    jg_nbuttons++;
    offset -= 60;

    jg.button[jg_nbuttons].r.width = 200;
    jg.button[jg_nbuttons].r.height = 50;
    jg.button[jg_nbuttons].r.left = x / 2 - jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = y / 2 - jg.button[jg_nbuttons].r.height / 2 + offset;
    jg.button[jg_nbuttons].r.right = jg.button[jg_nbuttons].r.left +
                               jg.button[jg_nbuttons].r.width;
    jg.button[jg_nbuttons].r.top = jg.button[jg_nbuttons].r.bot +
                             jg.button[jg_nbuttons].r.height;
    jg.button[jg_nbuttons].r.centerx = (jg.button[jg_nbuttons].r.left +
                                  jg.button[jg_nbuttons].r.right) / 2;
    jg.button[jg_nbuttons].r.centery = (jg.button[jg_nbuttons].r.bot +
                                  jg.button[jg_nbuttons].r.top) / 2;
    strcpy(jg.button[jg_nbuttons].text, "Main Menu");
    jg.button[jg_nbuttons].down = 0;
    jg.button[jg_nbuttons].click = 0;
    jg.button[jg_nbuttons].color[0] = 0.0f;
    jg.button[jg_nbuttons].color[1] = 0.2f;
    jg.button[jg_nbuttons].color[2] = 0.6f;
    jg.button[jg_nbuttons].hcolor[0] = 1.0f;
    jg.button[jg_nbuttons].hcolor[1] = 0.78f;
    jg.button[jg_nbuttons].hcolor[2] = 0.17f;
    jg.button[jg_nbuttons].dcolor[0] = jg.button[jg_nbuttons].color[0] * 0.5f;
    jg.button[jg_nbuttons].dcolor[1] = jg.button[jg_nbuttons].color[1] * 0.5f;
    jg.button[jg_nbuttons].dcolor[2] = jg.button[jg_nbuttons].color[2] * 0.5f;
    jg.button[jg_nbuttons].text_color = 0x00ffc72c;
    jg_nbuttons++;
    offset -= 60;

    jg.button[jg_nbuttons].r.width = 200;
    jg.button[jg_nbuttons].r.height = 50;
    jg.button[jg_nbuttons].r.left = x / 2 - jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = y / 2 - jg.button[jg_nbuttons].r.height / 2 + offset;
    jg.button[jg_nbuttons].r.right = jg.button[jg_nbuttons].r.left +
                               jg.button[jg_nbuttons].r.width;
    jg.button[jg_nbuttons].r.top = jg.button[jg_nbuttons].r.bot +
                             jg.button[jg_nbuttons].r.height;
    jg.button[jg_nbuttons].r.centerx = (jg.button[jg_nbuttons].r.left +
                                  jg.button[jg_nbuttons].r.right) / 2;
    jg.button[jg_nbuttons].r.centery = (jg.button[jg_nbuttons].r.bot +
                                  jg.button[jg_nbuttons].r.top) / 2;
    strcpy(jg.button[jg_nbuttons].text, "Quit");
    jg.button[jg_nbuttons].down = 0;
    jg.button[jg_nbuttons].click = 0;
    jg.button[jg_nbuttons].color[0] = 0.0f;
    jg.button[jg_nbuttons].color[1] = 0.2f;
    jg.button[jg_nbuttons].color[2] = 0.6f;
    jg.button[jg_nbuttons].hcolor[0] = 1.0f;
    jg.button[jg_nbuttons].hcolor[1] = 0.78f;
    jg.button[jg_nbuttons].hcolor[2] = 0.17f;
    jg.button[jg_nbuttons].dcolor[0] = jg.button[jg_nbuttons].color[0] * 0.5f;
    jg.button[jg_nbuttons].dcolor[1] = jg.button[jg_nbuttons].color[1] * 0.5f;
    jg.button[jg_nbuttons].dcolor[2] = jg.button[jg_nbuttons].color[2] * 0.5f;
    jg.button[jg_nbuttons].text_color = 0x00ffc72c;
    jg_nbuttons++;
    offset -= 60;
}

void joshua_init_opengl()
{
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

void render_pause_screen(int x, int y)
{
    Rect r;
    // positioning
    jg.logo.x = x / 2;
    jg.logo.y = y - (y / 4) + 30;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    // dim screen
    glBegin(GL_QUADS);
    glColor4ub(0, 0, 0, 200);
        glVertex2i(0, 0);
        glVertex2i(0, y);
        glVertex2i(x, y);
        glVertex2i(x, 0);
    glEnd();
 
    // roadrunner logo
    glBindTexture(GL_TEXTURE_2D, jg.logo.texture);
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glPushMatrix();
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
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    
    glDisable(GL_BLEND);
   
    r.bot = (y / 2) + 30;
    r.left = x / 2;
    r.center = 1;
    ggprint40(&r, 0, 0x00ffc72c, "PAUSED");

    // BUTTONS
    // render buttons here
    for (int i = 0; i < jg_nbuttons; i++) {
        if (jg.button[i].over) {
            glColor3fv(jg.button[i].hcolor);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(jg.button[i].r.left - 2,  jg.button[i].r.bot - 2);
                glVertex2i(jg.button[i].r.left - 2,  jg.button[i].r.top + 2);
                glVertex2i(jg.button[i].r.right + 2, jg.button[i].r.top + 2);
                glVertex2i(jg.button[i].r.right + 2, jg.button[i].r.bot - 2);
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
}

// FIGURE UOT HOW TO RESTART PHYSICS AND NOT RETAIN VALUES
void render_racing(int x, int y)
{
    Rect r;

    // positioning
    jg.pf.w = x;
    //jg.pf.h = y / 3;

    // render background
    glBindTexture(GL_TEXTURE_2D, jg.bg.texture);
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glBegin(GL_QUADS);
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[0]); glVertex2i(0, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[0]); glVertex2i(x, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[1]); glVertex2i(x, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // render platform
    glBindTexture(GL_TEXTURE_2D, jg.pf.texture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);        
        glTexCoord2f(jg.pf.xc[0], jg.pf.yc[1]); glVertex2i(0,       0);
        glTexCoord2f(jg.pf.xc[0], jg.pf.yc[0]); glVertex2i(0,       jg.pf.h);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[0]); glVertex2i(jg.pf.w, jg.pf.h);
        glTexCoord2f(jg.pf.xc[1], jg.pf.yc[1]); glVertex2i(jg.pf.w, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
 
    // render roadrunners
    int ix = jg.walkframe % 6;
    int iy = 0;
    if (jg.walkframe >= 6)
        iy = 1;
    float fx = (float)ix / 6.0;
    float fy = (float)iy / 2.0;
    for (int i = 0; i < 3; i++) {
        glBindTexture(GL_TEXTURE_2D, jg.rrsprite.texture);
        glColor3fv(jg.rr[i].color);
        glPushMatrix();
        glBegin(GL_QUADS);
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
        glBindTexture(GL_TEXTURE_2D, 0);
    }
 
    // render leaderboard
    r.bot = y - 30;
    r.left = 20;
    r.center = 0;
    ggprint13(&r, 16, 0x00003594, "Leaderboard");
    for (int i = 0; i < 3; i++) {
        ggprint8b(&r, 16, jg.rr[i].alt_color, "Roadrunner %i vel: %f", i + 1,
                jg.rr[i].vel[0]);
    }
    ggprint13(&r, 16, 0x00003594, "Controls");
    ggprint8b(&r, 16, 0x00003594, "o - outline");

    glDisable(GL_BLEND);
}
