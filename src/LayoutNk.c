#include "box2d/id.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include <raylib.h>
#include <box2d/box2d.h>
#include <stdio.h>
#include <stdlib.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include <raylib-nuklear.h>
#include <raylib-nuklear-font.h>

#define WINDOW_WIDTH    1080
#define WINDOW_HEIGHT   700
#define FONT_SIZE       14
#define UNITS_PER_METER 128.0f

struct nk_context *ctx;
float delta = 0.0f;

b2BodyId CreateObject(b2Vec2 position, b2WorldId worldId, Vector2 size)
{
    b2BodyDef def = b2DefaultBodyDef();
    def.position = position;
    def.type = b2_dynamicBody;

    b2BodyId id = b2CreateBody(worldId, &def);
    b2Vec2 extend;

    b2Polygon box = b2MakeBox(size.x, size.y);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(id, &shapeDef, &box);

    return id;
}


void DrawBody(b2BodyId id, Vector2 size)
{
    b2Vec2 pos = b2Body_GetPosition(id);
    b2Vec2 Ppos = b2Body_GetWorldPoint(id, (b2Vec2){-size.x * 0.5f, -size.y * 0.5f});
    float angle = b2Rot_GetAngle(b2Body_GetRotation(id));

    DrawCircleV((Vector2){pos.x, pos.y}, 5.0f, BLACK);

    DrawRectanglePro(
        (Rectangle){pos.x, pos.y, size.x, size.y},
        (Vector2){size.x * 0.5f, size.y * 0.5f},
        angle * RAD2DEG,
        BLUE
    );
}

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CrraBloom Simulator!");
    SetTargetFPS(60);

    b2WorldDef worldDef = b2DefaultWorldDef();
    b2WorldId worldId = b2CreateWorld(&worldDef);
    b2SetLengthUnitsPerMeter(UNITS_PER_METER);
    worldDef.gravity.y = 9.8f * UNITS_PER_METER;


    int fontSize = FONT_SIZE;
    bool colorCicle = true;

    float time = 5.0f;
    int subStepCount = 4;

    b2BodyId bodyId = CreateObject((b2Vec2){540, 300}, worldId, (Vector2){50.0f, 50.0f});

    ctx = InitNuklear(fontSize);
    Color randomColor;

    while (!WindowShouldClose()) {

        delta = GetFrameTime();
        time -= delta;
        UpdateNuklear(ctx);
        b2World_Step(worldId, delta, subStepCount);
        // printf("Color Cicle: %s\n", colorCicle? "True" : "False");
        if (time >= 0.0f)
        {
            printf("Time %f\n", time);
        } 
        else
        {
            printf("Timer out!\n");
            time = 0.0f;
        }

        if (nk_begin(ctx, "My window", nk_rect(20, 75, 220, 220), 
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE))
        {
            nk_layout_row_static(ctx, 50, 160, 1);
            nk_label_colored(ctx, "This text is going crazy!!", NK_TEXT_ALIGN_LEFT, ColorToNuklear(randomColor));
        }
        nk_end(ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawBody(bodyId, (Vector2){50.f, 50.0f});
        DrawNuklear(ctx);
        EndDrawing();
    }

    // Unload Window and Nuklear GUI:
    CloseWindow();
    UnloadNuklear(ctx);
    b2DestroyWorld(worldId);
    return 0;
}
