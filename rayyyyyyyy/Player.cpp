#include "Player.h"
#include "Enemy.h"
#include "Npc.h"
#include "gameState.h"
#include "weapons.h"
#include <cmath>

Player::Player(){}

Player::Player(float centerX, float centerY, float radius, Color c) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->radius = radius;
    this->color = c;
}

void Player::shoot(Vector2 direction) {
    Arrow arrow;
    arrow.position = { centerX, centerY };
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        arrow.direction = { direction.x / length, direction.y / length };
    } else {
        arrow.direction = { 0.0, 0.0 };
    }
    arrows.push_back(arrow);
}

void Player::updateArrow(float deltaTime) {

    for (auto& arrow : arrows) {
        if (arrow.active) {
            arrow.position.x += arrow.direction.x * arrow.speed * deltaTime;
            arrow.position.y += arrow.direction.y * arrow.speed * deltaTime;
        }
        float distanceFromPlayer = sqrtf((arrow.position.x - centerX) * (arrow.position.x - centerX) + (arrow.position.y - centerY) * (arrow.position.y - centerY));

        if (distanceFromPlayer > 2000.0) {
            arrow.active = false;
        }
    }
}

void Player::draw(Texture2D texture){
    DrawTexture(texture, centerX - texture.width / 2, centerY - texture.height / 2, WHITE);
}

void Player::drawArrow() {
    for (const auto& arrow : arrows) {
        if (arrow.active) {
            DrawRectangle(arrow.position.x - 5, arrow.position.y - 5, 10, 10, RED);
        }
    }
}

void Player::shootCamera(const Camera2D& camera) {
    Vector2 mouseScreenPosition = GetMousePosition();
    Vector2 mouseWorldPosition = GetScreenToWorld2D(mouseScreenPosition, camera);
    Vector2 playerPosition = { centerX, centerY };
    Vector2 direction = { mouseWorldPosition.x - playerPosition.x, mouseWorldPosition.y - playerPosition.y };
    shoot(direction);
}

void Player::update(const Rectangle& wall, const Rectangle& anotherWall, const Enemy& enemy, const Npc& npc, bool& canInteractWithNpc, gameState& currentGameState){
    if (IsKeyDown(KEY_D)) {
        centerX += 6.0;
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
            centerX -= 6.0;
        }
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
            centerX -= 6.0;
        }
    }
    if (IsKeyDown(KEY_A)) {
        centerX -= 6.0;
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
            centerX += 6.0;
        }
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
            centerX += 6.0;
        }
    }
    if (IsKeyDown(KEY_W)) {
        centerY -= 6.0;
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
            centerY += 6.0;
        }
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
            centerY += 6.0;
        }
    }
    if (IsKeyDown(KEY_S)) {
        centerY += 6.0;
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
            centerY -= 6.0;
        }
        if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
            centerY -= 6.0;
        }
    }
    if (CheckCollisionCircles({ centerX, centerY }, radius, { enemy.enemyCenterX, enemy.enemyCenterY }, enemy.enemyRadius)) {
        currentGameState = GAME_OVER;
    }
    if (CheckCollisionCircles({ centerX, centerY }, radius, { npc.npcCenterX, npc.npcCenterY }, npc.npcRadius + 25)) {
        canInteractWithNpc = true;
    }
    else {
        canInteractWithNpc = false;
    }
}
