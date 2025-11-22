#include "box2d/id.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include <raylib.h>
#include <raymath.h>
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

b2BodyId CreateObject(b2Vec2 position, b2WorldId worldId, Vector2 size, b2BodyType type)
{
    b2BodyDef def = b2DefaultBodyDef();
    def.position = position;
    def.type = type;

    b2BodyId id = b2CreateBody(worldId, &def);

    // Box extend
    // NOTE: We multitplty by 0.5 if we want half the box (That is what we need)
    b2Polygon box = b2MakeBox(size.x * 0.5f, size.y * 0.5f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.material.restitution = 0.2f;
    shapeDef.density = 20.0f;
    shapeDef.material.friction = 0.4f;

    b2CreatePolygonShape(id, &shapeDef, &box);
    b2Body_ApplyMassFromShapes(id);

    return id;
}

void DrawRotatedRectOutline(Vector2 center, Vector2 size, float angle, Color color)
{
    Vector2 half = { size.x / 2, size.y / 2 };

    Vector2 verts[4] = {
        {-half.x, -half.y},
        { half.x, -half.y},
        { half.x,  half.y},
        {-half.x,  half.y}
    };

    for (int i = 0; i < 4; i++)
    {
        verts[i] = Vector2Rotate(verts[i], angle);
        verts[i].x += center.x;
        verts[i].y += center.y;
    }

    for (int i = 0; i < 4; i++)
    {
        DrawLineEx(verts[i], verts[(i + 1) % 4], 2, color);
    }
}

void DrawBody(b2BodyId id, Vector2 size, Color color)
{
    b2Vec2 pos = b2Body_GetPosition(id);
    b2Vec2 Ppos = b2Body_GetWorldPoint(id, (b2Vec2){-size.x * 0.5f, -size.y * 0.5f});
    b2Vec2 Bextend = {size.x * 0.5f, size.y * 0.5f};
    float angle = b2Rot_GetAngle(b2Body_GetRotation(id));

    DrawRectanglePro(
        (Rectangle){pos.x, pos.y, size.x, size.y},
        (Vector2){Bextend.x, Bextend.y},
        angle * RAD2DEG,
        color
    );

    DrawRotatedRectOutline((Vector2){pos.x, pos.y}, (Vector2){size.x, size.y}, angle, DARKGRAY);

}

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CrraBloom Simulator!");
    SetTargetFPS(60);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 9.8f * UNITS_PER_METER;

    b2WorldId worldId = b2CreateWorld(&worldDef);
    b2SetLengthUnitsPerMeter(UNITS_PER_METER);

    int fontSize = FONT_SIZE;
    bool colorCicle = true;

    float time = 5.0f;
    int subStepCount = 6;

    Vector2 bodySize = {50.0f, 50.0f};
    Vector2 floorSize = {300.0f, 50.0f};
    b2BodyId bodyId = CreateObject((b2Vec2){540, 200}, worldId, bodySize, b2_dynamicBody);
    b2BodyId cube1 = CreateObject((b2Vec2){540, 100}, worldId, (Vector2){50.0f, 50.0f}, b2_dynamicBody);
    b2BodyId floor = CreateObject((b2Vec2){540, 500}, worldId, floorSize, b2_staticBody);

    ctx = InitNuklear(fontSize);
    Color randomColor;

    while (!WindowShouldClose()) {

        delta = GetFrameTime();
        time -= delta;
        UpdateNuklear(ctx);

        // printf("Color Cicle: %s\n", colorCicle? "True" : "False");
        if (time >= 0.0f)
        {
            printf("Time %f\n", time);
        } 
        else
        {
            // printf("Timer out!\n");
            b2World_Step(worldId, delta, subStepCount);
            time = 0.0f;
        }

        if (IsKeyDown(KEY_SPACE))
        {
            b2Vec2 F = {100000000000.0f, 0.0f};
            b2Body_ApplyForceToCenter(bodyId, F, true);
        }

        if (nk_begin(ctx, "My window", nk_rect(20, 75, 220, 220), 
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE))
        {
            nk_layout_row_static(ctx, 50, 160, 1);
            nk_label_colored(ctx, "This text is going crazy!!", NK_TEXT_ALIGN_LEFT, ColorToNuklear(randomColor));
        }
        nk_end(ctx) ;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawBody(bodyId, bodySize, BLUE);
        DrawBody(floor, floorSize, GREEN);
        DrawBody(cube1, (Vector2){50.0f, 50.0f}, RED);
        DrawNuklear(ctx);
        EndDrawing();
    }

    // Unload Window and Nuklear GUI:
    CloseWindow();
    UnloadNuklear(ctx);
    b2DestroyWorld(worldId);
    return 0;
}
