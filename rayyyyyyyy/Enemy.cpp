#include "Enemy.h"

Enemy::Enemy(){}
Enemy::Enemy(float enemyCenterX, float enemyCenterY, float enemyRadius, Color enemyColor){
    this->enemyCenterX = enemyCenterX;
    this->enemyCenterY = enemyCenterY;
    this->enemyRadius = enemyRadius;
    this->enemyColor = enemyColor;
}

void Enemy::draw(Texture2D enemyTexture) {
    DrawTexture(enemyTexture, enemyCenterX - enemyTexture.width / 2, enemyCenterY - enemyTexture.height / 2, WHITE);
}

void Enemy::updateEmenyPosition() {
    enemyCenterX += 0.6;
}