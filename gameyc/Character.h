#ifndef CHARACTER_H
#define CHARACTER_H
//Tránh việc Character.h bị nạp nhiều lần trong cùng một chương trình.

#define SDL_MAIN_HANDLED
//SDL_MAIN_HANDLED: Ngăn SDL tự động xử lý main() trên một số hệ điều hành.
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int CHARACTER_SPEED = 4;
const int FRAME_DELAY = 10;
const int BULLET_SPEED = 10;

struct Character {
    SDL_Texture* runRight[8]; // Animation chạy sang phải
    SDL_Texture* runLeft[8];  // Animation chạy sang trái
    SDL_Texture* jump;        // Ảnh nhảy khi di chuyển sang phải
    SDL_Texture* jumpLeft;    // Ảnh nhảy khi di chuyển sang trái
    int x, y;     // vị trí nhân vật
    int direction; // 0: Up, 1: Down, 2: Left, 3: Right
    int frame; //Chỉ số ảnh animation hiện tại.
    bool moving;
    bool isJumping;
    int jumpVelocity; //Vận tốc nhảy (giá trị âm để nhảy lên, dương để rơi xuống).
};
struct Bullet {
    int x, y;
    int direction; // 0: Up, 1: Down, 2: Left, 3: Right
};
// Khai báo biến toàn cục
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern Character character;
extern bool isLeftPressed;
extern bool isRightPressed;
extern vector <Bullet> bullets;
// Hàm khởi tạo, xử lý, render
bool init();
bool loadCharacter();
void handleEvent(SDL_Event& e);
void render();
void close();
void runGame();
SDL_Texture* loadTexture(const char* path);

#endif // CHARACTER_H
