#ifndef _FONTS_H_
#define _FONTS_H_
#define MAXBUTTONS 8
typedef struct t_rect {
	int left;
	int top;
	int right;
	int bot;
	int width;
	int height;
	int center;
	int centerx;
	int centery;
} Rect;

// source: bship.cpp framework
typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float hcolor[3];
    float dcolor[3];
    unsigned int text_color;
} Button;

extern void initialize_fonts(void);
extern void cleanup_fonts(void);
extern void ggprint16(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint12(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint13(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint10(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint08(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint07(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint06(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint8b(Rect *r, int advance, int cref, const char *fmt, ...);
extern void ggprint40(Rect *r, int advance, int cref, const char *fmt, ...);

extern int texState();
#endif //_FONTS_H_

