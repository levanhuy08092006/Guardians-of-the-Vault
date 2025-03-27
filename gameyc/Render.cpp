#include "Render.h"
#include "Map.h"
#include <algorithm>
using namespace std;

vector<Enemy> enemiesGroup1;
vector<Enemy> enemiesGroup2;
Uint32 lastSpawnTimeGroup1 = 0;
Uint32 lastSpawnTimeGroup2 = 0;
SDL_Texture* gameOverTexture = nullptr;
bool gameOver = false;
SDL_Texture* winGameTexture = nullptr;
bool gameWon = false;

void render() {
    Uint32 currentTime = SDL_GetTicks();

    if (!gameWon && !gameOver) {
        if (currentTime - lastSpawnTimeGroup1 >= SPAWN_INTERVAL) {
            Enemy newEnemy;
            newEnemy.x = 225;
            newEnemy.y = 250;
            newEnemy.targetIndex = 0;
            newEnemy.waypoints = {{225, 335}, {945, 335}, {945, 470}, {906, 470}, {821, 520}, {680, 570}, {580, 665}, {140, 665}};
            newEnemy.texture = loadTexture("enemy.png");
            if (newEnemy.texture) {
                enemiesGroup1.push_back(newEnemy);
                lastSpawnTimeGroup1 = currentTime;
            }
        }
        if (currentTime - lastSpawnTimeGroup2 >= SPAWN_INTERVAL) {
            Enemy newEnemy;
            newEnemy.x = 1065;
            newEnemy.y = 665;
            newEnemy.targetIndex = 0;
            newEnemy.waypoints = {{1065, 665}, {140, 665}};
            newEnemy.texture = loadTexture("enemy.png");
            if (newEnemy.texture) {
                enemiesGroup2.push_back(newEnemy);
                lastSpawnTimeGroup2 = currentTime;
            }
        }

        for (auto enemyIt = enemiesGroup1.begin(); enemyIt != enemiesGroup1.end();) {
            if (enemyIt->targetIndex < enemyIt->waypoints.size()) {
                int targetX = enemyIt->waypoints[enemyIt->targetIndex].first;
                int targetY = enemyIt->waypoints[enemyIt->targetIndex].second - 40;
                if (enemyIt->x < targetX) enemyIt->x += ENEMY_SPEED;
                else if (enemyIt->x > targetX) enemyIt->x -= ENEMY_SPEED;
                if (enemyIt->y < targetY) enemyIt->y += ENEMY_SPEED;
                else if (enemyIt->y > targetY) enemyIt->y -= ENEMY_SPEED;

                if (abs(enemyIt->x - targetX) < ENEMY_SPEED && abs(enemyIt->y - targetY) < ENEMY_SPEED) {
                    enemyIt->targetIndex++;
                }

                SDL_Rect enemyRect = {enemyIt->x, enemyIt->y, 60, 60};
                SDL_Rect endPointRect = {140, 665, 10, 10};
                if (SDL_HasIntersection(&enemyRect, &endPointRect)) {
                    gameOver = true;
                    break;
                }
                ++enemyIt;
            } else {
                SDL_DestroyTexture(enemyIt->texture);
                enemyIt = enemiesGroup1.erase(enemyIt);
            }
        }

        for (auto enemyIt = enemiesGroup2.begin(); enemyIt != enemiesGroup2.end();) {
            if (enemyIt->targetIndex < enemyIt->waypoints.size()) {
                int targetX = enemyIt->waypoints[enemyIt->targetIndex].first;
                int targetY = enemyIt->waypoints[enemyIt->targetIndex].second - 40;
                if (enemyIt->x < targetX) enemyIt->x += ENEMY_SPEED;
                else if (enemyIt->x > targetX) enemyIt->x -= ENEMY_SPEED;
                if (enemyIt->y < targetY) enemyIt->y += ENEMY_SPEED;
                else if (enemyIt->y > targetY) enemyIt->y -= ENEMY_SPEED;

                if (abs(enemyIt->x - targetX) < ENEMY_SPEED && abs(enemyIt->y - targetY) < ENEMY_SPEED) {
                    enemyIt->targetIndex++;
                }

                SDL_Rect enemyRect = {enemyIt->x, enemyIt->y, 60, 60};
                SDL_Rect endPointRect = {140, 665, 10, 10};
                if (SDL_HasIntersection(&enemyRect, &endPointRect)) {
                    gameOver = true;
                    break;
                }
                ++enemyIt;
            } else {
                SDL_DestroyTexture(enemyIt->texture);
                enemyIt = enemiesGroup2.erase(enemyIt);
            }
        }

        for (auto& bullet : bullets) {
            switch (bullet.direction) {
                case 2: bullet.x -= BULLET_SPEED; break;
                case 3: bullet.x += BULLET_SPEED; break;
            }
        }

        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            SDL_Rect bulletRect = {bulletIt->x, bulletIt->y, 8, 8};
            bool bulletRemoved = false;

            for (auto enemyIt = enemiesGroup1.begin(); enemyIt != enemiesGroup1.end();) {
                SDL_Rect enemyRect = {enemyIt->x, enemyIt->y, 60, 60};
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    SDL_DestroyTexture(enemyIt->texture);
                    enemyIt = enemiesGroup1.erase(enemyIt);
                    bulletIt = bullets.erase(bulletIt);
                    bulletRemoved = true;
                    break;
                } else {
                    ++enemyIt;
                }
            }
            if (bulletRemoved) continue;

            for (auto enemyIt = enemiesGroup2.begin(); enemyIt != enemiesGroup2.end();) {
                SDL_Rect enemyRect = {enemyIt->x, enemyIt->y, 60, 60};
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    SDL_DestroyTexture(enemyIt->texture);
                    enemyIt = enemiesGroup2.erase(enemyIt);
                    bulletIt = bullets.erase(bulletIt);
                    bulletRemoved = true;
                    break;
                } else {
                    ++enemyIt;
                }
            }
            if (!bulletRemoved) ++bulletIt;
        }

        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
            return bullet.x < 0 || bullet.x > SCREEN_WIDTH || bullet.y < 0 || bullet.y > SCREEN_HEIGHT;
        }), bullets.end());

        SDL_Rect heroRect = {hero.x, hero.y, CHARACTER_WIDTH, CHARACTER_HEIGHT};
        for (auto& enemy : enemiesGroup1) {
            SDL_Rect enemyRect = {enemy.x, enemy.y, 60, 60};
            if (SDL_HasIntersection(&heroRect, &enemyRect)) {
                gameOver = true;
                break;
            }
        }
        for (auto& enemy : enemiesGroup2) {
            SDL_Rect enemyRect = {enemy.x, enemy.y, 60, 60};
            if (SDL_HasIntersection(&heroRect, &enemyRect)) {
                gameOver = true;
                break;
            }
        }

        SDL_Rect winPointRect = {482, 338, 10, 10};
        if (SDL_HasIntersection(&heroRect, &winPointRect)) {
            gameWon = true;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (gameWon) {
        if (!winGameTexture) {
            winGameTexture = loadTexture("wingame.png");
        }
        SDL_Rect winGameRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, winGameTexture, nullptr, &winGameRect);
    }
    else if (gameOver) {
        if (!gameOverTexture) {
            gameOverTexture = loadTexture("gameover.png");
        }
        SDL_Rect gameOverRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    }
    else {
        if (mapTexture) {
            SDL_RenderCopy(renderer, mapTexture, nullptr, nullptr);
        }

        SDL_Texture* currentTexture = nullptr;
        if (hero.isJumping) {
            currentTexture = (hero.direction == 2) ? hero.jumpLeft : hero.jump;
        } else {
            currentTexture = (hero.direction == 2) ? hero.runLeft[hero.frame] : hero.runRight[hero.frame];
        }

        SDL_Rect destRect = {hero.x, hero.y, 50, 50};
        SDL_RenderCopy(renderer, currentTexture, nullptr, &destRect);

        for (auto& bullet : bullets) {
            SDL_Rect bulletRect = {bullet.x, bullet.y, 8, 8};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &bulletRect);
        }

        for (auto& enemy : enemiesGroup1) {
            SDL_Rect enemyRect = {enemy.x, enemy.y, 60, 60};
            SDL_RenderCopy(renderer, enemy.texture ? enemy.texture : nullptr, nullptr, &enemyRect);
        }
        for (auto& enemy : enemiesGroup2) {
            SDL_Rect enemyRect = {enemy.x, enemy.y, 60, 60};
            SDL_RenderCopy(renderer, enemy.texture ? enemy.texture : nullptr, nullptr, &enemyRect);
        }
    }

    SDL_RenderPresent(renderer);
}
