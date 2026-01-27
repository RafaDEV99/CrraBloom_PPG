#include "box2d/collision.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
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
float gravity = 9.8f;

typedef struct PhysicsObject
{
    b2BodyDef Def;
    b2BodyId Id;
    Vector2 size;
    Vector2 position;
} PhysicsObject;

// NOTE: Temp:
// PhysicsObject CreatePhysicsObject(Vector2 position, Vector2 size, b2BodyType bodyType, b2WorldId world)
// {
//     // Created a PhysicsObject type (The rest of the values are avilable)
//     PhysicsObject object;
//
//     object.Def = b2DefaultBodyDef();
//
//     object.size = size;
//     object.position = position;
//     object.Def.position = (b2Vec2){object.position.x, object.position.y};
//     object.Def.type = bodyType;
//
//     object.Id = b2CreateBody(world, &object.Def);
//
//     b2Polygon shape;
//
//     return object;
// }

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

    b2CreatePolygonShape(id, &shapeDef, &box);
    b2Body_ApplyMassFromShapes(id);

    return id;
}

b2BodyId CreateCircleObject(b2Vec2 position, b2WorldId worldId, int radius, b2BodyType type)
{
    b2BodyDef CircleBody = b2DefaultBodyDef();

    CircleBody.position = position;
    CircleBody.type = type;

    b2BodyId CircleId = b2CreateBody(worldId, &CircleBody);
    b2Circle circle = {{0.0f, 0.0f}, radius};
    b2ShapeDef CircleShapeDef = b2DefaultShapeDef();

    b2CreateCircleShape(CircleId, &CircleShapeDef, &circle);

    return CircleId;
}

void DrawRotatedRectOutline(Vector2 center, Vector2 size, float angle, int borderThickness, Color color)
{
    Vector2 half = { size.x / 2, size.y / 2 };

    Rectangle wireRect = {-half.x, -half.y, size.x, size.y};

    rlPushMatrix();
    rlTranslatef(center.x, center.y, 0);
    rlRotatef(angle * RAD2DEG, 0, 0, 1);
    DrawRectangleLinesEx(wireRect, borderThickness, color);
    rlPopMatrix();

}

void DrawBody(b2BodyId id, Vector2 size, Color color)
{
    b2Vec2 pos = b2Body_GetPosition(id);
    int borderSize = 2;
    b2Vec2 Ppos = b2Body_GetWorldPoint(id, (b2Vec2){-size.x * 0.5f, -size.y * 0.5f});
    b2Vec2 Bextend = {size.x * 0.5f, size.y * 0.5f};
    float angle = b2Rot_GetAngle(b2Body_GetRotation(id));

    DrawRectanglePro(
        (Rectangle){pos.x, pos.y, size.x, size.y},
        (Vector2){Bextend.x, Bextend.y},
        angle * RAD2DEG,
        color
    );

    DrawRotatedRectOutline((Vector2){pos.x, pos.y}, (Vector2){size.x, size.y}, angle, borderSize, DARKGRAY);
}

void DrawCapsule2D(Vector2 start, Vector2 end, float radius, float rotation, Color color)
{
    DrawCircleV(start, radius, color);
    DrawCircleV(end, radius, color);

    DrawLineEx(start, end, radius * 2.0f, color);
}

