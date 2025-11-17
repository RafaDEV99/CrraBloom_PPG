#include <raylib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include <raylib-nuklear.h>
#include <raylib-nuklear-font.h>

#define WINDOW_WIDTH    1080
#define WINDOW_HEIGHT   700
#define FONT_SIZE       14

struct nk_context *ctx;
float delta = 0.0f;

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CrraBloom Simulator!");
    SetTargetFPS(60);

    int fontSize = FONT_SIZE;

    ctx = InitNuklear(fontSize);


    while (!WindowShouldClose()) {

        delta = GetFrameTime();
        float time = GetTime();
        UpdateNuklear(ctx);
        printf("Time %f\n", time);

        if (time >= 10)
        {
            return 0;
        }

        if (nk_begin(ctx, "My window", nk_rect(20, 75, 220, 220), 
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE))
        {
            // TODO: Do something...
        }
        nk_end(ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawNuklear(ctx);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
