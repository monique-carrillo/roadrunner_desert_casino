/*
 * original program: rainforest.cpp
 *  original author: Gordon Griesel
 *             date: 2013 to 2021
 *
 *      modified by: 3350 group 1
 * modified program: rdc.cpp
 *          purpose: software engineering group project
 *             date: fall 2024
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "jgaribay.h"
#include "dbenavides.h"
#include "jchicas.h"
#include "images.h"
#include "mcarrillo.h"

#define NCARDS 52

//Setup timers
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown = 0.0;
double timeSpan = 0.0;
unsigned int upause = 0;
double timeDiff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
            (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
    memcpy(dest, source, sizeof(struct timespec));
}

int lbutton = 0;
int rbutton = 0;

Button button[MAXBUTTONS];
int nbuttons = 6;
float money = 0.0;

Image img[2] = {
    background_time(),
    "./images/felt.jpg"
};

Image card_images[NCARDS] = {
    "./images/cards/c_A.jpg", "./images/cards/c_2.jpg",
    "./images/cards/c_3.jpg", "./images/cards/c_4.jpg",
    "./images/cards/c_5.jpg", "./images/cards/c_6.jpg",
    "./images/cards/c_7.jpg", "./images/cards/c_8.jpg",
    "./images/cards/c_9.jpg", "./images/cards/c_10.jpg",
    "./images/cards/c_J.jpg", "./images/cards/c_Q.jpg",
    "./images/cards/c_K.jpg", "./images/cards/d_A.jpg",
    "./images/cards/d_2.jpg", "./images/cards/d_3.jpg",
    "./images/cards/d_4.jpg", "./images/cards/d_5.jpg",
    "./images/cards/d_6.jpg", "./images/cards/d_7.jpg",
    "./images/cards/d_8.jpg", "./images/cards/d_9.jpg",
    "./images/cards/d_10.jpg", "./images/cards/d_J.jpg",
    "./images/cards/d_Q.jpg", "./images/cards/d_K.jpg",
    "./images/cards/h_A.jpg", "./images/cards/h_2.jpg",
    "./images/cards/h_3.jpg", "./images/cards/h_4.jpg",
    "./images/cards/h_5.jpg", "./images/cards/h_6.jpg",
    "./images/cards/h_7.jpg", "./images/cards/h_8.jpg",
    "./images/cards/h_9.jpg", "./images/cards/h_10.jpg",
    "./images/cards/h_J.jpg", "./images/cards/h_Q.jpg",
    "./images/cards/h_K.jpg", "./images/cards/s_A.jpg",
    "./images/cards/s_2.jpg", "./images/cards/s_3.jpg",
    "./images/cards/s_4.jpg", "./images/cards/s_5.jpg",
    "./images/cards/s_6.jpg", "./images/cards/s_7.jpg",
    "./images/cards/s_8.jpg", "./images/cards/s_9.jpg",
    "./images/cards/s_10.jpg", "./images/cards/s_J.jpg",
    "./images/cards/s_Q.jpg", "./images/cards/s_K.jpg"
};

enum {
    MODE_MENU = 0,
    MODE_RACING,
    MODE_BLACKJACK,
    MODE_POKER,
    MODE_CEELO,
    MODE_SHOP
};

class Global {
    public:
        int xres, yres;
        int paused;
        int gamemode;
    bool gamecheck = false;
        int check2 = 0;
        int done;
        GLuint bgTexture;
        Image *felt_image;
        GLuint felt_texture;
        Image *card_image[NCARDS];
        GLuint card_texture[NCARDS];
        Global() {
            done = 0;
            gamemode = MODE_MENU;
            paused = 0;
            xres = 800;
            yres = 600;
        }
} g;

void init_opengl(void);
void mouse_click(int);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void init();
void physics(void);
void render_main_menu(void);
void render(void);

class X11_wrapper {
    private:
        Display *dpy;
        Window win;
    public:
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER,
                            None };
            XSetWindowAttributes swa;
            setup_screen_res(640, 480);
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL) {
                printf("\n\tcannot connect to X server\n\n");
                exit(EXIT_FAILURE);
            }
            Window root = DefaultRootWindow(dpy);
            XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
            if (vi == NULL) {
                printf("\n\tno appropriate visual found\n\n");
                exit(EXIT_FAILURE);
            } 
            Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
            swa.colormap = cmap;
            swa.event_mask = ExposureMask |
                             KeyPressMask |
                             KeyReleaseMask |
                             ButtonPressMask |
                             ButtonReleaseMask |
                             PointerMotionMask |
                             StructureNotifyMask |
                             SubstructureNotifyMask;
            win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
                    vi->depth, InputOutput, vi->visual,
                    CWColormap | CWEventMask, &swa);
            GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
            set_title();
        }
        ~X11_wrapper() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void set_title() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "Roadrunner Desert Casino");
        }
        void setup_screen_res(const int w, const int h) {
            g.xres = jg.xres = w;
            g.yres = jg.yres = h;
        }
        void reshape_window(int width, int height) {
            // window resized
            setup_screen_res(width, height);
            glViewport(0, 0, (GLint)width, (GLint)height);
            glMatrixMode(GL_PROJECTION); glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            glOrtho(0, g.xres, 0, g.yres, -1, 1);
            set_title();
            init();
        }
        void check_resize(XEvent *e) {
            //The ConfigureNotify is sent by the
            //server if the window is resized.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            if (xce.width != g.xres || xce.height != g.yres) {
                //Window size did change.
                reshape_window(xce.width, xce.height);
            }
        }
        bool getXPending() {
            return XPending(dpy);
        }
        XEvent getXNextEvent() {
            XEvent e;
            XNextEvent(dpy, &e);
            return e;
        }
        void swap_buffers() {
            glXSwapBuffers(dpy, win);
        }
} x11;

int main()
{
    srand(time(NULL));
    init_opengl();
    initialize_fonts();
    init();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    
    while (!g.done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            check_mouse(&e);
            check_keys(&e);
        }
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        timeSpan = timeDiff(&timeStart, &timeCurrent);
        timeCopy(&timeStart, &timeCurrent);
        physicsCountdown += timeSpan;
        while (physicsCountdown >= physicsRate) {
            physics();
            physicsCountdown -= physicsRate;
        }
        render();
        x11.swap_buffers();
    }
    cleanup_fonts();
    return 0;
}

void init_cards()
{
    for (int i = 0; i < NCARDS; i++) {
        g.card_image[i] = &card_images[i];
        glGenTextures(1, &g.card_texture[i]);
        int w = g.card_image[i]->width;
        int h = g.card_image[i]->height;
        glBindTexture(GL_TEXTURE_2D, g.card_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        unsigned char *card_data = buildAlphaData(&card_images[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, card_data);
        free(card_data);
    }
}
void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    
    joshua_init_opengl();    
    init_felttex();
    //init_card_textures();
    init_cards();

    glGenTextures(1, &g.bgTexture);
    
    // background
    glBindTexture(GL_TEXTURE_2D, g.bgTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img[0].width, img[0].height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, img[0].data);

    // felt
    g.felt_image = &img[1];
    glGenTextures(1, &g.felt_texture);
    int w = g.felt_image->width;
    int h = g.felt_image->height;
    glBindTexture(GL_TEXTURE_2D, g.felt_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 g.felt_image->data);
}


void init_sounds() { }

void init()
{
    int offset = 75;
    joshua_init();

    for (int i = 0; i < nbuttons; i++) {
        button[i].r.width = 200;
        button[i].r.height = 50;
        button[i].r.left = g.xres / 2 - button[i].r.width / 2;
        button[i].r.bot = g.yres / 2 - button[i].r.height / 2 + offset;
        button[i].r.right = button[i].r.left + button[i].r.width;
        button[i].r.top = button[i].r.bot + button[i].r.height;
        button[i].r.centerx = (button[i].r.left + button[i].r.right) / 2;
        button[i].r.centery = (button[i].r.bot + button[i].r.top) / 2;
        switch (i) {
            case 0:
                strcpy(button[i].text, "Roadrunner Racing");
                break;
            case 1:
                strcpy(button[i].text, "Blackjack");
                break;
            case 2:
                strcpy(button[i].text, "Poker");
                break;
            case 3:
                strcpy(button[i].text, "Cee-lo");
                break;
            case 4:
                strcpy(button[i].text, "Shop");
                break;
            case 5:
                strcpy(button[i].text, "Exit");
                break;
        }
        button[i].down = 0;
        button[i].click = 0;
        for (int j = 0; j < COLOR_SIZE; j++) {
            button[i].color[j] = jg.theme.primary1[j];
            button[i].hcolor[j] = jg.theme.secondary1[j];
            button[i].dcolor[j] = jg.theme.primary1[j] * 0.5f;
        }
        button[i].text_color = jg.theme.secondary2;
        offset -= 55;
    }
}

void mouse_click(int action)
{
    if (action == 1) {
        if (g.gamemode == MODE_MENU) {
            for (int i = 0; i < nbuttons; i++) {
                if (button[i].over) {
                    button[i].down  = 1;
                    button[i].click = 1;
                    if (i == 0) {
                        g.gamemode = MODE_RACING;
                    }
                    if (i == 1) {
                        g.gamemode = MODE_BLACKJACK;
                    }
                    if (i == 2) {
                        g.gamemode = MODE_POKER;
                    }
                    if (i == 3) {
                        g.gamemode = MODE_CEELO;
                    }
                    if (i == 4) {
                        g.gamemode = MODE_SHOP;
                    }
                    if (i == 5) {
                        g.done = 1;
                    }
                }
            }
        } else if (g.paused) {
            for (int i = 0; i < 3; i++) {
                if (jg.button[i].over) {
                    jg.button[i].down  = 1;
                    jg.button[i].click = 1;
                    if (i == 0) {
                        // unpause
                        g.paused = !g.paused;
                    }
                    if (i == 1) {
                        // go to menu
                        g.paused = 0;
                        g.gamemode = MODE_MENU;
                        jg.win = 0;
                        jg.setup = 0;
                        jg.chosen = 0;
                        // reset roadrunners
                        for (int i = 0; i < 3; i++) {
                            jg.rr[i].x = 250;
                            jg.rr[i].y = 250;
                            jg.rr[i].vel[0] = jg.rr[i].vel[1] = 0.0f;
                        }
                    }
                    if (i == 2) {
                        // exit
                        g.done = 1;
                    }
                }
            }
        } else if (g.gamemode == MODE_RACING) {
            for (int i = 3; i < 6; i++) {
                if (jg.button[i].over) {
                    jg.button[i].down  = 1;
                    jg.button[i].click = 1;
                    if (i == 3) {
                        jg.chosen = 0;
                        jg.setup = 1;
                    }
                    if (i == 4) {
                        jg.chosen = 1;
                        jg.setup = 1;
                    }
                    if (i == 5) {
                        jg.chosen = 2;
                        jg.setup = 1;
                    }
                }
            }
        } else if (g.gamemode == MODE_SHOP) {
            for (int i = 6; i < jg_nbuttons; i++) {
                if (jg.button[i].over) {
                    jg.button[i].down  = 1;
                    jg.button[i].click = 1;
                    if (i == 6) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = csub;
                            init();
                        }
                    }
                    if (i == 7) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = light;
                            init();
                        }
                    }
                    if (i == 8) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = dark;
                            init();
                        }
                    }
                    if (i == 9) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = strawberry;
                            init();
                        }
                    }
                    if (i == 10) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = charcoal;
                            init();
                        }
                    }
                    if (i == 11) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = mint;
                            init();
                        }
                    }
                    if (i == 12) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = royal;
                            init();
                        }
                    }
                    if (i == 13) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = peach;
                            init();
                        }
                    }
                    if (i == 14) {
                        if (money >= 100) {
                            money -= 100.0;
                            jg.theme = slate;
                            init();
                        }
                    }
                }
            }
        }
    }
    if (action == 2) {
        if (g.gamemode == MODE_MENU) {
            for (int i = 0; i < nbuttons; i++) {
                button[i].down = 0;
                button[i].click = 0;
            }
        } else if (g.paused) {
            for (int i = 0; i < 3; i++) {
                jg.button[i].down = 0;
                jg.button[i].click = 0;
            }
        } else if (g.gamemode == MODE_RACING) {
            for (int i = 3; i < 6; i++) {                       
                jg.button[i].down = 0;
                jg.button[i].click = 0;
            }
        } else if (g.gamemode == MODE_SHOP) {
            for (int i = 6; i < jg_nbuttons; i++) {                       
                jg.button[i].down = 0;
                jg.button[i].click = 0;
            }
        }
    }
}

void check_mouse(XEvent *e)
{
    // mouse moved?
    // button clicked?
    static int savex = 0;
    static int savey = 0;
    int x, y;
    lbutton = 0;
    rbutton = 0;
    if (e->type == ButtonRelease) {
        mouse_click(2);
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button == 1) {
            //Left button is down
            lbutton = 1;
        }
        if (e->xbutton.button == 3) {
            //Right button is down
            rbutton = 1;
        }
    }
    x = e->xbutton.x;
    y = e->xbutton.y;
    y = g.yres - y;
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        //Mouse moved
        savex = e->xbutton.x;
        savey = e->xbutton.y;
    }
    if (x == savex && y == savey)
        return;
    savex = x;
    savey = y;
    if (g.gamemode == MODE_MENU) {
        // mouse over buttons?
        for (int i = 0; i < nbuttons; i++) {
            button[i].over = 0;
            button[i].down = 0;
            if (x >= button[i].r.left &&
                x <= button[i].r.right &&
                y >= button[i].r.bot &&
                y <= button[i].r.top) {
                button[i].over = 1;
                break;
            }
        }
        if (lbutton)
            mouse_click(1);
        if (rbutton)
            mouse_click(1);
    } else if (g.paused) {
        for (int i = 0; i < 3; i++) {
            jg.button[i].over = 0;
            jg.button[i].down = 0;
            if (x >= jg.button[i].r.left &&
                    x <= jg.button[i].r.right &&
                    y >= jg.button[i].r.bot &&
                    y <= jg.button[i].r.top) {
                jg.button[i].over = 1;
                break;
            }
        }
        if (lbutton)
            mouse_click(1);
        if (rbutton)
            mouse_click(1);
    } else if (g.gamemode == MODE_RACING) {
        // mouse over buttons?
        for (int i = 3; i < 6; i++) {
            jg.button[i].over = 0;
            jg.button[i].down = 0;
            if (x >= jg.button[i].r.left &&
                x <= jg.button[i].r.right &&
                y >= jg.button[i].r.bot &&
                y <= jg.button[i].r.top) {
                jg.button[i].over = 1;
                break;
            }
        }
        if (lbutton)
            mouse_click(1);
        if (rbutton)
            mouse_click(1);
    } else if (g.gamemode == MODE_SHOP) {
        // mouse over buttons?
        for (int i = 6; i < jg_nbuttons; i++) {
            jg.button[i].over = 0;
            jg.button[i].down = 0;
            if (x >= jg.button[i].r.left &&
                x <= jg.button[i].r.right &&
                y >= jg.button[i].r.bot &&
                y <= jg.button[i].r.top) {
                jg.button[i].over = 1;
                break;
            }
        }
        if (lbutton)
            mouse_click(1);
        if (rbutton)
            mouse_click(1);
    }
}

extern int monique_show;
extern int db_show;

void check_keys(XEvent *e)
{
    // keyboard input
    static int shift = 0;
    if (e->type != KeyRelease && e->type != KeyPress)
        return;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift = 0;
        return;
    }
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift = 1;
        return;
    }
    (void)shift;
    switch (key) {
        case XK_Escape:
            if (g.gamemode == MODE_MENU)
                g.done = 1;
            else
                g.paused = !g.paused;
            break;

     case XK_space:
                g.gamecheck = true;
            break;
     case XK_p:
                p_pressed++;
                if (g.gamemode == MODE_POKER) {
                    show_db();
                    money += money_prize;

                } else {
                    break;
                }
            break;
     case XK_b:
                hit++;
                if (g.gamemode == MODE_BLACKJACK) {
                    mcarrilloFeature();
                    money += prize;

                } //else {
                    //break;
                //}
            //break;
    }
}

void physics()
{
    if (g.gamemode == MODE_MENU)
        return;
    if (g.paused) {
        // paused, dont do physics
    } else {
        // all physics go here
        if (jg.setup)
            joshua_physics();
    }
    /*
    if (!g.paused) {
        if (g.gamemode == MODE_RACING) {
            // roadrunner racing physics
            joshua_physics();
        }
        if (g.gamemode == MODE_BLACKJACK) {
            // blackjack physics go here
        }
        if (g.gamemode == MODE_POKER) {
            // poker physics go here
        }
    }
    */
}

