#ifndef RENDER_H
#define RENDER_H

#include "GameUtils.h"
#include "Hero.h"
#include <vector>
#include <utility>

struct Enemy {
    int x, y;
    int targetIndex;
    std::vector<std::pair<int, int>> waypoints;
    SDL_Texture* texture;
};

extern std::vector<Enemy> enemiesGroup1;
extern std::vector<Enemy> enemiesGroup2;
extern Uint32 lastSpawnTimeGroup1;
extern Uint32 lastSpawnTimeGroup2;
extern SDL_Texture* gameOverTexture;
extern bool gameOver;

void render();

#endif // RENDER_H
