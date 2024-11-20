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
#include "images.h"
#include "mcarrillo.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//macros
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define ALPHA 1

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

#define MAXBUTTONS 8
Button button[MAXBUTTONS];
int nbuttons = 0;

Image img[1] = { background_time() };

void init_opengl(void);
void mouse_click(int);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void init();
void physics(void);
void render_mm(void);
void render(void);

enum {
    MODE_MENU = 0,
    MODE_RACING,
    MODE_BLACKJACK,
    MODE_POKER
};

class Global {
    public:
        int xres, yres;
        int paused;
        int gamemode;
        int done;
        GLuint bgTexture;
        Global() {
            done = 0;
            gamemode = MODE_MENU;
            paused = 0;
            xres = 800;
            yres = 600;
        }
} g;

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
            g.xres = w;
            g.yres = h;
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
        void swapBuffers() {
            glXSwapBuffers(dpy, win);
        }
} x11;


int main()
{
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
        x11.swapBuffers();
    }
    cleanup_fonts();
    return 0;
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
    initialize_fonts();
    
    joshua_init_opengl();    

    glGenTextures(1, &g.bgTexture);
    
    // background
    glBindTexture(GL_TEXTURE_2D, g.bgTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img[0].width, img[0].height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, img[0].data);
    
}

void init_sounds() { }

void init()
{
    int offset = 0;
    joshua_init();

    nbuttons=0;
    // roadrunner racing
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = g.xres / 2 - button[nbuttons].r.width / 2;
    button[nbuttons].r.bot = g.yres / 2 - button[nbuttons].r.height / 2;
    button[nbuttons].r.right = button[nbuttons].r.left +
                               button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot +
                             button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left +
                                  button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot +
                                  button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Roadrunner Racing");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.0f;
    button[nbuttons].color[1] = 0.2f;
    button[nbuttons].color[2] = 0.6f;
    button[nbuttons].hcolor[0] = 1.0f;
    button[nbuttons].hcolor[1] = 0.78f;
    button[nbuttons].hcolor[2] = 0.17f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffc72c;
    nbuttons++;
    offset -= 60;
    
    // blackjack
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = g.xres / 2 - button[nbuttons].r.width / 2;
    button[nbuttons].r.bot = g.yres / 2 - button[nbuttons].r.height / 2 + offset;
    button[nbuttons].r.right = button[nbuttons].r.left +
                               button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot +
                             button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left +
                                  button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot +
                                  button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Blackjack");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.0f;
    button[nbuttons].color[1] = 0.2f;
    button[nbuttons].color[2] = 0.6f;
    button[nbuttons].hcolor[0] = 1.0f;
    button[nbuttons].hcolor[1] = 0.78f;
    button[nbuttons].hcolor[2] = 0.17f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffc72c;
    nbuttons++;
    offset -= 60;
 
    // poker
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = g.xres / 2 - button[nbuttons].r.width / 2;
    button[nbuttons].r.bot = g.yres / 2 - button[nbuttons].r.height / 2 + offset;
    button[nbuttons].r.right = button[nbuttons].r.left +
                               button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot +
                             button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left +
                                  button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot +
                                  button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Poker");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.0f;
    button[nbuttons].color[1] = 0.2f;
    button[nbuttons].color[2] = 0.6f;
    button[nbuttons].hcolor[0] = 1.0f;
    button[nbuttons].hcolor[1] = 0.78f;
    button[nbuttons].hcolor[2] = 0.17f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffc72c;
    nbuttons++;
    offset -= 60;

    // exit
    button[nbuttons].r.width = 200;
    button[nbuttons].r.height = 50;
    button[nbuttons].r.left = g.xres / 2 - button[nbuttons].r.width / 2;
    button[nbuttons].r.bot = g.yres / 2 - button[nbuttons].r.height / 2 + offset;
    button[nbuttons].r.right = button[nbuttons].r.left +
                               button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot +
                             button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left +
                                  button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot +
                                  button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Exit");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.0f;
    button[nbuttons].color[1] = 0.2f;
    button[nbuttons].color[2] = 0.6f;
    button[nbuttons].hcolor[0] = 1.0f;
    button[nbuttons].hcolor[1] = 0.78f;
    button[nbuttons].hcolor[2] = 0.17f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffc72c;
    nbuttons++;
}

void mouse_click(int action)
{
    if (action == 1) {
        for (int i = 0; i < nbuttons; i++) {
            if (button[i].over) {
                button[i].down  = 1;
                button[i].click = 1;
                if (i == 0) {
                    // raodrunner racing
                    g.gamemode = MODE_RACING;
                }
                if (i == 1) {
                    // blackjack
                    g.gamemode = MODE_BLACKJACK;
                }
                if (i == 2) {
                    // poker
                    g.gamemode = MODE_POKER;
                }
                if (i == 3) {
                    // exit
                    g.done = 1;
                }
            }
        }
    }
    if (action == 2) {
        for (int i = 0; i < nbuttons; i++) {
            button[i].down = 0;
            button[i].click = 0;
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
        shift=1;
        return;
    }
    (void)shift;
    switch (key) {
        case XK_o:
            jg.outline = !jg.outline;
            break;
        case XK_b:
            monique_show = !monique_show;
            break;
        case XK_g:
            db_show = !db_show;
            break;
        case XK_Escape:
            if (g.gamemode == MODE_MENU)
                g.done = 1;
            else
                g.paused = !g.paused;
            break;
    }
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen*xlen + ylen*ylen + zlen*zlen;
    if (len == 0.0) {
        MakeVector(0.0,0.0,1.0,vec);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return(len);
}
void physics()
{
    if (g.gamemode == MODE_MENU)
        return;
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
}

void render_mm()
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
    r.bot  = g.yres - g.yres / 4;
    ggprint40(&r, 0, 0x000000ff, "Main Menu");

    for (int i = 0; i < nbuttons; i++) {
        if (button[i].over) {
            glColor3fv(button[i].hcolor);
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
                glVertex2i(button[i].r.left - 2,  button[i].r.bot - 2);
                glVertex2i(button[i].r.left - 2,  button[i].r.top + 2);
                glVertex2i(button[i].r.right + 2, button[i].r.top + 2);
                glVertex2i(button[i].r.right + 2, button[i].r.bot - 2);
                glVertex2i(button[i].r.left - 2,  button[i].r.bot - 2);
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

void render()
{
    // clear screen
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // render main menu
    if (g.gamemode == MODE_MENU) {
        // menu
        render_mm();
    } else if (g.gamemode == MODE_RACING) {
        // roadrunner racing
        render_racing(g.xres, g.yres);
    } else if (g.gamemode == MODE_BLACKJACK) {
        // blackjack
    } else if (g.gamemode == MODE_POKER) {
        // poker
    }

    if (g.paused)
        render_pause_screen(g.xres, g.yres);
    
}
