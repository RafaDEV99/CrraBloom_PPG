#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "../Includes/raylib-nuklear.h"

int main() {
    InitWindow(1080, 700, "raylib-nuklear example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Create the Nuklear Context
    int fontSize = 10;
    bool show_window = true;
    struct nk_context *ctx = InitNuklear(fontSize);
    struct nk_rect boundsA = {50, 50, 200, 200};
    struct nk_rect boundsB = {300, 50, 200, 200};
    int windowOptions = NK_WINDOW_CLOSABLE|NK_WINDOW_BORDER|NK_WINDOW_MOVABLE;

    static int show_group =  1;
    enum {EASY, HARD};
    static int op = EASY;
    static float value = 0.6f;
    static int i =  20;

    char windowName[] = "My window";

    while (!WindowShouldClose()) {
        // Update the Nuklear context, along with input
        UpdateNuklear(ctx);

        // Nuklear GUI Code
        // https://github.com/Immediate-Mode-UI/Nuklear/wiki/Window
        if (nk_begin(ctx, windowName, boundsA, windowOptions) && (show_window))
        {
            // GUI widgets
        }
        else {
            windowOptions = NK_WINDOW_HIDDEN;
        }
        nk_end(ctx);

        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Render the Nuklear GUI
            DrawNuklear(ctx);

        EndDrawing();
    }

    // De-initialize the Nuklear GUI
    UnloadNuklear(ctx);

    CloseWindow();
    return 0;
}
