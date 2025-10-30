#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#define RAYLIB_NUKLEAR_INCLUDE_DEFAULT_FONT
#include <raylib-nuklear-font.h>
#include <raylib-nuklear.h>

struct nk_context *ctx;

int main()
{
    InitWindow(1080, 700, "Raygui example");
    SetTargetFPS(60);

    int fontSize = 14;

    float testDencity = 0.0f;

    // Font font = LoadFontFromNuklear(fontSize);
    ctx = InitNuklear(fontSize);

    int windowFlags = NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE;
    char windowTittle[] = "Object Creation window";

    struct nk_colorf cubeColor = ColorToNuklearF(BLUE);

    while (!WindowShouldClose()) 
    {
        UpdateNuklear(ctx);

        printf("The object was: %f dencity\n", testDencity);

        if (nk_begin_titled(ctx, "ObjWindow", windowTittle, nk_rect(20, 75, 340, 320), windowFlags))
        {
            nk_layout_row_static(ctx, 35, 340, 1);
            nk_label_wrap(ctx, "Welcome to the CrraBloom creation window!");

            nk_layout_row_static(ctx, 30, 250, 1);
            nk_slider_float(ctx, 0.0f, &testDencity, 100.0f, 0.1f);

            nk_layout_row_begin(ctx, NK_STATIC, 30, 1);
            {
                nk_layout_row_push(ctx, 100);
                nk_label(ctx, "First Row", NK_TEXT_LEFT);

                nk_layout_row_push(ctx, 100);
                nk_label(ctx, "Second Row", NK_TEXT_LEFT);
            }
            nk_layout_row_end(ctx);

            nk_layout_row_dynamic(ctx, 60, 2);
            nk_button_label(ctx, "Button 1");
            nk_button_label(ctx, "Button 2");
            nk_button_label(ctx, "Button 3");
            nk_button_label(ctx, "Button 4");

            nk_layout_row_static(ctx, 120, 120, 1);
            cubeColor = nk_color_picker(ctx, cubeColor, NK_RGBA);
        }
        nk_end(ctx);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(20, 20);
            DrawRectangle(540, 340, 40, 40, ColorFromNuklearF(cubeColor));
            DrawNuklear(ctx);
        EndDrawing();
    }

    UnloadNuklear(ctx);
    CloseWindow();
    return 0;
}
