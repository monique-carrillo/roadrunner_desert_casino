//Josef Chicas
//Currently contains the basic functions needed to open a window
//need rework into title screen, implement support for Davids photo function for background images
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"

Display *dpy;
Window win;

int done=0;
int xres=640, yres=480;

void cleanupXWindows(void) {
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void) {
	//window title
	 XMapWindow(dpy, win);
         XStoreName(dpy, win, "Runner Casino");
}

void setup_screen_res(const int w, const int h) {
	xres = w;
	yres = h;
}

void initXWindows(void) {
	Window root;
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;

	setup_screen_res(640, 480);
	dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
						StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
							vi->depth, InputOutput, vi->visual,
							CWColormap | CWEventMask, &swa);
	GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	set_title();
}

void reshape_window(int width, int height) {
	setup_screen_res(width, height);
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	set_title();
}

void init_opengl(void) {
	glViewport(0, 0, xres, yres);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//2d mode
	glOrtho(0, xres, 0, yres, -1, 1);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void check_resize(XEvent *e) {
	//check if window is resized
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != xres || xce.height != yres) {
		reshape_window(xce.width, xce.height);
	}
}

void check_mouse(XEvent *e) {
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

void check_keys(XEvent *e)
{
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (key == XK_Escape) {
			done=1;
			return;
		}
	}
}

void render(void)
{



}
