#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 1435;
const int SCREEN_HEIGHT = 700;
const int CHARACTER_SPEED = 4;
const int FRAME_DELAY = 10;
const int BULLET_SPEED = 10;
const int ENEMY_SPEED = 1;
const int SPAWN_INTERVAL = 2000;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern Mix_Music* backgroundMusic;

bool init();
SDL_Texture* loadTexture(const char* path);
void close();

#endif // GAME_UTILS_H
