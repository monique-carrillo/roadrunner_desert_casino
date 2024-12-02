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

JGlobal::JGlobal() {
    jg.win = 0;
    camera[0] = camera[1] = 0.0f;
    frame = 0;
    pf.w = 650;
    pf.h = 175;
    lg.w = 75;
    lg.h = 50;
    lg.x = 0;
    lg.y = 0;
}

int rr_setup = 0;
int rr_chosen = 0;

void joshua_init()
{
    int offset = 0;
    // positioning
    jg.pf.w = jg.xres;
    //jg.pf.h = y / 3;
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
    jg.button[jg_nbuttons].r.left = jg.xres / 2 -
        jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = jg.yres / 2 -
        jg.button[jg_nbuttons].r.height / 2 + offset;
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
    jg.button[jg_nbuttons].r.left = jg.xres / 2 -
        jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = jg.yres / 2 -
        jg.button[jg_nbuttons].r.height / 2 + offset;
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
    jg.button[jg_nbuttons].r.left = jg.xres / 2 -
        jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = jg.yres / 2 -
        jg.button[jg_nbuttons].r.height / 2 + offset;
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
    
    offset = 0;
    jg.button[jg_nbuttons].r.width = 200;
    jg.button[jg_nbuttons].r.height = 50;
    jg.button[jg_nbuttons].r.left = jg.xres / 2 -
        jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = jg.yres / 2 -
        jg.button[jg_nbuttons].r.height / 2 + offset;
    jg.button[jg_nbuttons].r.right = jg.button[jg_nbuttons].r.left +
                               jg.button[jg_nbuttons].r.width;
    jg.button[jg_nbuttons].r.top = jg.button[jg_nbuttons].r.bot +
                             jg.button[jg_nbuttons].r.height;
    jg.button[jg_nbuttons].r.centerx = (jg.button[jg_nbuttons].r.left +
                                  jg.button[jg_nbuttons].r.right) / 2;
    jg.button[jg_nbuttons].r.centery = (jg.button[jg_nbuttons].r.bot +
                                  jg.button[jg_nbuttons].r.top) / 2;
    strcpy(jg.button[jg_nbuttons].text, "Red");
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
    jg.button[jg_nbuttons].r.left = jg.xres / 2 -
        jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = jg.yres / 2 -
        jg.button[jg_nbuttons].r.height / 2 + offset;
    jg.button[jg_nbuttons].r.right = jg.button[jg_nbuttons].r.left +
                               jg.button[jg_nbuttons].r.width;
    jg.button[jg_nbuttons].r.top = jg.button[jg_nbuttons].r.bot +
                             jg.button[jg_nbuttons].r.height;
    jg.button[jg_nbuttons].r.centerx = (jg.button[jg_nbuttons].r.left +
                                  jg.button[jg_nbuttons].r.right) / 2;
    jg.button[jg_nbuttons].r.centery = (jg.button[jg_nbuttons].r.bot +
                                  jg.button[jg_nbuttons].r.top) / 2;
    strcpy(jg.button[jg_nbuttons].text, "Green");
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
    jg.button[jg_nbuttons].r.left = jg.xres / 2 -
        jg.button[jg_nbuttons].r.width / 2;
    jg.button[jg_nbuttons].r.bot = jg.yres / 2 -
        jg.button[jg_nbuttons].r.height / 2 + offset;
    jg.button[jg_nbuttons].r.right = jg.button[jg_nbuttons].r.left +
                               jg.button[jg_nbuttons].r.width;
    jg.button[jg_nbuttons].r.top = jg.button[jg_nbuttons].r.bot +
                             jg.button[jg_nbuttons].r.height;
    jg.button[jg_nbuttons].r.centerx = (jg.button[jg_nbuttons].r.left +
                                  jg.button[jg_nbuttons].r.right) / 2;
    jg.button[jg_nbuttons].r.centery = (jg.button[jg_nbuttons].r.bot +
                                  jg.button[jg_nbuttons].r.top) / 2;
    strcpy(jg.button[jg_nbuttons].text, "Blue");
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
    int w, h;

    // logo
    jg.lg.image = &jimg[0];
    w = jg.lg.image->width;
    h = jg.lg.image->height;
    glGenTextures(1, &jg.lg.texture);
    glBindTexture(GL_TEXTURE_2D, jg.lg.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *lg_jimg_data = buildAlphaData(&jimg[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            lg_jimg_data);
    free(lg_jimg_data);

    // background
    jg.bg.image = &jimg[1];
    w = jg.bg.image->width;
    h = jg.bg.image->height;
    glGenTextures(1, &jg.bg.texture);
    glBindTexture(GL_TEXTURE_2D, jg.bg.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
            jg.bg.image->data);
    jg.bg.xc[0] = 0.0;
    jg.bg.xc[1] = 0.25;
    jg.bg.yc[0] = 0.0;
    jg.bg.yc[1] = 1.0;

    // roadrunner
    jg.rr_texture.image = &jimg[2];
    w = jg.rr_texture.image->width;
    h = jg.rr_texture.image->height;
    glGenTextures(1, &jg.rr_texture.texture);
    glBindTexture(GL_TEXTURE_2D, jg.rr_texture.texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *roadrunner_jimg_data = buildAlphaData(&jimg[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            roadrunner_jimg_data);
    free(roadrunner_jimg_data);

    // PLATFORM
    jg.pf.image = &jimg[3];
    w = jg.pf.image->width;
    h = jg.pf.image->height;
    glGenTextures(1, &jg.pf.texture);
    glBindTexture(GL_TEXTURE_2D, jg.pf.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
            jg.pf.image->data);
    jg.pf.xc[0] = 0.0;
    jg.pf.xc[1] = 0.25;
    jg.pf.yc[0] = 0.0;
    jg.pf.yc[1] = 1.0;
}

void joshua_physics()
{
    /*
    for (int i = 0; i < 3; i++) {
        jg.rr[i].vel[1] += GRAVITY;
        if (jg.rr[i].vel[0] > 0) {
            jg.rr[i].vel[0] -= 0.005;
            if (jg.rr[i].vel[0] < 0)
                    jg.rr[i].vel[0] = 0.0;
        }
        if (jg.rr[i].last_x != jg.rr[i].x || jg.rr[i].last_y != jg.rr[i].y) {
            // moving
            jg.rr[i].last_x = jg.rr[i].x;
            jg.rr[i].last_y = jg.rr[i].y;
            // change frame
            jg.frame += 1;
            // move background
            jg.bg.xc[0] += 0.005;
            jg.bg.xc[1] += 0.005;
            jg.pf.xc[0] += 0.01;
            jg.pf.xc[1] += 0.01;
        }

        jg.rr[i].x += jg.rr[i].vel[0]; // move x pos
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
    */
    if (jg.win)
        return;
    jg.bg.xc[0] += 0.005;
    jg.bg.xc[1] += 0.005;
    jg.pf.xc[0] += 0.01;
    jg.pf.xc[1] += 0.01;
   
    float avgvel = 0;
    for (int i = 0; i < 3; i++) {
        avgvel += jg.rr[i].vel[0];
    }
    avgvel /= 3;
    

    for (int i = 0; i < 3; i++) {
        if (jg.rr[i].x > jg.xres) {
            if (rr_chosen == i) {
                jg.win = 1;
                money += 100;
            } else {
                jg.win = 2;
                money -= 50;
            }
        }
    
        //if (jg.rr[i].vel[0] != 0)
        jg.frame += 1;

        jg.rr[i].vel[1] += GRAVITY;
        jg.rr[i].last_x = jg.rr[i].x;
        jg.rr[i].last_y = jg.rr[i].y;
        jg.rr[i].vel[0] = (double)rand() / (double)RAND_MAX;
        
        if (jg.rr[i].vel[0] > avgvel)
            jg.rr[i].x += jg.rr[i].vel[0] * 2; // move x pos
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

void render_setup()
{
    Rect r;
    // BUTTONS
    // render buttons here
    
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.7f);
        glVertex2i(0, 0);
        glVertex2i(0, jg.yres);
        glVertex2i(jg.xres, jg.yres);
        glVertex2i(jg.xres, 0);
    glEnd();
        
    r.left = jg.xres / 2;
    r.bot = jg.yres - (jg.yres / 4);
    r.center = 1;
    ggprint40(&r, 0, jg.button[0].text_color, "Choose a roadrunner!");
    
    for (int i = 3; i < jg_nbuttons; i++) {
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

void render_pause_screen()
{
    Rect r;
    // positioning
    jg.lg.x = jg.xres / 2;
    jg.lg.y = jg.yres - (jg.yres / 4) + 30;
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    // dim screen
    glBegin(GL_QUADS);
    glColor4ub(0, 0, 0, 200);
        glVertex2i(0, 0);
        glVertex2i(0, jg.yres);
        glVertex2i(jg.xres, jg.yres);
        glVertex2i(jg.xres, 0);
    glEnd();
 
    // roadrunner lg
    glBindTexture(GL_TEXTURE_2D, jg.lg.texture);
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glPushMatrix();
    glTranslatef(jg.lg.x, jg.lg.y, 0.0f);
    glBegin(GL_QUADS);        
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-jg.lg.w, -jg.lg.h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-jg.lg.w,  jg.lg.h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( jg.lg.w,  jg.lg.h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( jg.lg.w, -jg.lg.h);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    
    glDisable(GL_BLEND);
   
    r.bot = (jg.yres / 2) + 30;
    r.left = jg.xres / 2;
    r.center = 1;
    ggprint40(&r, 0, 0x00ffc72c, "PAUSED");

    // BUTTONS
    // render buttons here
    for (int i = 0; i < 3; i++) {
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
void render_racing()
{
    Rect r;

    // render background
    glBindTexture(GL_TEXTURE_2D, jg.bg.texture);
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glBegin(GL_QUADS);
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[1]); glVertex2i(0,       0);
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[0]); glVertex2i(0,       jg.yres);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[0]); glVertex2i(jg.xres, jg.yres);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[1]); glVertex2i(jg.xres, 0);
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
    int ix = jg.frame % 6;
    int iy = 0;
    if (jg.frame >= 6)
        iy = 1;
    float fx = (float)ix / 6.0;
    float fy = (float)iy / 2.0;
    for (int i = 0; i < 3; i++) {
        glBindTexture(GL_TEXTURE_2D, jg.rr_texture.texture);
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
    r.bot = jg.yres - 30;
    r.left = 20;
    r.center = 0;
    ggprint13(&r, 16, 0x00003594, "Leaderboard");
    for (int i = 0; i < 3; i++) {
        ggprint8b(&r, 16, jg.rr[i].alt_color, "Roadrunner %i vel: %f", i + 1,
                jg.rr[i].vel[0]);
    }

    if (jg.win == 1) {
        glBegin(GL_QUADS);
        glColor4ub(0, 0, 0, 200);
            glVertex2i(0, 0);
            glVertex2i(0, jg.yres);
            glVertex2i(jg.xres, jg.yres);
            glVertex2i(jg.xres, 0);
        glEnd();
        
        r.bot = jg.yres / 2;
        r.left = jg.xres / 2;
        r.center = 1;
        ggprint40(&r, 50, 0x0000ff00, "YOU WIN :)");
        ggprint40(&r, 0, 0x0000ff00, "+$100");
    } else if (jg.win == 2) {
        glBegin(GL_QUADS);
        glColor4ub(0, 0, 0, 200);
            glVertex2i(0, 0);
            glVertex2i(0, jg.yres);
            glVertex2i(jg.xres, jg.yres);
            glVertex2i(jg.xres, 0);
        glEnd();
        
        r.bot = jg.yres / 2;
        r.left = jg.xres / 2;
        r.center = 1;
        ggprint40(&r, 50, 0x00ff0000, "YOU LOST :(");
        ggprint40(&r, 0, 0x00ff0000, "-$50");
    }
    
    glDisable(GL_BLEND);
}
