#ifndef HERO_H
#define HERO_H

#include <SDL.h>
#include <vector>

struct Hero {
    SDL_Texture* runRight[8];
    SDL_Texture* runLeft[8];
    SDL_Texture* jump;
    SDL_Texture* jumpLeft;
    int x, y;
    int direction; // 0: Up, 1: Down, 2: Left, 3: Right
    int frame;
    bool moving;
    bool isJumping;
    int jumpVelocity;
};

struct Bullet {
    int x, y;
    int direction;
};

extern Hero hero;
extern bool isLeftPressed;
extern bool isRightPressed;
extern std::vector<Bullet> bullets;

bool loadHero();
void handleEvent(SDL_Event& e);

#endif // HERO_H

