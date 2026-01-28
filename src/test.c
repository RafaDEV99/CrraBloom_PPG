#include "raylib.h"
#include <stdio.h>
#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

int main() {
    InitWindow(640, 480, "Interfaz con Iconos");

    // 1. Cargar la fuente con iconos usando raylib (por ejemplo, FontAwesome)
    // Nota: Debes tener el archivo .ttf en tu carpeta de proyecto.

    int ranges_icons[] = {
        0x0049, 0x004A,
        0xF07C, 0xF07C,	/*   */
        0xF083, 0xF083,	/*   */
        0xF0AD, 0xF0AD,	/*   */
        0xF021, 0xF021,	/*   */
        0
    };

    // int ranges_icons[] = { 0x0020, 0x007F, 0xF07C, 0xF07C, 0xF083, 0xF083, 0xF0AD, 0xF0AD, 0xF021, 0xF021, 0 };

    float fontSize = 10.0f;
    Font iconFont = LoadFont("../fonts/Font Awesome 7 Free-Solid-900.otf");
    struct nk_font_atlas *atlas;

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
            if (nk_button_label(ctx, " ")) {
                // Acción al presionar el icono 1
            }
            if (nk_button_label(ctx, "wdswsw")) {
                // Acción al presionar el icono 2 (Configuración)
            }
            if (nk_button_label(ctx, "")) {
                // Acción al presionar el icono 3 (Usuario)
            }
        }
        nk_end(ctx);
        printf("%d\n", GetCodepointCount("Hellow, is me!!  "));

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
