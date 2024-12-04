#ifndef MCARRILLO_H
#define MCARRILLO_H

extern float prize;
extern int hit;
extern int deal[2];
extern int play[2];

void BJ(int x_res, int y_res);
void initializeDeck();
void shuffleDeck();
void mcarrillo_init();
void mcarrillo_render();
void mcarrilloFeature();

#endif // MCARRILLO_H

