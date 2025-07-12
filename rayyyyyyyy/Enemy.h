#pragma once
#include <raylib.h>
class Enemy{
public:
    float enemyCenterX, enemyCenterY, enemyRadius;
    Color enemyColor;
    Enemy();
    Enemy(float enemyCenterX, float enemyCenterY, float enemyRadius, Color enemyColor);
    void draw(Texture2D enemyTexture);
    void updateEmenyPosition();

};