void DrawCapsule2dWires(Vector2 start, Vector2 end, int diameter, float rotation, Color color);

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CrraBloom Simulator!");
    SetTargetFPS(60);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = gravity * UNITS_PER_METER;

    b2WorldId worldId = b2CreateWorld(&worldDef);
    b2SetLengthUnitsPerMeter(UNITS_PER_METER);

    int fontSize = FONT_SIZE;
    bool colorCicle = true;
    bool drawAll = true;

    float time = 0.0f;
    int subStepCount = 6;

    b2Vec2 ForceDirection = {-100000000.0f, 0.0f};
    float forcePower = 100000000.0f;
    int cicleRadius = 25.0f;

    b2Vec2 CirclePos = {540.0f, 70.0f};

    Vector2 bodySize = {50.0f, 50.0f};
    Vector2 floorSize = {540.0f, 100.0f};
    b2BodyId EpicCircle = CreateCircleObject(CirclePos, worldId, cicleRadius, b2_dynamicBody);
    b2BodyId bodyId = CreateObject((b2Vec2){540, 200}, worldId, bodySize, b2_dynamicBody);
    b2BodyId cube1 = CreateObject((b2Vec2){540, 100}, worldId, (Vector2){50.0f, 50.0f}, b2_dynamicBody);
    b2BodyId floor = CreateObject((b2Vec2){540, 500}, worldId, floorSize, b2_staticBody);
    b2BodyId bodies[1000] = {0};

    bool hasBody = false;
    int bodyCount = 0;

    Color randCubeColor;
    Vector2 TestVector = {300.0f, 200.0f};

    ctx = InitNuklear(fontSize);
    PhysicsObject object;

    float rotate = 0.0f;

    while (!WindowShouldClose()) {

        delta = GetFrameTime();
        time += delta;
        UpdateNuklear(ctx);
        b2World_Step(worldId, delta, subStepCount);

        printf("Time: %f\n", time);
        printf("Counters: %d\n", b2World_GetCounters(worldId).bodyCount);
        if (time >= 0.5f)
        {
            randCubeColor = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
            time = 0.0f;
        }

        if (IsKeyPressed(KEY_R))
        {
            for (int i = 0; i < bodyCount; i++)
            {
                b2BodyId b = bodies[i];
                if (b2Body_IsValid(b))
                {
                    b2DestroyBody(bodies[i]);
                    bodies[i] = b2_nullBodyId;
                }
            }
        }

        switch (GetKeyPressed()) {
            case KEY_LEFT:
                ForceDirection = (b2Vec2){-forcePower, 0.0f};
                break;
            case KEY_RIGHT:
                ForceDirection = (b2Vec2){forcePower, 0.0f};
                break;
            case KEY_SPACE:
                ForceDirection = (b2Vec2){0.0f, -forcePower};
                break;
            default:
                break;
        }

        if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
        {
            b2Body_ApplyForceToCenter(bodyId, ForceDirection, true);
        }

        switch (GetKeyPressed()) {
            case KEY_LEFT:
                ForceDirection = (b2Vec2){-forcePower, 0.0f};
                break;
            case KEY_RIGHT:
                ForceDirection = (b2Vec2){forcePower, 0.0f};
                break;
            case KEY_SPACE:
                ForceDirection = (b2Vec2){0.0f, -forcePower};
                break;
            default:
                break;
        }

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT))
        {
            b2Body_ApplyForceToCenter(bodyId, ForceDirection, true);
        }

        if (nk_begin(ctx, "Body spawn control", nk_rect(20, 130, 220, 220), 
            NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE))
        {
            nk_layout_row_static(ctx, 50, 160, 1);
            nk_label(ctx, "Body spawn menu!", NK_TEXT_CENTERED);
            if (nk_button_label(ctx, "Spawn Object"))
            {
                bodies[bodyCount] = CreateObject((b2Vec2){GetRandomValue(75, 1005), 0.0f}, worldId, (Vector2){50.0f, 50.0f}, b2_dynamicBody);
                bodyCount++;
            }
        }
        nk_end(ctx);

        b2Vec2 UpdatedCirclePos = b2Body_GetPosition(EpicCircle);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawBody(bodyId, bodySize, BLUE);
        DrawBody(floor, floorSize, GREEN);
        DrawBody(cube1, (Vector2){50.0f, 50.0f}, RED);
        DrawCircle(UpdatedCirclePos.x, UpdatedCirclePos.y, cicleRadius, BLUE);
        DrawRing(
            (Vector2){UpdatedCirclePos.x, UpdatedCirclePos.y},
            cicleRadius - 2.0f,
            cicleRadius,
            0, 360,
            64,
            DARKGRAY
        );

        for (int i = 0; i < bodyCount; i++)
        {
            if (b2Body_IsValid(bodies[i]) && drawAll)
            {
                DrawBody(bodies[i], (Vector2){50.0f, 50.0f}, randCubeColor);
            }
            else
            {
                bodyCount = 0.0f;
                continue;
            }
        }

        rotate -= 10;

        DrawText(TextFormat("Objects (+3): %d", bodyCount), 20, 75, 30, GRAY);
        DrawCapsule2D((Vector2){300, 200}, (Vector2){300, 290}, 20, rotate, BLUE);
        DrawFPS(20.0f, 20.0f);

        // Capsule parameters
        Vector2 start = { 400, 150 }; // Top circle center
        Vector2 end = { 400, 180 };   // Bottom circle center
        float diameter = 20.0f;       // Capsule width


        DrawNuklear(ctx);

        EndDrawing();
    }

    // Unload libs and stuff:
    CloseWindow();
    UnloadNuklear(ctx);
    b2DestroyWorld(worldId);
    return 0;
}
