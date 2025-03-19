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

// Các hàm khác giữ nguyên
bool init() {
    //Khởi tạo SDL để xử lý đồ họa
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    //tạo cửa sổ có kích thước SCREEN_WIDTH x SCREEN_HEIGHT
    window = SDL_CreateWindow("Character Movement", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    /*SDL_CreateTextureFromSurface(renderer, loadedSurface): Chuyển ảnh từ SDL_Surface thành SDL_Texture để hiển thị lên màn hình.
     SDL_FreeSurface(loadedSurface): Giải phóng bộ nhớ của loadedSurface vì đã chuyển sang texture.*/
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
    // Xóa màn hình và đặt màu nền (nếu cần)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu đen
    SDL_RenderClear(renderer);
    // Render mapgame với kích thước phù hợp
    SDL_Rect mapRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // Kích thước bằng màn hình

    // Render mapgame
    if (mapTexture) {
        SDL_RenderCopy(renderer, mapTexture, nullptr, nullptr); // Render toàn bộ map lên màn hình
    }

    // Render nhân vật
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

    // Render đạn
    for (auto& bullet : bullets) {
        SDL_Rect bulletRect = {bullet.x, bullet.y, 8, 8}; // Kích thước đạn
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ
        SDL_RenderFillRect(renderer, &bulletRect);
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
    SDL_DestroyTexture(character.jump);     // Giải phóng ảnh nhảy khi đi sang phải
    SDL_DestroyTexture(character.jumpLeft); // Giải phóng ảnh nhảy khi đi sang trái

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

