#include "Character.h"

int main() {
    if (!init() || !loadCharacter() || !loadMap()) {
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

        // Di chuyển trái/phải với kiểm tra va chạm
        if (isLeftPressed && character.x > 0) {
            int newX = character.x - CHARACTER_SPEED;
            if (!checkCollisionWithWalls(newX, character.y)) {
                character.x = newX;
            }
            character.direction = 2;
        }
        if (isRightPressed && character.x < SCREEN_WIDTH - 50) {
            int newX = character.x + CHARACTER_SPEED;
            if (!checkCollisionWithWalls(newX, character.y)) {
                character.x = newX;
            }
            character.direction = 3;
        }

        // Xử lý nhảy và rơi
        if (character.isJumping) {
            character.y += character.jumpVelocity;
            character.jumpVelocity += 1; // Trọng lực

            // Kiểm tra va chạm với tường khi nhảy hoặc rơi
            bool landed = false;
            for (const auto& wall : walls) {
                // Nếu rơi xuống và chạm mép trên của tường
                if (character.jumpVelocity > 0 && character.y + 80 >= wall.y1 && character.y + 80 <= wall.y2 &&
                    character.x + 50 > wall.x1 && character.x < wall.x2) {
                    character.y = wall.y1 - 50; // Đặt nhân vật trên tường
                    character.isJumping = false;
                    character.jumpVelocity = 0;
                    landed = true;
                    break;
                }
                // Nếu nhảy lên và chạm mép dưới của tường
                else if (character.jumpVelocity < 0 && character.y <= wall.y2 && character.y >= wall.y1 &&
                         character.x + 50 > wall.x1 && character.x < wall.x2) {
                    character.y = wall.y2; // Dừng lại dưới tường
                    character.jumpVelocity = 0; // Ngừng nhảy lên
                    break;
                }
            }

            // Nếu rơi xuống đáy màn hình
            if (!landed && character.y >= SCREEN_HEIGHT - 50) {
                character.y = SCREEN_HEIGHT - 50;
                character.isJumping = false;
                character.jumpVelocity = 0;
            }
        } else {
            // Kiểm tra xem nhân vật có đang đứng trên mặt đất hay tường không
            bool onSurface = (character.y == SCREEN_HEIGHT - 50); // Đáy màn hình
            for (const auto& wall : walls) {
                if (character.y + 50 == wall.y1 && character.x + 50 > wall.x1 && character.x < wall.x2) {
                    onSurface = true;
                    break;
                }
            }
            if (!onSurface) {
                character.isJumping = true; // Bắt đầu rơi nếu không đứng trên gì
                character.jumpVelocity = 0;
            }
        }

        // Cập nhật animation
        if (!character.isJumping && (isLeftPressed || isRightPressed)) {
            if (++frameCounter >= FRAME_DELAY) {
                frameCounter = 0;
                character.frame = (character.frame + 1) % 8;
            }
        }

        // Cập nhật đạn
        for (auto& bullet : bullets) {
            switch (bullet.direction) {
                case 2: bullet.x -= BULLET_SPEED; break;
                case 3: bullet.x += BULLET_SPEED; break;
            }
        }
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
            return bullet.x < 0 || bullet.x > SCREEN_WIDTH || bullet.y < 0 || bullet.y > SCREEN_HEIGHT;
        }), bullets.end());

        render();
        SDL_Delay(16);
    }

    close();
    return 0;
}
