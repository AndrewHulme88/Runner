#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

    const int gravity = 1000;

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{6};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 12.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 400;
    }

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x + 200 };

    // scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;

    int velocity = 0;
    bool isInAir = false;
    const int jumpValue = -600;

    int nebVelocity = -200; 

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{};
    float bgXMid{};
    float bgXFore{};

    bool collision{};

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        // delta time variable
        const float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT;
        if(bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }

        bgXMid -= 40 * dT;
        if(bgXMid <= -midground.width * 2)
        {
            bgXMid = 0.0;
        }

        bgXFore -= 80 * dT;
        if(bgXFore <= -foreground.width * 2)
        {
            bgXFore = 0.0;
        }

        // draw background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw midground
        Vector2 bgMid1Pos{bgXMid, 0.0};
        DrawTextureEx(midground, bgMid1Pos, 0.0, 2.0, WHITE);
        Vector2 bgMid2Pos{bgXMid + midground.width * 2, 0.0};
        DrawTextureEx(midground, bgMid2Pos, 0.0, 2.0, WHITE);

        // draw foreground
        Vector2 bgFore1Pos{bgXFore, 0.0};
        DrawTextureEx(foreground, bgFore1Pos, 0.0, 2.0, WHITE);
        Vector2 bgFore2Pos{bgXFore + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, bgFore2Pos, 0.0, 2.0, WHITE);


        // ground check
        if(isOnGround(scarfyData, windowDimensions[1]))
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
        for(int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebVelocity * dT;
        }

        finishLine += nebVelocity * dT;

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        if(!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        // update nebulae animation
        for(int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        for(AnimData nebula : nebulae)
        {
            float padding{50};
            Rectangle nebRec{
                nebula.pos.x + padding,
                nebula.pos.y + padding,
                nebula.rec.width - 2 * padding,
                nebula.rec.height - 2 * padding
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if(CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if(collision)
        {
            DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, WHITE);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, WHITE);
        }
        
        else
        {
            // draw nebula
            for(int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}