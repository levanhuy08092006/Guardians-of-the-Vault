#ifndef CHARACTER_H
#define CHARACTER_H
//Tránh việc Character.h bị nạp nhiều lần trong cùng một chương trình.

#define SDL_MAIN_HANDLED
//SDL_MAIN_HANDLED: Ngăn SDL tự động xử lý main() trên một số hệ điều hành.
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int SCREEN_WIDTH = 1435;
const int SCREEN_HEIGHT = 700;
const int CHARACTER_SPEED = 4;
const int FRAME_DELAY = 10;
const int BULLET_SPEED = 10;
const int ENEMY_SPEED = 1;
const int SPAWN_INTERVAL = 2000; // 5000ms = 5 giây, thời gian giữa các lần spawn

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

struct Wall {
    int x1, y1; // Tọa độ điểm đầu
    int x2, y2; // Tọa độ điểm cuối
};

struct Enemy {
    int x, y;
    int targetIndex; // Chỉ số waypoint hiện tại
    vector<pair<int, int>> waypoints; // Danh sách các điểm mốc
    SDL_Texture* texture; // Texture của enemy
};

// Khai báo biến toàn cục
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern Character character;
extern bool isLeftPressed;
extern bool isRightPressed;
extern vector <Bullet> bullets;
extern vector<Wall> walls;
extern vector<Enemy> enemiesGroup1; // Nhóm 1
extern vector<Enemy> enemiesGroup2; // Nhóm 2
extern Uint32 lastSpawnTimeGroup1; // Thêm khai báo extern
extern Uint32 lastSpawnTimeGroup2; // Thêm khai báo extern
extern bool loadMap();
extern SDL_Texture* gameOverTexture; // Thêm texture cho gameover
extern bool gameOver; // Biến kiểm tra trạng thái gameover
extern Mix_Music* backgroundMusic; // Thêm biến cho nhạc nền

// Hàm khởi tạo, xử lý, render
bool init();
bool loadCharacter();
void handleEvent(SDL_Event& e);
void render();
void close();
void runGame();
SDL_Texture* loadTexture(const char* path);
extern SDL_Texture* mapTexture; // Thêm biến toàn cục để lưu texture của map
bool checkCollisionWithWalls(int newX, int newY);

#endif // CHARACTER_H
