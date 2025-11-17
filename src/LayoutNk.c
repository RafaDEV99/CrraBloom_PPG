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
    Color randomColor;

    while (!WindowShouldClose()) {

        delta = GetFrameTime();
        float time = GetTime();
        UpdateNuklear(ctx);
        printf("Time %f\n", time);

        randomColor = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};

        if (time >= 10)
        {
            // Memory free escape:
            CloseWindow();
            UnloadNuklear(ctx);
            return 0;
        }

        if (nk_begin(ctx, "My window", nk_rect(20, 75, 220, 220), 
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE))
        {
            nk_layout_row_static(ctx, 50, 120, 1);
            nk_label_colored(ctx, "This text is going crazy!!", NK_TEXT_ALIGN_LEFT, ColorToNuklear(randomColor));
        }
        nk_end(ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawNuklear(ctx);
        EndDrawing();
    }

    // Unload Window and Nuklear GUI:
    CloseWindow();
    UnloadNuklear(ctx);
    return 0;
}
