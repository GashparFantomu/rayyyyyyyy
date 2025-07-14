#include "Player.h"
#include "Enemy.h"
#include "Npc.h"
#include "gameState.h"
#include "weapons.h"

Player::Player(){}

Player::Player(float centerX, float centerY, float radius, Color c) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->radius = radius;
    this->color = c;
}

void Player::shoot(int direction) {
    Arrow arrow;
    arrow.position = { centerX, centerY };
    switch (direction) {
        case 0: arrow.direction = { 0, -1 }; break;
        case 1: arrow.direction = { 0, 1 }; break;
        case 2: arrow.direction = { -1, 0 }; break;
        case 3: arrow.direction = { 1, 0 }; break;
    }
    arrows.push_back(arrow);
}

void Player::updateArrow(float deltaTime) {
    for (auto& arrow : arrows) {
        if (arrow.active) {
            arrow.position.x += arrow.direction.x * arrow.speed * deltaTime;
            arrow.position.y += arrow.direction.y * arrow.speed * deltaTime;
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
    if (IsKeyPressed(KEY_UP)) {
        shoot(0);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        shoot(1);

    }
    if (IsKeyPressed(KEY_LEFT)) {
        shoot(2);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        shoot(3);
    }
}
