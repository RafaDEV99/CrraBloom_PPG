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
        if (nk_begin(ctx, "Fist window", nk_rect(20, 75, 320, 450), 
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE))
        {
            nk_layout_row_static(ctx, 70, 200, 1);
            nk_label(ctx, "Welcome to Nuklear!!", NK_TEXT_CENTERED|NK_TEXT_ALIGN_LEFT);
            nk_button_label(ctx, "Yes, this a button!");
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
