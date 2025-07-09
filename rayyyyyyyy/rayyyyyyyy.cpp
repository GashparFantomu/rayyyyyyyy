#include "raylib.h"
#include "raylib-tileson.h"
#include "tileson.hpp"

enum gameState {
    MENU, PLAYING, GAME_OVER
};
gameState currentGameState = MENU;
class Enemy {
public:
    float enemyCenterX, enemyCenterY, enemyRadius;
    Color enemyColor;
    Enemy(){}

    Enemy(float enemyCenterX, float enemyCenterY, float enemyRadius, Color enemyColor) {
        this->enemyCenterX = enemyCenterX;
        this->enemyCenterY = enemyCenterY;
        this->enemyRadius = enemyRadius;
        this->enemyColor = enemyColor;
    }      

    void draw(Texture2D enemyTexture){
        DrawTexture(enemyTexture, enemyCenterX - enemyTexture.width / 2, enemyCenterY - enemyTexture.height / 2, WHITE);
    }
    void updateEmenyPosition(){
        enemyCenterX += 1;
        //if (IsKeyDown(KEY_D)) {
        //    enemyCenterX += 6.0;

        //}
        //if (IsKeyDown(KEY_A)) {
        //    enemyCenterX -= 6.0;

        //}
        //if (IsKeyDown(KEY_W)) {
        //    enemyCenterY -= 6.0;

        //}
        //if (IsKeyDown(KEY_S)) {
        //    enemyCenterY += 6.0;
        //}
    }
};

class Player{
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

    void shoot() {

    }

    void draw(Texture2D texture) {
        DrawTexture(texture, centerX - texture.width / 2, centerY - texture.height / 2, WHITE);
    }
    void update(const Rectangle& wall, const Rectangle& anotherWall, const Enemy& enemy) {
        if (IsKeyDown(KEY_D)) {
            centerX += 6.0;
            if (CheckCollisionCircleRec({centerX, centerY}, radius, wall)) {
                centerX -=6.0;
            }
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerX -= 6.0;
            }
        }
        if (IsKeyDown(KEY_A)) {
            centerX -= 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerX += 6.0;
            }
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerX += 6.0;
            }
        }
        if (IsKeyDown(KEY_W)) {
            centerY -= 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerY +=6.0;
            }
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerY += 6.0;
            }
        }
        if (IsKeyDown(KEY_S)) {
            centerY += 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerY -=6.0;
            }
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerY -= 6.0;
            }
        }
        if (CheckCollisionCircles({ centerX, centerY }, radius, { enemy.enemyCenterX, enemy.enemyCenterY }, enemy.enemyRadius)) {
            currentGameState = GAME_OVER;
        }
    }
};

float Lerp(float start, float end, float amount) { //easing la camera
    return start + (end - start) * amount;
}

int main()
{
    InitWindow(1920, 800, "metal soul - prototype");
    InitAudioDevice();
    Music bgMusic = LoadMusicStream("assets/Into Darkness.mp3");
    Music menuMusic = LoadMusicStream("assets/Dragonsreach.mp3");
    Music endMusic = LoadMusicStream("assets/death.mp3");

    PlayMusicStream(menuMusic);


    SetTargetFPS(60);
    
    Image icon = LoadImage("assets/icon.png");
    Texture2D playerImage = LoadTexture("assets/knight.png");
    Texture2D enemyImage = LoadTexture("assets/knight.png");

    Player player(120.0, 120.0, 12, WHITE);
    Enemy enemy(150.0, 150.0, 15, WHITE);

    Texture2D background = LoadTexture("assets/background.jpg");
    Texture2D menuBG = LoadTexture("assets/menu.jpg");

    Texture2D tileset = LoadTexture("assets/tmw_desert_spacing.png");
    SetTextureFilter(tileset, TEXTURE_FILTER_POINT);
    Map map = LoadTiled("assets/desert.json");
    if (!IsTiledReady(map)) {
        TraceLog(LOG_ERROR, "Failed to load the damn map!");
    }

    Rectangle wall = { 300, 200, 130, 30 };
    Rectangle anotherWall = { 500, 400, 400, 30 };

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Camera2D camera = { 0 };

    camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0;
    camera.zoom = 1.;
    float lerpFactor = 0.05;


    SetWindowIcon(icon);
    UnloadImage(icon);
    
    gameState previousState = currentGameState;

    while (!WindowShouldClose()){
        if (currentGameState != previousState) {
            switch (currentGameState) {
            case MENU:
                StopMusicStream(bgMusic);
                StopMusicStream(endMusic);
                PlayMusicStream(menuMusic);
                break;
            case PLAYING:
                StopMusicStream(menuMusic);
                StopMusicStream(endMusic);
                PlayMusicStream(bgMusic);
                break;
            case GAME_OVER:
                StopMusicStream(bgMusic);
                StopMusicStream(menuMusic);
                PlayMusicStream(endMusic);
                break;
            }
            previousState = currentGameState;

        }

        switch (currentGameState) {
        case MENU:
            BeginDrawing();
            UpdateMusicStream(menuMusic);
            ClearBackground(BLACK);
            DrawTexture(menuBG, 0, 0, WHITE);
            DrawText("METAL SOUL - PROTORYPE", 600, 200, 50, WHITE);
            DrawText("PRESS ENTER", 850, 450, 30, RAYWHITE);
            if (IsKeyPressed(KEY_ENTER)) {
                currentGameState = PLAYING;
            }
            EndDrawing();
            break;
        case PLAYING:
            UpdateMusicStream(bgMusic);
            //actualizari
            player.update(wall, anotherWall, enemy);
            enemy.updateEmenyPosition();
            camera.target.x = Lerp(camera.target.x, player.centerX, lerpFactor);
            camera.target.y = Lerp(camera.target.y, player.centerY, lerpFactor);
            //camera.target.x = roundf(camera.target.x);
            //camera.target.y = roundf(camera.target.y);


            BeginDrawing();
            //desenare
            ClearBackground(BLACK);

            BeginMode2D(camera);
            DrawTiled(map, 0, 0, WHITE);

            //DrawTexture(background, 0, 0, WHITE);

            //DrawRectangleRec(wall, GRAY); //zid 1
            //DrawRectangleRec(anotherWall, DARKPURPLE); //alt zid 


            player.draw(playerImage);
            enemy.draw(enemyImage);

            EndMode2D();
            EndDrawing();
            break;
        case GAME_OVER:
            BeginDrawing();
            ClearBackground(BLACK);
            UpdateMusicStream(endMusic);
            DrawText("YOU FUCKIN' DIED!", 600, 250, 70, RED);
            //DrawText("I looooooove watching you fail", 700, 550, 25, RED);
            if (IsKeyPressed(KEY_ENTER)) {
                currentGameState = PLAYING;
                player.centerX = 120;
                player.centerY = 120;
                StopMusicStream(endMusic);
            }
            EndDrawing();
        }

    }
    UnloadMusicStream(bgMusic);
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(endMusic);
    UnloadMap(map);

    CloseAudioDevice();

    CloseWindow();
    return 0;
}