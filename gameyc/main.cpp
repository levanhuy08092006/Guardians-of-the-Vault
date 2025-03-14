#include "Character.h"

int main() {
    if (!init() || !loadCharacter()) {
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

    // Di chuyển nhân vật
    if (isLeftPressed && character.x > 0) {
        character.x -= CHARACTER_SPEED;
        character.direction = 2; // Cập nhật hướng sang trái
    }
    if (isRightPressed && character.x < SCREEN_WIDTH - 80) {
        character.x += CHARACTER_SPEED;
        character.direction = 3; // Cập nhật hướng sang phải
    }

    // Xử lý nhảy
    if (character.isJumping) {
        character.y += character.jumpVelocity;
        character.jumpVelocity += 1; // Trọng lực
        if (character.y >= SCREEN_HEIGHT - 80) {
            character.y = SCREEN_HEIGHT - 80;
            character.isJumping = false;
        }
    }

    // Cập nhật animation
    if (!character.isJumping && (isLeftPressed || isRightPressed)) {
        if (++frameCounter >= FRAME_DELAY) {
            frameCounter = 0;
            character.frame = (character.frame + 1) % 8;
        }
    }
    // Cập nhật vị trí đạn
        for (auto& bullet : bullets) {
            switch (bullet.direction) {
                case 2: bullet.x -= BULLET_SPEED; break; // Sang trái
                case 3: bullet.x += BULLET_SPEED; break; // Sang phải
            }
        }

        // Xóa đạn nếu ra khỏi màn hình
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
            return bullet.x < 0 || bullet.x > SCREEN_WIDTH || bullet.y < 0 || bullet.y > SCREEN_HEIGHT;
        }), bullets.end());

    render();
    SDL_Delay(16); // 60 FPS
}

    close();
    return 0;
}
