#include "raylib.h"
#include <stdio.h>
#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

void GetAllCodepoints(int codepoints[])
{
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 400; i++) codepoints[95 + i] = 0xf000 + i;
}

int main() {
    InitWindow(640, 480, "Interfaz con Iconos");

    // 1. Cargar la fuente con iconos usando raylib (por ejemplo, FontAwesome)
    // Nota: Debes tener el archivo .ttf en tu carpeta de proyecto.
    //

    int codepoints[512];
    GetAllCodepoints(codepoints);

    float fontSize = 10.0f;
    Font iconFont = LoadFontEx("../fonts/Font Awesome 7 Free-Solid-900.otf", fontSize, codepoints, 495);

    // 2. Inicializar Nuklear con la fuente cargada en lugar de la predeterminada
    // Se usa InitNuklearEx para pasar una estructura Font de raylib [1, 2]
    struct nk_context *ctx = InitNuklearEx(iconFont, fontSize);

    while (!WindowShouldClose()) {
        UpdateNuklear(ctx);

        if (nk_begin(ctx, "Panel de Control", nk_rect(50, 50, 220, 220),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
            
            nk_layout_row_static(ctx, 40, 40, 3);

            // 3. Usar los iconos en botones o etiquetas
            // En Nuklear, puedes pasar el carácter UTF-8 del icono [3]
            // (Ejemplo simbólico: "\uf015" suele ser un icono de 'Inicio')
            if (nk_button_label(ctx, "\uf0c7")) {
                // Acción al presionar el icono 1
            }
            if (nk_button_label(ctx, "test")) {
                // Acción al presionar el icono 2 (Configuración)
            }
            if (nk_button_label(ctx, "  ")) {
                // Acción al presionar el icono 3 (Usuario)
            }
        }
        nk_end(ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Renderizar la interfaz con la fuente de iconos [1]
        DrawNuklear(ctx);

        EndDrawing();
    }

    // 4. Limpieza de recursos
    UnloadNuklear(ctx);
    UnloadFont(iconFont); // Liberar la fuente de raylib
    CloseWindow();

    return 0;
}
