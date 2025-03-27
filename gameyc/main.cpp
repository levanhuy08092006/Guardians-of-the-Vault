#define SDL_MAIN_HANDLED
#include "Hero.h"
#include "GameUtils.h"
#include "Map.h"
#include "Render.h"
#include <iostream>
using namespace std;

int main() {
    if (!init() || !loadHero() || !loadMap()) {
        cerr << "Failed to initialize!" << endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;
    int frameCounter = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            handleEvent(e);
        }

        if (!gameOver) {
            if (isLeftPressed && hero.x > 0) {
                int newX = hero.x - CHARACTER_SPEED;
                if (!checkCollisionWithWalls(newX, hero.y)) {
                    hero.x = newX;
                }
                hero.direction = 2;
            }
            if (isRightPressed && hero.x < SCREEN_WIDTH - 50) {
                int newX = hero.x + CHARACTER_SPEED;
                if (!checkCollisionWithWalls(newX, hero.y)) {
                    hero.x = newX;
                }
                hero.direction = 3;
            }
            if (hero.isJumping) {
                hero.y += hero.jumpVelocity;
                hero.jumpVelocity += 1;

                bool landed = false;
                for (const auto& wall : walls) {
                    if (hero.jumpVelocity > 0 && hero.y + 50 >= wall.y1 && hero.y + 50 <= wall.y2 &&
                        hero.x + 50 > wall.x1 && hero.x < wall.x2) {
                        hero.y = wall.y1 - 50;
                        hero.isJumping = false;
                        hero.jumpVelocity = 0;
                        landed = true;
                        break;
                    }
                    /*else if (hero.jumpVelocity < 0 && hero.y <= wall.y2 && hero.y >= wall.y1 &&
                             hero.x + 50 > wall.x1 && hero.x < wall.x2) {
                        hero.y = wall.y2;
                        hero.jumpVelocity = 0;
                        break;
                    }*/
                }
                if (!landed && hero.y >= SCREEN_HEIGHT - 50) {
                    hero.y = SCREEN_HEIGHT - 50;
                    hero.isJumping = false;
                    hero.jumpVelocity = 0;
                }
            } else {
                bool onSurface = (hero.y == SCREEN_HEIGHT - 50);
                for (const auto& wall : walls) {
                    if (hero.y + 50 == wall.y1 && hero.x + 50 > wall.x1 && hero.x < wall.x2) {
                        onSurface = true;
                        break;
                    }
                }
                if (!onSurface) {
                    hero.isJumping = true;
                    hero.jumpVelocity = 0;
                }
            }
            if (!hero.isJumping && (isLeftPressed || isRightPressed)) {
                if (++frameCounter >= FRAME_DELAY) {
                    frameCounter = 0;
                    hero.frame = (hero.frame + 1) % 8;
                }
            } else {
                hero.frame = 0; // Đặt lại frame về 0 khi không di chuyển
            }
        }

        render();
        SDL_Delay(16);
    }

    close();
    return 0;
}
