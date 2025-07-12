#pragma once
#include <raylib.h>

class Npc{
public:
    float npcCenterX, npcCenterY, npcRadius;
    Color npcColor;
    Npc();
    Npc(float npcCenterX, float npcCenterY, float npcRadius, Color npcColor);
    void draw(Texture2D npcTexture);
};

