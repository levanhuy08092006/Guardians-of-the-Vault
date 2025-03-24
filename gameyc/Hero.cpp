#include "Hero.h"
#include "GameUtils.h"
#include <iostream>
using namespace std;

Hero hero;
bool isLeftPressed = false;
bool isRightPressed = false;
vector<Bullet> bullets;

bool loadHero() {
    const char* runRightFiles[8] = {"run1.png", "run2.png", "run3.png", "run4.png", "run5.png", "run6.png", "run7.png", "run8.png"};
    const char* runLeftFiles[8] = {"run11.png", "run21.png", "run31.png", "run41.png", "run51.png", "run61.png", "run71.png", "run81.png"};

    for (int i = 0; i < 8; ++i) {
        hero.runRight[i] = loadTexture(runRightFiles[i]);
        hero.runLeft[i] = loadTexture(runLeftFiles[i]);
        if (!hero.runRight[i] || !hero.runLeft[i]) return false;
    }

    hero.jump = loadTexture("bay.png");
    hero.jumpLeft = loadTexture("bay2.png");
    if (!hero.jump || !hero.jumpLeft) return false;

    hero.x = 50;
    hero.y = SCREEN_HEIGHT - 50;
    hero.direction = 3;
    hero.frame = 0;
    hero.moving = false;
    hero.isJumping = false;
    hero.jumpVelocity = 0;

    return true;
}

void handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (!hero.isJumping) {
                    hero.isJumping = true;
                    hero.jumpVelocity = -14;
                }
                break;
            case SDLK_LEFT:
                isLeftPressed = true;
                hero.direction = 2;
                break;
            case SDLK_RIGHT:
                isRightPressed = true;
                hero.direction = 3;
                break;
            case SDLK_SPACE:
                Bullet newBullet;
                if (hero.direction == 3) {
                    newBullet.x = hero.x + 45;
                    newBullet.y = hero.y + 5;
                } else if (hero.direction == 2) {
                    newBullet.x = hero.x;
                    newBullet.y = hero.y + 5;
                }
                newBullet.direction = hero.direction;
                bullets.push_back(newBullet);
                break;
        }
    }
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: isLeftPressed = false; break;
            case SDLK_RIGHT: isRightPressed = false; break;
        }
    }
}
