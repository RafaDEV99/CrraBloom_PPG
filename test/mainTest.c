#include <raylib.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#define RAYLIB_NUKLEAR_INCLUDE_DEFAULT_FONT
#include <raylib-nuklear-font.h>
#include <raylib-nuklear.h>

struct nk_context *ctx;

int main()
{
    InitWindow(1080, 700, "Raygui example");
    SetTargetFPS(60);

    int fontSize = 16;
    // Font font = LoadFontFromNuklear(fontSize);
    ctx = InitNuklear(fontSize);

    while (!WindowShouldClose()) 
    {
        UpdateNuklear(ctx);
        if (nk_begin(ctx, "Fist window", nk_rect(20, 75, 220, 220), 
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE))
        {
            nk_label(ctx, "Welcome to Nuklear!!", NK_TEXT_ALIGN_TOP);
        }
        nk_end(ctx);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(20, 20);
            DrawNuklear(ctx);
        EndDrawing();
    }

    return 0;
}
