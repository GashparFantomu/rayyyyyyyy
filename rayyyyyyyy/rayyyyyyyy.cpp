#include "raylib.h"

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

class Player : Enemy{
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
    void update(const Rectangle& wall, const Rectangle& anotherWall, const Enemy& enemy) {
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
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerX += 6.0;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            centerY -= 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerY +=6.0;
            }
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerY += 6.0;
            }
        }
        if (IsKeyDown(KEY_DOWN)) {
            centerY += 6.0;
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, wall)) {
                centerY -=6.0;
            }
            if (CheckCollisionCircleRec({ centerX, centerY }, radius, anotherWall)) {
                centerY -= 6.0;
            }
        }
        if (CheckCollisionCircles({ centerX, centerY }, radius, { enemy.enemyCenterX, enemy.enemyCenterY }, enemy.enemyRadius)) {
            DrawText("YOU FUCKIN' DIED!", centerX, centerY, 40, RED);
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
    Music bgMusic = LoadMusicStream("assets/background.mp3");
    PlayMusicStream(bgMusic);

    SetTargetFPS(60);
    
    Image icon = LoadImage("assets/icon.png");
    Texture2D playerImage = LoadTexture("assets/knight.png");
    Texture2D enemyImage = LoadTexture("assets/knight.png");

    Player player(120.0, 120.0, 12, WHITE);
    Enemy enemy(150.0, 150.0, 15, WHITE);

    Texture2D background = LoadTexture("assets/terrain.png");

    Rectangle wall = { 300, 200, 130, 30 };

    Rectangle anotherWall = { 500, 400, 400, 30 };

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Camera2D camera = { 0 };

    camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0;
    camera.zoom = 1.2;
    float lerpFactor = 0.05;


    SetWindowIcon(icon);
    UnloadImage(icon);
    
    while (!WindowShouldClose()){
        UpdateMusicStream(bgMusic);
        //actualizari
        player.update(wall, anotherWall, enemy);
        enemy.updateEmenyPosition();
        camera.target.x = Lerp(camera.target.x, player.centerX, lerpFactor);
        camera.target.y = Lerp(camera.target.y, player.centerY, lerpFactor);

        BeginDrawing();
        //desenare
        ClearBackground(BLACK);

        BeginMode2D(camera);
 
        DrawTexture(background, 0, 0, WHITE);
        
        DrawRectangleRec(wall, GRAY); //zid 1
        DrawRectangleRec(anotherWall, DARKPURPLE); //alt zid 
        

        player.draw(playerImage);
        enemy.draw(enemyImage);
        
        EndMode2D();
        EndDrawing();
    }
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}