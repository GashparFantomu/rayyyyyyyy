#pragma once
using namespace std;
#include <raylib.h>
#include "Enemy.h"
#include "Npc.h"
#include "gameState.h" 
#include <vector>
#include "weapons.h"


class Player {
public:
	float centerY, centerX, radius;
	Color color;
	vector<Arrow> arrows;
	Player();
	Player(float centerX, float centerY, float radius, Color c);
	void shoot(int direction);
	void drawArrow();
	void updateArrow(float deltaTIme);
	void draw(Texture2D texture);
	void update(const Rectangle& wall, const Rectangle& anotherWall, const Enemy& enemy, const Npc& npc, bool& canInteractWithNpc, gameState& currentGameState);
};

