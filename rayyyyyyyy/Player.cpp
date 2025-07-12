#include "Player.h"
#include "Enemy.h"
#include "Npc.h"
#include "gameState.h"

Player::Player(){}

Player::Player(float centerX, float centerY, float radius, Color c) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->radius = radius;
    this->color = c;
}

void Player::shoot() {}

void Player::draw(Texture2D texture){
    DrawTexture(texture, centerX - texture.width / 2, centerY - texture.height / 2, WHITE);
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
