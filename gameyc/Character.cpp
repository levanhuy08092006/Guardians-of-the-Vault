#include "Character.h"

// Định nghĩa biến toàn cục
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* mapTexture = nullptr;
SDL_Texture* currentTexture = nullptr;
Character character;
bool isLeftPressed = false;
bool isRightPressed = false;
vector<Bullet> bullets;
vector<Wall> walls;
vector<Enemy> enemiesGroup1; // Định nghĩa nhóm 1
vector<Enemy> enemiesGroup2; // Định nghĩa nhóm 2
Uint32 lastSpawnTimeGroup1 = 0; // Thời gian spawn cuối cùng cho nhóm 1
Uint32 lastSpawnTimeGroup2 = 0; // Thời gian spawn cuối cùng cho nhóm 2
SDL_Texture* gameOverTexture = nullptr; // Định nghĩa texture gameover
bool gameOver = false; // Trạng thái gameover ban đầu là false
Mix_Music* backgroundMusic = nullptr; // Định nghĩa nhạc nền

// Các hàm khác giữ nguyên
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) { // Thêm SDL_INIT_AUDIO
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        return false;
    }
    //tạo cửa sổ có kích thước SCREEN_WIDTH x SCREEN_HEIGHT
    window = SDL_CreateWindow("protect the treasure", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    //tạo bộ vẽ (renderer) để vẽ lên cửa sổ
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    //Khởi tạo SDL_image để hỗ trợ ảnh PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (!texture) {
        cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << endl;
    }
    return texture;
}

bool loadMap() {
    mapTexture = loadTexture("mapgame.png");
    if (!mapTexture) {
        cerr << "Failed to load map texture!" << endl;
        return false;
    }

    // Định nghĩa tường với độ dày (chiều cao)
    walls.push_back({170, 370, 803, 380}); // Tường 1: cao 10 pixel (392 -> 402)
    walls.push_back({702, 565, 820, 575}); // Tường 2: cao 10 pixel (565 -> 575)
    walls.push_back({920, 500, 1125, 510});//3
    walls.push_back({632, 622, 680, 632});//4
    walls.push_back({830, 428, 849, 438});//5
    walls.push_back({860, 530, 875, 540});//6
    walls.push_back({1155, 580, 1227, 590});//7

    return true;
}
bool loadCharacter() {


    const char* runRightFiles[8] = {"run1.png", "run2.png", "run3.png", "run4.png", "run5.png", "run6.png", "run7.png", "run8.png"};
    const char* runLeftFiles[8] = {"run11.png", "run21.png", "run31.png", "run41.png", "run51.png", "run61.png", "run71.png", "run81.png"};

    for (int i = 0; i < 8; ++i) {
        character.runRight[i] = loadTexture(runRightFiles[i]);
        character.runLeft[i] = loadTexture(runLeftFiles[i]);
        if (!character.runRight[i] || !character.runLeft[i]) return false;
    }

    character.jump = loadTexture("bay.png");      // Ảnh nhảy khi di chuyển sang phải
    character.jumpLeft = loadTexture("bay2.png"); // Ảnh nhảy khi di chuyển sang trái
    if (!character.jump || !character.jumpLeft) return false;

    character.x = 50;
    character.y = SCREEN_HEIGHT-50;
    character.direction = 3; // Mặc định hướng sang phải
    character.frame = 0;
    character.moving = false;
    character.isJumping = false;
    character.jumpVelocity = 0;

    // Tải texture gameover
    gameOverTexture = loadTexture("gameover.png");
    if (!gameOverTexture) {
        cerr << "Failed to load gameover.png!" << endl;
        return false;
    }
    // Tải và phát nhạc nền
    backgroundMusic = Mix_LoadMUS("musicgame.wav");
    if (!backgroundMusic) {
        cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << endl;
        return false;
    }
    Mix_PlayMusic(backgroundMusic, -1); // Phát lặp vô hạn

// Khởi tạo nhóm 1
    vector<pair<int, int>> waypoints1 = {{225, 335}, {945, 335}, {945, 470}, {906, 470}, {821, 520}, {680, 570}, {580, 665}, {140, 665}};
        Enemy enemy1;
        enemy1.x = 225;
        enemy1.y = 250; // Đặt trên mặt đất (giả sử chiều cao enemy là 60)
        enemy1.targetIndex = 0;
        enemy1.waypoints = waypoints1;
        enemy1.texture = loadTexture("enemy.png");
        if (!enemy1.texture) {
            cerr << "Failed to load enemy.png!" << endl;
            return false;
        }
        enemiesGroup1.push_back(enemy1);

// Khởi tạo nhóm 2
    vector<pair<int, int>> waypoints2 = {{1065, 665}, {140, 665}};
        Enemy enemy2;
        enemy2.x = 1065;
        enemy2.y = 665 ; // Đặt trên mặt đất
        enemy2.targetIndex = 0;
        enemy2.waypoints = waypoints2;
        enemy2.texture = loadTexture("enemy.png");
        if (!enemy2.texture) {
        cerr << "Failed to load enemy.png!" << endl;
        return false;
    }
        enemiesGroup2.push_back(enemy2);
        return true;
}

void handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (!character.isJumping) {
                    character.isJumping = true;
                    character.jumpVelocity = -14; // Tốc độ nhảy ban đầu
                }
                break;;
            case SDLK_LEFT:
                isLeftPressed = true;
                character.direction = 2; // Cập nhật hướng sang trái
                break;
            case SDLK_RIGHT:
                isRightPressed = true;
                character.direction = 3; // Cập nhật hướng sang phải
                break;
            case SDLK_SPACE: // Bắn đạn khi nhấn phím Space
                Bullet newBullet;
                    if (character.direction == 3) { // Nếu nhân vật đang quay sang phải
                newBullet.x = character.x + 45; // Vị trí xuất phát của đạn
                newBullet.y = character.y + 5;
                    } else if (character.direction == 2) { // Nếu nhân vật đang quay sang trái
                newBullet.x = character.x; // Vị trí xuất phát của đạn
                newBullet.y = character.y + 5;
                    }
                newBullet.direction = character.direction; // Hướng đạn theo hướng nhân vật
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

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (!gameOver) {
        // Render bình thường nếu chưa gameover
        if (mapTexture) {
            SDL_RenderCopy(renderer, mapTexture, nullptr, nullptr);
        }

        SDL_Texture* currentTexture = nullptr;
        if (character.isJumping) {
            if (character.direction == 2) {
                currentTexture = character.jumpLeft;
            } else {
                currentTexture = character.jump;
            }
        } else {
            if (character.direction == 2) {
                currentTexture = character.runLeft[character.frame];
            } else {
                currentTexture = character.runRight[character.frame];
            }
        }

        SDL_Rect destRect = {character.x, character.y, 50, 50};
        SDL_RenderCopy(renderer, currentTexture, nullptr, &destRect);

        for (auto& bullet : bullets) {
            SDL_Rect bulletRect = {bullet.x, bullet.y, 8, 8};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &bulletRect);
        }

        for (auto& enemy : enemiesGroup1) {
            SDL_Rect enemyRect = {enemy.x, enemy.y, 60, 60};
            if (enemy.texture) {
                SDL_RenderCopy(renderer, enemy.texture, nullptr, &enemyRect);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &enemyRect);
            }
        }

        for (auto& enemy : enemiesGroup2) {
            SDL_Rect enemyRect = {enemy.x, enemy.y, 60, 60};
            if (enemy.texture) {
                SDL_RenderCopy(renderer, enemy.texture, nullptr, &enemyRect);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &enemyRect);
            }
        }
    } else {
        // Render ảnh gameover khi game kết thúc
        if (gameOverTexture) {
            SDL_Rect gameOverRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // Full màn hình
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
        }
    }

    SDL_RenderPresent(renderer);
}
bool checkCollisionWithWalls(int newX, int newY) {
    int characterWidth = 50;
    int characterHeight = 50;
    SDL_Rect characterRect = {newX, newY, characterWidth, characterHeight};

    for (const auto& wall : walls) {
        SDL_Rect wallRect = {wall.x1, wall.y1, wall.x2 - wall.x1, wall.y2 - wall.y1};
        if (SDL_HasIntersection(&characterRect, &wallRect)) {
            return true; // Có va chạm
        }
    }
    return false; // Không có va chạm
}

void close() {
    if (mapTexture) {
        SDL_DestroyTexture(mapTexture);
        mapTexture = nullptr;
    }
    for (int i = 0; i < 8; ++i) {
        SDL_DestroyTexture(character.runRight[i]); // Giải phóng texture chạy sang phải
        SDL_DestroyTexture(character.runLeft[i]);  // Giải phóng texture chạy sang trái
    }
    // Giải phóng texture của enemies
    for (auto& enemy : enemiesGroup1) {
        SDL_DestroyTexture(enemy.texture);
    }
    for (auto& enemy : enemiesGroup2) {
        SDL_DestroyTexture(enemy.texture);
    }
    SDL_DestroyTexture(character.jump);     // Giải phóng ảnh nhảy khi đi sang phải
    SDL_DestroyTexture(character.jumpLeft); // Giải phóng ảnh nhảy khi đi sang trái

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio(); // Đóng SDL_mixer
    IMG_Quit();
    SDL_Quit();
}

