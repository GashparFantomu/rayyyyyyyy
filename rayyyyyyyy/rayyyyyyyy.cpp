#include "raylib.h"
#include "raylib-tileson.h"
#include "tileson.hpp"
#include "Npc.h"
#include "Enemy.h"
#include "Player.h"
#include "gameState.h"

float Lerp(float start, float end, float amount) { //easing la camera
    return start + (end - start) * amount;
}

int main(){
    InitWindow(1920, 800, "metal soul - prototype");
    InitAudioDevice();
    Music bgMusic = LoadMusicStream("assets/Into Darkness.mp3");
    Music menuMusic = LoadMusicStream("assets/Dragonsreach.mp3");
    Music endMusic = LoadMusicStream("assets/death.mp3");

    PlayMusicStream(menuMusic);
    gameState currentGameState = MENU; 

    SetTargetFPS(60);
    
    Image icon = LoadImage("assets/icon.png");
    Texture2D playerImage = LoadTexture("assets/knight.png");
    Texture2D enemyImage = LoadTexture("assets/knight.png");
    Texture2D npcImage = LoadTexture("assets/knight.png");
    Texture2D arrow = LoadTexture("assets/Arrow01(32x32).png");

    Player player(120.0, 120.0, 12, WHITE);
    Enemy enemy(150.0, 150.0, 15, WHITE);
    Npc npc(110.0, 110.0, 12, WHITE);

    bool canInteractWithNpc = false;

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
    camera.zoom = 1.0;
    float lerpFactor = 0.05;


    SetWindowIcon(icon);
    
    gameState previousState = currentGameState;

    while (!WindowShouldClose()){ //THE MAIN GAME LOOP
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
            player.update(wall, anotherWall, enemy, npc, canInteractWithNpc, currentGameState);
            player.updateArrow(GetFrameTime()); 
            enemy.updateEmenyPosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                player.shootCamera(camera);
            }

            camera.target.x = Lerp(camera.target.x, player.centerX, lerpFactor);
            camera.target.y = Lerp(camera.target.y, player.centerY, lerpFactor);
            camera.target.x = roundf(camera.target.x);
            camera.target.y = roundf(camera.target.y);


            BeginDrawing();
            //desenare
            ClearBackground(BLACK);

            BeginMode2D(camera);
            DrawTiled(map, 0, 0, WHITE);


            DrawRectangleRec(wall, GRAY); //zid 1
            DrawRectangleRec(anotherWall, DARKPURPLE); //alt zid 


            player.draw(playerImage);
            
            player.drawArrow();
            enemy.draw(enemyImage);
            npc.draw(npcImage);
            
            EndMode2D();

            if (canInteractWithNpc) {
                DrawText("Interact(E)", screenWidth / 2, screenHeight - 65, 25, RAYWHITE);
                //if (IsKeyPressed(KEY_E)) {
                //    DrawText("Some generic shitty ass dialog text without animation", screenWidth / 2, screenHeight - 120, 25, RAYWHITE);
                //}
            }

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
    UnloadTexture(enemyImage);
    UnloadTexture(playerImage);
    UnloadTexture(npcImage);
    UnloadTexture(background);
    UnloadTexture(menuBG);
    UnloadImage(icon);
    UnloadTexture(tileset);


    CloseAudioDevice();

    CloseWindow();
    return 0;
}