#pragma once
#include <vector>
#include "raylib.h"

struct Arrow {
    Vector2 position;
    Vector2 direction;
    float speed = 500.0; // pixels per second
    bool active = true;
};