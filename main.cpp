#include "raylib.h"



int main()
{
    // window dimensions
    const int windowWidth = 512;
    const int windowHeight = 380;

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    const int gravity = 1000;

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebRec{0.0, 0.0, nebula.width / 8, nebula.height / 8};
    Vector2 nebPos{windowWidth, windowHeight - nebRec.height};
    int nebVelocity = -600; 

    // scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = windowWidth/2 - scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;

    // animation frame
    int frame;
    const float updateTime = 1.0 / 12.0;
    float runningTime = 0;

    int velocity = 0;
    bool isInAir = false;
    const int jumpValue = -600;

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        // delta time variable
        const float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        // ground check
        if(scarfyPos.y >= windowHeight - scarfyRec.height)
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // apply gravity
            velocity += gravity * dT;
            isInAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpValue;
        }

        // update nebula position
        nebPos.x += nebVelocity * dT;

        // update scarfy position
        scarfyPos.y += velocity * dT;

        runningTime += dT;
        if(runningTime >= updateTime && !isInAir)
        {
            runningTime = 0;
            // update animation frame
            scarfyRec.x = frame * scarfyRec.width;
            frame++;
            if(frame > 5)
            {
                frame = 0;
            }
        }

        // draw nebula
        DrawTextureRec(nebula, nebRec, nebPos, WHITE);

        // draw scarfy
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}