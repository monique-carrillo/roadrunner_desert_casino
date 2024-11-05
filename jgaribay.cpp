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

GLuint textureID;

Image jimg[3] = {
    "./images/roadrunner.jpg",
    "./images/desert.jpg",
    "./images/rrsprite.jpg"
};

class Texture {
    public:
        Image *bgImage;
        GLuint bgTexture;
        float xc[2];
        float yc[2];
};

class JGlobal {
    public:
        Texture bg;
} jg;

Roadrunner::Roadrunner() {
    pos[0] = 100;
    pos[1] = 300;
    vel[0] = vel[1] = 0.0f;
    w = 100;
    h = 200;
};

void joshua_main()
{
    Roadrunner rr;
}

void joshua_init_opengl()
{
    glEnable(GL_BLEND);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, jimg[1].width, jimg[1].height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, jg.bg.bgImage->data);
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
}

void joshua_render(int x, int y)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    int speed = 0;

    /*
    glBegin(GL_QUADS);
        glColor3ub(0, 53, 148);
        glVertex2i(10, 10);
        glVertex2i(x - 10, 10);
        glColor3ub(255, 255, 255);
        glVertex2i(x - 10, y - 10);
        glVertex2i(10, y - 10);
    glEnd();
    */

    glEnable(GL_TEXTURE_2D);

    // background
    glColor3f(1.0f, 1.0f, 1.0f); // reset color
    glBindTexture(GL_TEXTURE_2D, jg.bg.bgTexture);
    glBegin(GL_QUADS);        
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(jg.bg.xc[0], jg.bg.yc[0]); glVertex2i(0, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[1]); glVertex2i(x, y);
        glTexCoord2f(jg.bg.xc[1], jg.bg.yc[1]); glVertex2i(x, 0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);        
        glTexCoord2f(0.0f, 1.0f); glVertex2i(100, 100);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x - 100, 100);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x - 100, y - 100);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(100, y - 100);
    glEnd();

    Rect r;
    unsigned int c = 0x00ffffff;
    r.bot = y - 30;
    r.left = 20;
    r.center = 0;
    for (int i = 0; i < 3; i++) {
        ggprint8b(&r, 16, c, "Roadrunner %i speed: %i", i + 1, speed);
    }

    Rect s;
    c = 0x00003594;
    s.bot = y - 100;
    s.left = x / 2;
    s.center = 1;
    ggprint40(&s, 16, c, "Rowdy Roadrunner Race");
}
