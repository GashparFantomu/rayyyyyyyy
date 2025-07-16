#pragma once
#include <vector>
#include "raylib.h"

struct Arrow {
    Vector2 position;
    Vector2 direction; // Should be (0,1), (0,-1), (1,0), or (-1,0)
    float speed = 500.0; // pixels per second
    bool active = true;
};