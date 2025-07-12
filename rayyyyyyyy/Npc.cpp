#include "Npc.h"

Npc::Npc(){}
Npc::Npc(float npcCenterX, float npcCenterY, float npcRadius, Color npcColor){
    this->npcCenterX = npcCenterX;
    this->npcCenterY = npcCenterY;
    this->npcRadius = npcRadius;
    this->npcColor = npcColor;
}

void Npc::draw(Texture2D npcTexture) {
    DrawTexture(npcTexture, npcCenterX - npcTexture.width / 2, npcCenterY - npcTexture.height / 2, WHITE);
}