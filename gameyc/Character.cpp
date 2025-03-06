#include "Character.h"
#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Character character;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    window = SDL_CreateWindow("Character Movement", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    return true;
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

bool loadCharacter() {
    const char* runLeftFiles[8] = {"run11.png", "run21.png", "run31.png", "run41.png", "run51.png", "run61.png", "run71.png", "run81.png"};
    const char* runRightFiles[8] = {"run1.png", "run2.png", "run3.png", "run4.png", "run5.png", "run6.png", "run7.png", "run8.png"};

    for (int i = 0; i < 8; ++i) {
        character.runLeft[i] = loadTexture(runLeftFiles[i]);
        character.runRight[i] = loadTexture(runRightFiles[i]);
        if (!character.runLeft[i] || !character.runRight[i]) return false;
    }

    character.up = loadTexture("bay.png");
    character.down = loadTexture("bay.png");
    if (!character.up || !character.down) return false;

    character.x = SCREEN_WIDTH / 2;
    character.y = SCREEN_HEIGHT / 2;
    character.direction = 1;
    character.frame = 0;
    character.moving = false;
    return true;
}

void handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        character.moving = true;
        switch (e.key.keysym.sym) {
            case SDLK_UP: if (character.y > 0) character.y -= CHARACTER_SPEED; character.direction = 0; break;
            case SDLK_DOWN: if (character.y < SCREEN_HEIGHT - 80) character.y += CHARACTER_SPEED; character.direction = 1; break;
            case SDLK_LEFT: if (character.x > 0) character.x -= CHARACTER_SPEED; character.direction = 2; break;
            case SDLK_RIGHT: if (character.x < SCREEN_WIDTH - 80) character.x += CHARACTER_SPEED; character.direction = 3; break;
        }
    }
    if (e.type == SDL_KEYUP) {
        character.moving = false;
    }
}

void render() {
    SDL_RenderClear(renderer);
    SDL_Texture* currentTexture = (character.direction == 0 || character.direction == 1) ? character.up : (character.direction == 2 ? character.runLeft[character.frame] : character.runRight[character.frame]);
    SDL_Rect destRect = {character.x, character.y, 80, 80};
    SDL_RenderCopy(renderer, currentTexture, nullptr, &destRect);
    SDL_RenderPresent(renderer);
}

void close() {
    for (int i = 0; i < 8; ++i) {
        SDL_DestroyTexture(character.runLeft[i]);
        SDL_DestroyTexture(character.runRight[i]);
    }
    SDL_DestroyTexture(character.up);
    SDL_DestroyTexture(character.down);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void runGame() {
    if (!init() || !loadCharacter()) return;

    bool quit = false;
    SDL_Event e;
    int frameCounter = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            handleEvent(e);
        }

        if (character.moving && (character.direction == 2 || character.direction == 3)) {
            if (++frameCounter >= FRAME_DELAY) {
                frameCounter = 0;
                character.frame = (character.frame + 1) % 8;
            }
        }

        render();
        SDL_Delay(16); // 60 FPS
    }

    close();
}