void render_main_menu()
{
    Rect r;

    // background
    glBindTexture(GL_TEXTURE_2D, g.bgTexture);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0,      0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0,      g.yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    r.left = g.xres / 2;
    r.bot  = g.yres - g.yres / 6;
    r.center = 1;
    ggprint40(&r, 50, jg.theme.primary2, "Roadrunner Desert");
    ggprint40(&r, 0, jg.theme.primary2, "Casino");

    for (int i = 0; i < nbuttons; i++) {
        if (button[i].over) {
            glColor3fv(button[i].hcolor);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(button[i].r.left - 2,  button[i].r.bot - 2);
                glVertex2i(button[i].r.left - 2,  button[i].r.top + 2);
                glVertex2i(button[i].r.right + 2, button[i].r.top + 2);
                glVertex2i(button[i].r.right + 2, button[i].r.bot - 2);
            glEnd();
            glLineWidth(1);
        }
        if (button[i].down) {
            glColor3fv(button[i].dcolor);
        } else {
            glColor3fv(button[i].color);
        }
        glBegin(GL_QUADS);
            glVertex2i(button[i].r.left,  button[i].r.bot);
            glVertex2i(button[i].r.left,  button[i].r.top);
            glVertex2i(button[i].r.right, button[i].r.top);
            glVertex2i(button[i].r.right, button[i].r.bot);
        glEnd();
        r.left = button[i].r.centerx;
        r.bot  = button[i].r.centery-8;
        r.center = 1;
        if (button[i].down) {
            ggprint16(&r, 0, button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, button[i].text_color, button[i].text);
        }
    }
}

