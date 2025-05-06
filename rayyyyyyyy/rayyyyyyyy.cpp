#include "raylib.h"

class Player {
public:
    float centerY, centerX, radius;
    Color color;

    Player(){}
    Player(float centerX, float centerY, float radius, Color c) {
        this->centerX = centerX;
        this->centerY = centerY;
        this->radius = radius;
        this->color = c;
    }
    void draw() {
        DrawCircle(centerX, centerY, radius, color);
        
    }
    void update(const Rectangle& wall) {
        if (IsKeyDown(KEY_RIGHT)) {
            centerX += 6.0;
            if (CheckCollisionCircleRec({centerX, centerY}, radius, wall)) {
                centerX -=6.0;
            }
        }
        if (IsKeyDown(KEY_LEFT)) {
            centerX -= 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerX += 6.0;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            centerY -= 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerY +=6.0;
            }
        }
        if (IsKeyDown(KEY_DOWN)) {
            centerY += 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerY -=6.0;
            }
        }
    }
};


int main()
{
    InitWindow(1920, 800, "fereastra");
    SetTargetFPS(60);
    Image icon = LoadImage("assets/icon.png");
    Texture playerImage = LoadTexture("assets/icon.png");
    Player player(120.0, 120.0, 12, WHITE);
    Texture2D background = LoadTexture("assets/background.jpg");
    Rectangle wall = { 300, 200, 130, 30 };

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Camera2D camera = { 0 };

    camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0;
    camera.zoom = 1.0;
    Vector2 playerPosition = { player.centerX, player.centerY };
    DrawTexture(playerImage, player.centerX, player.centerY, WHITE);
    SetWindowIcon(icon);
    UnloadImage(icon);
    
    while (!WindowShouldClose()){
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        DrawText("TEXT", 880, 420, 50, LIGHTGRAY);
        DrawTexture(background, 0, 0, WHITE);
        
        DrawRectangleRec(wall, GRAY);
        camera.target = Vector2{ player.centerX, player.centerY };
        player.draw();
        player.update(wall);
        
        
        
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}