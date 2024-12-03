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

int jg_nbuttons = 15;

// roadrunner sprite source:
// https://www.spriters-resource.com/arcade/roadrun/sheet/11658/
// shop background image obtained from freepik:
// Designed by Freepik: https://www.freepik.com/free-vector/
// flat-design-poker-table-background_88532181.htm
// #fromView=search&page=1&position=2&uuid=eb890701-051d-4744-b293-c702abde4895
Image jimg[5] = {
    "./images/logo.jpg",
    "./images/desert.jpg",
    "./images/roadrunner.jpg",
    "./images/platform.jpg",
    "./images/shop_background.jpg"
};

Roadrunner::Roadrunner() {
    x = 250;  // cx
    y = 250;  // cy
    vel[0] = vel[1] = 0.0f;
    w = 30;  // width
    h = 40;  // height
};

Theme::Theme(const float *p1, unsigned int p2, const float *s1, unsigned int s2)
{
    for (int i = 0; i < COLOR_SIZE; i++) {
        primary1[i] = p1[i];
        secondary1[i] = s1[i];
    }
    primary2 = p2;
    secondary2 = s2;
}

Theme::Theme(Theme &t)
{
    for (int i = 0; i < COLOR_SIZE; i++) {
        primary1[i] = t.primary1[i];
        secondary1[i] = t.secondary1[i];
    }
    primary2 = t.primary2;
    secondary2 = t.secondary2;
}

float csub_p1[3] = { 0.0f, 0.2f, 0.58f };
float csub_s1[3] = { 1.0f, 0.78f, 0.17f };
Theme csub(csub_p1, 0x00003594, csub_s1, 0x00ffc72c);

float light_p[3] = { 0.95f, 0.95f, 0.95f };
float light_s[3] = { 0.8f, 0.8f, 0.8f };
Theme light(light_p, 0xF2F2F2, light_s, 0xCCCCCC);

float dark_p[3] = { 0.1f, 0.1f, 0.1f };
float dark_s[3] = { 0.2f, 0.2f, 0.2f };
Theme dark(dark_p, 0x1A1A1A, dark_s, 0x333333);

float strawberry_p[3] = { 1.0f, 0.18f, 0.33f };
float strawberry_s[3] = { 1.0f, 0.67f, 0.74f };
Theme strawberry(strawberry_p, 0xFF2E55, strawberry_s, 0xFFABB9);

float charcoal_p[3] = { 0.17f, 0.17f, 0.17f };
float charcoal_s[3] = { 0.3f, 0.3f, 0.3f };
Theme charcoal(charcoal_p, 0x2B2B2B, charcoal_s, 0x4C4C4C);

float mint_p[3] = { 0.6f, 1.0f, 0.8f };
float mint_s[3] = { 0.5f, 0.94f, 0.7f };
Theme mint(mint_p, 0x99FFCC, mint_s, 0x80F0B3);

float royal_p[3] = { 0.25f, 0.41f, 0.88f };
float royal_s[3] = { 0.0f, 0.14f, 0.4f };
Theme royal(royal_p, 0x4169E1, royal_s, 0x002366);

float peach_p[3] = { 1.0f, 0.85f, 0.73f };
float peach_s[3] = { 1.0f, 0.72f, 0.58f };
Theme peach(peach_p, 0xFFDAB9, peach_s, 0xFFA07A);

float slate_p[3] = { 0.44f, 0.5f, 0.56f };
float slate_s[3] = { 0.62f, 0.71f, 0.8f };
Theme slate(slate_p, 0x708090, slate_s, 0x9FB6CD);

JGlobal jg;

