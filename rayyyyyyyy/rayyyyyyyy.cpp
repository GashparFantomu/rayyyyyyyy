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
    void draw(Texture2D texture) {
        DrawTexture(texture, centerX - texture.width / 2, centerY - texture.height / 2, WHITE);
    }
    void update(const Rectangle& wall, const Rectangle& anotherWall) {
        if (IsKeyDown(KEY_RIGHT)) {
            centerX += 6.0;
            if (CheckCollisionCircleRec({centerX, centerY}, radius, wall)) {
                centerX -=6.0;
            }
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerX -= 6.0;
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
    InitWindow(1920, 800, "metal soul - prototype");
    SetTargetFPS(60);
    Image icon = LoadImage("assets/icon.png");
    Texture2D playerImage = LoadTexture("assets/knight.png");

    Player player(120.0, 120.0, 12, WHITE);

    Texture2D background = LoadTexture("assets/background.jpg");


    Rectangle wall = { 300, 200, 130, 30 };

    Rectangle anotherWall = { 500, 400, 400, 30 };

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
 
        DrawTexture(background, 0, 0, WHITE);
        
        DrawRectangleRec(wall, GRAY);
        DrawRectangleRec(anotherWall, DARKPURPLE);
        camera.target = Vector2{ player.centerX, player.centerY };
        player.draw(playerImage);
        player.update(wall, anotherWall);
        
        
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}