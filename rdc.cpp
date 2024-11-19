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

int lbutton = 0;
int rbutton = 0;

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//macros
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define ALPHA 1

//Setup timers
//clock_gettime(CLOCK_REALTIME, &timePause);
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

Image img[1] = {   
    background_time()
};

class Global {
    public:
    int done;
    int xres, yres;
    int menu;
    int status;
    GLuint bgTexture;
    Global() {
        menu = 1;
        status = 0;
        done = 0;
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
        GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        XSetWindowAttributes swa;
        setupScreenRes(640, 480);
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
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                            StructureNotifyMask | SubstructureNotifyMask;
        win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
                                vi->depth, InputOutput, vi->visual,
                                CWColormap | CWEventMask, &swa);
        GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        glXMakeCurrent(dpy, win, glc);
        setTitle();
    }
    ~X11_wrapper() {
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }
    void setTitle() {
        //Set the window title bar.
        XMapWindow(dpy, win);
        XStoreName(dpy, win, "Roadrunner Desert Casino");
    }
    void setupScreenRes(const int w, const int h) {
        g.xres = w;
        g.yres = h;
    }
    void reshapeWindow(int width, int height) {
        //window has been resized.
        setupScreenRes(width, height);
        //
        glViewport(0, 0, (GLint)width, (GLint)height);
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        glOrtho(0, g.xres, 0, g.yres, -1, 1);
        setTitle();
    }
    void checkResize(XEvent *e) {
        //The ConfigureNotify is sent by the
        //server if the window is resized.
        if (e->type != ConfigureNotify)
            return;
        XConfigureEvent xce = e->xconfigure;
        if (xce.width != g.xres || xce.height != g.yres) {
            //Window size did change.
            reshapeWindow(xce.width, xce.height);
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

void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void init();
void physics(void);
void render(void);

int main()
{
    init_opengl();
    init();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    
    int done = 0;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            check_mouse(&e);
            done = check_keys(&e);

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

void initSounds() { }

void init()
{
    joshua_init();
}

void check_mouse(XEvent *e)
{
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button == 1) {
            //Left button is down
        }
        if (e->xbutton.button == 3) {
            //Right button is down
        }
    }

    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        //Mouse moved
        savex = e->xbutton.x;
        savey = e->xbutton.y;
    }
}

extern int monique_show;
extern int db_show;

int check_keys(XEvent *e)
{
    //keyboard input?
    static int shift = 0;
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift = 0;
        return 0;
    }
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift = 1;
        return 0;
    }
    switch (key) {
        case XK_o:
            if (shift)
                jg.outline = !jg.outline;
            break;
        case XK_b:
            monique_show = !monique_show;
            break;
        case XK_g:
            db_show = !db_show;
            break;
        case XK_Up:
            g.menu--;
            if (g.menu < 1) {
                g.menu = 1;
            }
            break;
        case XK_Down:
            g.menu++;
            if (g.menu > 4) {
                g.menu = 4;
            }
            break;
        case XK_Escape:
            g.status = !g.status;
            break;
        case XK_Return:
            int press = g.menu;
            if (press == 4) {
                return 1;
            }
            if (press == 3) {
                joshua_features = !joshua_features;
            }
    }
    return 0;
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
    if (!g.status) {
        if (joshua_features)
            joshua_physics();
    }
}
extern void show_my_feature_db(int, int);

void render()
{
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
   
    glEnable(GL_TEXTURE_2D);
    
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.bgTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
    glEnd();

    if (monique_show) {
       mcarrilloFeature();
    }
    if (db_show == 1) {
        show_my_feature_db(30, g.yres - 80);
    }
    
    render_main_menu(g.yres, g.xres, g.menu);
    glDisable(GL_TEXTURE_2D);
    
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    if (joshua_features)
        joshua_render(g.xres, g.yres, g.status);

    glDisable(GL_BLEND);
}