JGlobal::JGlobal() : theme(csub) {
    win = 0;
    camera[0] = camera[1] = 0.0f;
    frame = 0;
    pf.w = 650;
    pf.h = 175;
    lg.w = 75;
    lg.h = 50;
    lg.x = 0;
    lg.y = 0;
    setup = 0;
    chosen = 0;
}

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

    for (int i = 0; i < jg_nbuttons; i++) {
        jg.button[i].r.width = 200;
        jg.button[i].r.height = 50;
        jg.button[i].r.left = jg.xres / 2 - jg.button[i].r.width / 2;
        jg.button[i].r.bot = jg.yres / 2 - jg.button[i].r.height / 2 + offset;
        jg.button[i].r.right = jg.button[i].r.left + jg.button[i].r.width;
        jg.button[i].r.top = jg.button[i].r.bot + jg.button[i].r.height;
        jg.button[i].r.centerx = (jg.button[i].r.left + jg.button[i].r.right) /
            2;
        jg.button[i].r.centery = (jg.button[i].r.bot + jg.button[i].r.top) /
            2;
        switch (i) {
            case 0:
                strcpy(jg.button[i].text, "Resume");
                break;
            case 1:
                strcpy(jg.button[i].text, "Main Menu");
                break;
            case 2:
                strcpy(jg.button[i].text, "Quit");
                break;
            case 3:
                strcpy(jg.button[i].text, "Red");
                break;
            case 4:
                strcpy(jg.button[i].text, "Green");
                break;
            case 5:
                strcpy(jg.button[i].text, "Blue");
                break;
            case 6:
                strcpy(jg.button[i].text, "CSUB");
                break;
            case 7:
                strcpy(jg.button[i].text, "Light");
                break;
            case 8:
                strcpy(jg.button[i].text, "Dark");
                break;
            case 9:
                strcpy(jg.button[i].text, "Strawberry");
                break;
            case 10:
                strcpy(jg.button[i].text, "Charcoal");
                break;
            case 11:
                strcpy(jg.button[i].text, "Mint");
                break;
            case 12:
                strcpy(jg.button[i].text, "Royal");
                break;
            case 13:
                strcpy(jg.button[i].text, "Peach");
                break;
            case 14:
                strcpy(jg.button[i].text, "Slate");
                break;
        }
        jg.button[i].down = 0;
        jg.button[i].click = 0;
        for (int j = 0; j < COLOR_SIZE; j++) {
            jg.button[i].color[j] = jg.theme.primary1[j];
            jg.button[i].hcolor[j] = jg.theme.secondary1[j];
            jg.button[i].dcolor[j] = jg.theme.primary1[j] * 0.5f;
        }
        jg.button[i].text_color = jg.theme.secondary2;
        if (i == 2)
            offset = 75;
        if (i == 5)
            offset = 100;
        else
            offset -= 60;
    }
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
    
    // shop background
    jg.shop.image = &jimg[4];
    w = jg.shop.image->width;
    h = jg.shop.image->height;
    glGenTextures(1, &jg.shop.texture);
    glBindTexture(GL_TEXTURE_2D, jg.shop.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
            jg.shop.image->data);
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
        if (jg.rr[i].x + jg.rr[i].w >= jg.xres) {
            if (jg.chosen == i) {
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
    ggprint40(&r, 0, jg.theme.primary2, "Choose a roadrunner!");
    
    for (int i = 3; i < 6; i++) {
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
    glColor3fv(jg.theme.secondary1); // reset color
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
    ggprint40(&r, 0, jg.theme.primary2, "PAUSED");

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

void render_shop()
{
    Rect r;

    glBindTexture(GL_TEXTURE_2D, jg.shop.texture);
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0,       0);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0,       jg.yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(jg.xres, jg.yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(jg.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    r.bot = jg.yres - 90;
    r.left = jg.xres / 2;
    r.center = 1;
    ggprint40(&r, 0, jg.theme.primary2, "SHOP");

    for (int i = 6; i < jg_nbuttons; i++) {
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
        if (i == jg.chosen) {
            r.bot = jg.rr[i].y + jg.rr[i].h;
            r.left = jg.rr[i].x;
            r.center = 1;
            ggprint16(&r, -20, jg.rr[i].alt_color, "\\/");
            ggprint16(&r, -20, jg.rr[i].alt_color, "|");
            ggprint16(&r, 0, jg.rr[i].alt_color, "You");
        }
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
