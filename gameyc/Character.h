#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int CHARACTER_SPEED = 5;
const int FRAME_DELAY = 10;

struct Character {
    SDL_Texture* runLeft[8];
    SDL_Texture* runRight[8];
    SDL_Texture* up;
    SDL_Texture* down;
    int x, y;
    int direction;
    int frame;
    bool moving;
};
bool init();
bool loadCharacter();
void handleEvent();
void render();
void close();
void runGame();



#endif // CHARACTER_H_INCLUDED
