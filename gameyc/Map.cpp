#include "Map.h"
#include "GameUtils.h"
#include <iostream>

SDL_Texture* mapTexture = nullptr;
std::vector<Wall> walls;

bool loadMap() {
    mapTexture = loadTexture("mapgame.png");
    if (!mapTexture) {
        std::cerr << "Failed to load map texture!" << std::endl;
        return false;
    }

    walls.push_back({170, 370, 803, 380});
    walls.push_back({702, 565, 820, 575});
    walls.push_back({920, 500, 1125, 510});
    walls.push_back({632, 622, 680, 632});
    walls.push_back({830, 428, 849, 438});
    walls.push_back({860, 530, 875, 540});
    walls.push_back({1155, 580, 1227, 590});

    return true;
}

bool checkCollisionWithWalls(int newX, int newY) {

    SDL_Rect characterRect = {newX, newY, CHARACTER_WIDTH, CHARACTER_HEIGHT};

    for (const auto& wall : walls) {
        SDL_Rect wallRect = {wall.x1, wall.y1, wall.x2 - wall.x1, wall.y2 - wall.y1};
        if (SDL_HasIntersection(&characterRect, &wallRect)) {
            return true;
        }
    }
    return false;
}
