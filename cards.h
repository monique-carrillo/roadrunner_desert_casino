#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <iostream>
#include <string.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>  // Use Apple's OpenGL framework
#else
//#include <windows.h>
#include <GL/gl.h>      // Use standard OpenGL on other systems
#endif

using namespace std;

class Card
{
    public:
        // Constructors
        Card();
        Card(char ss, char vv);

        // Modifiers
        void setSuit(char ss);
        void setValue(char vv);

        // Accessors
        char getSuit();
        char getValue();

        // Other methods
        void suit();
        static void drawPolygon(int xMin, int xMax, int yMin, int yMax);
        void draw(int x, int y);

    private:
        // Attributes
        char s, v;
};

// Default constructor
Card::Card() : s('z'), v('z') {}

// Constructor with suit and value parameters
Card::Card(char ss, char vv) : s(ss), v(vv) {}

// Modifier methods
void Card::setSuit(char ss) {
    s = ss;
}

void Card::setValue(char vv) {
    v = vv;
}

// Accessor methods
char Card::getSuit() {
    return s;
}

char Card::getValue() {
    return v;
}

// Display suit and value
void Card::suit() {
    cout << s << ":" << v << endl;
}

// Draw a polygon (rectangle) for the card
void Card::drawPolygon(int xMin, int xMax, int yMin, int yMax) {
    glBegin(GL_POLYGON);
    glVertex2d(xMin, yMin);
    glVertex2d(xMax, yMin);
    glVertex2d(xMax, yMax);
    glVertex2d(xMin, yMax);
    glEnd();
}

// Draw the card at a given position
void Card::draw(int x, int y) {
    glColor3ub(0, 0, 255);  // Set color to blue
    drawPolygon(x, x + 60, y, y + 85);
}

#endif // CARD_H_INCLUDED

