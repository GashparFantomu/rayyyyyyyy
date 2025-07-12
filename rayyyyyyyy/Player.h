#pragma once
#include <raylib.h>
#include "Enemy.h"
#include "Npc.h"
#include "gameState.h" // Ensure gameState is included


class Player {
public:
	float centerY, centerX, radius;
	Color color;
	Player();
	Player(float centerX, float centerY, float radius, Color c);
	void shoot();
	void draw(Texture2D texture);
	void update(const Rectangle& wall, const Rectangle& anotherWall, const Enemy& enemy, const Npc& npc, bool& canInteractWithNpc, gameState& currentGameState);
};

