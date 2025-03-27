#ifndef MAP_H
#define MAP_H

#include "GameUtils.h"
#include <vector>

struct Wall {
    int x1, y1;
    int x2, y2;
};

extern SDL_Texture* mapTexture;
extern std::vector<Wall> walls;

bool loadMap();
bool checkCollisionWithWalls(int newX, int newY);

#endif