void render_cards()
{
    int xoffset = 0;
    int yoffset = 0;
    for (int i = 0; i < NCARDS; i++) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, g.card_texture[i]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(10 + xoffset,  
                    200 + yoffset);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(100 + xoffset,  
                    200 + yoffset);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(100 + xoffset, 
                    10 + yoffset);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(10 + xoffset, 
                    10 + yoffset);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        if (i % 6 == 0 && i != 0) {
            yoffset += 210;
            xoffset = 0;
        }
        xoffset += 110;
    }
}

void renduh_cards(int *position, int limit)
{
    int xoffset = 0;
    int yoffset = 0;
    if (limit == 5) {
        xoffset = 0;
        yoffset = 150;
    } else if (limit == 2) {
        xoffset = 0;
        yoffset = 50;
    }
    for (int i = 0; i < limit; i++) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, g.card_texture[position[i]]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(10 + xoffset,
                    200 + yoffset);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(100 + xoffset,
                    200 + yoffset);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(100 + xoffset,
                    10 + yoffset);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(10 + xoffset,
                    10 + yoffset);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        if (i % 6 == 0 && i != 0) {
            yoffset += 210;
            xoffset = 0;
        }
        xoffset += 110;
    }

}

void render()
{
    Rect r;
    // clear screen
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // render main menu
    if (g.gamemode == MODE_MENU) {
        // menu
        render_main_menu();
    } else if (g.gamemode == MODE_SHOP) {
        render_shop();
    } else if (g.gamemode == MODE_RACING) {
        // roadrunner racing
        if (!jg.setup)
            render_setup();
        else
            render_racing();
    } else if (g.gamemode == MODE_BLACKJACK) {
        // blackjack
        //mcarrillo_init();
        //mcarrillo_render();
        glColor3f(1.0, 1.0, 1.0); // pure white
        glBindTexture(GL_TEXTURE_2D, g.felt_texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0,      0);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0,      g.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, g.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, 0);
        glEnd();
        //render_cards();
        if (hit) {
            renduh_cards(deal, 2);
            renduh_cards(play, 2);
        }

        BJ(g.xres, g.yres);
        //mcarrilloFeature();
    } else if (g.gamemode == MODE_POKER) {
        // poker
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, g.felt_texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, g.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, g.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        if (p_pressed) {
            renduh_cards(mesa, 5);
            renduh_cards(jugador, 2);
        }
        render_poker(g.xres, g.yres);
    } else if (g.gamemode == MODE_CEELO) {
        glColor3f(1.0, 1.0, 1.0); // pure white
        glBindTexture(GL_TEXTURE_2D, g.felt_texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0,      0);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0,      g.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, g.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);


	Rect r, s;

        r.bot = g.yres - 30;
        r.left = 20;
        r.center = 0;
        ggprint13(&r, 16, 0x00FFFFFF, "Cee-lo");

        s.bot = g.yres - 50;
        s.left = 20;
        s.center = 0;
        ggprint13(&s, 16, 0x00FFFFFF, "press space to roll");


	float payout;
        float charge;
        if (money < 100) {
                payout = 40;
                charge = 10;
        }


        if (money > 100 && money < 500) {
                payout = 80;
                charge = 20;
        }

        if (money > 500 && money < 1000) {
                payout = 200;
                charge = 50;
        }

        if (money > 1000) {
                payout = 100;
                charge = 5;
        }





        int win = 0;
        render_word();
        if (g.gamecheck == true) {
            money = money - charge;
            win = ceelo(g.xres, g.yres, money);
            render_win(win);
            g.gamecheck = false;
        }
        if (win == 1) {
            money = money + payout;
        }
    }

    if (g.paused)
        render_pause_screen();

    r.left = 20;
    r.bot  = 20;
    r.center = 0;
    ggprint40(&r, 0, (money < 0) ? 0x00ff0000 : 0x0000ff00, "$%.2f", money);
}
