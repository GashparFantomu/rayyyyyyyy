#include "raylib.h"
#include "raylib-tileson.h"
#include "tileson.hpp"
#include <filesystem>
#include "Npc.h"
#include "Enemy.h"
#include "Player.h"
#include "gameState.h"

class GameMap {
private:
    tson::Map map;
    std::vector<std::vector<bool>> collisionGrid;
    int tileSize = 32;

public:
    bool LoadMap(const std::string& mapPath) {
        tson::Tileson parser;
        auto parseResult = parser.parse(mapPath); // Parse the map file

        if (map.getStatus() != tson::ParseStatus::OK) {
            TraceLog(LOG_ERROR, "Failed to load map: %s", mapPath.c_str());
            return false;
        }

        // Initialize collision grid
        SetupCollisionGrid();

        TraceLog(LOG_INFO, "Map loaded successfully: %dx%d",
            map.getSize().x, map.getSize().y);
        return true;
    }

    void SetupCollisionGrid() {
        int mapWidth = map.getSize().x;
        int mapHeight = map.getSize().y;

        collisionGrid.resize(mapHeight);
        for (int y = 0; y < mapHeight; y++) {
            collisionGrid[y].resize(mapWidth, false);
        }

        // Find collision layer
        auto* collisionLayer = map.getLayer("collision");
        if (!collisionLayer) {
            TraceLog(LOG_WARNING, "No collision layer found");
            return;
        }

        // Process collision tiles
        for (auto& tilePair : collisionLayer->getTileObjects()) {
            auto& tileObject = tilePair.second; // Access the tson::TileObject from the pair
            int x = tileObject.getPosition().x / tileSize;
            int y = tileObject.getPosition().y / tileSize;

            if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
                // Check if tile has collision property
                if (tileObject.getTile() && tileObject.getTile()->getProperties().hasProperty("solid")) {
                    collisionGrid[y][x] = tileObject.getTile()->getProperties().getValue<bool>("solid");
                }
            }
        }
    }

    bool IsColliding(Vector2 position, Vector2 size) {
        // Convert world position to tile coordinates
        int startX = (int)(position.x / tileSize);
        int startY = (int)(position.y / tileSize);
        int endX = (int)((position.x + size.x) / tileSize);
        int endY = (int)((position.y + size.y) / tileSize);

        // Check bounds
        if (startX < 0 || startY < 0 || endX >= collisionGrid[0].size() ||
            endY >= collisionGrid.size()) {
            return true; // Out of bounds = collision
        }

        // Check collision grid
        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                if (collisionGrid[y][x]) {
                    return true;
                }
            }
        }
        return false;
    }
    void DrawMap(Camera2D camera) {
        // Use raylib-tileson to draw the map
        // This is a simplified version - you'll need to implement layer rendering

        for (auto& layer : map.getLayers()) {
            if (layer.getName() == "collision") continue; // Skip collision layer

            for (auto& tile : layer.getTileObjects()) {
                Vector2 pos = {
                    (float)tile.second.getPosition().x,
                    (float)tile.second.getPosition().y
                };

                // Draw tile (you'll need to load your tileset texture)
                // DrawTexture(tilesetTexture, pos.x, pos.y, WHITE);
            }
        }
    }
};


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