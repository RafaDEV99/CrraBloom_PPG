/**********************************
 *
 * CraBOOM engine THE epic physics program
 * Code by: @RafaDEV99
 * Alpha code
 *
 *********************************/

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External include (from the include folder :P )
#define PHYSAC_IMPLEMENTATION
#include "../Includes/physac.h"

// Gui Header files
#define RAYGUI_IMPLEMENTATION
#include "../Includes/raygui.h"

// NOTE: Add this later:
// #define RAYLIB_NUKLEAR_IMPLEMENTATION
// #include <raylib-nuklear.h>
// #include <raylib-nuklear-font.h>

// All defines and macros goes here
#define WINDOW_WIDTH   1080
#define WINDOW_HEIGHT  700
#define WINDOW_TITLE   "CraBOOM engine - Raylib"

#define GRAVITY_Y  9.8f
#define GRAVITY_X  0.0f

// Enums
enum ObjectType
{
    RECTANGLE_BODY,
    CIRCLE_BODY,
    POLYGON_BODY,
};

// Global Values (Inseted boxes values also):
int RectHeight = 40;
int RectWidth = 40;

int circleRad = 20;

int PolySides = 15;
int PolyRadius = 15;

int ValueGetX = 0;
int ValueGetY = 0;

PhysicsBody bodiesList[128] = { 0 };
// TODO: Rectangle BodieRect[16] = { 0 };

// Function yoinked from the Physac examples :3
// This function draws the bodies
void DrawCollisionWireFrames(int count)
{
    for (int i = 0; i < count; i++)
    {
        PhysicsBody body = GetPhysicsBody(i);

        if (body != NULL)
        {
            int vertexCount = GetPhysicsShapeVerticesCount(i);
            for (int j = 0; j < vertexCount; j++)
            {
                // Get physics bodies shape vertices to draw lines
                // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                DrawLineV(vertexA, vertexB, DARKGRAY);     // Draw a line between two vertex positions
            }
        }

    }
}

const char* GetBodyTypeName(int bodyIndex)
{
    switch (bodyIndex) 
    {
        case RECTANGLE_BODY:
            return "Rectangle";
        case CIRCLE_BODY:
            return "Circle";
        case POLYGON_BODY:
            return "Polygon";
        default:
            return "MissingNo";
            break;
    }
}

bool editVal = true;
bool editValueY = true;

int main()
{
    // Initialization functions
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    InitPhysics();
    SetTargetFPS(60);

    // NOTE: Init varibles
    float delta;
    bool buttonFlag = false;

    
    bool DropEdit = false; // <-- Drop down menu 1
    int ValueDrop = RECTANGLE_BODY;

    int GetDencity = 0;
    int BodiesCount = 0;

    // Main booleans:
    bool editValX = false;
    bool editValY = false;

    bool dencityEdit = false;

    float gravityX = 0.0f;
    float gravityY = 0.0f;

    Vector2 MousePosition = { 0.0f, 0.0f };

    while (!WindowShouldClose()) 
    {
        // Update
        delta = GetFrameTime();
        BodiesCount = GetPhysicsBodiesCount();
        MousePosition = GetMousePosition();
        SetPhysicsGravity(GRAVITY_X, GRAVITY_Y);

        if (IsKeyPressed(KEY_D))
        {
            for (int i = 0; i < BodiesCount; i++)
            {
                DestroyPhysicsBody(bodiesList[i]);
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("CraBOOM engine - Alpha 2", 10, 40, 20, DARKGRAY);
            DrawText("Press ESC to exit", 300, 40, 20, DARKGRAY);
            DrawText(TextFormat("Bodies: %d", BodiesCount), 950, 10, 20, DARKGRAY);
            DrawFPS(10, 10);
            DrawCollisionWireFrames(BodiesCount);
            if (GuiButton((Rectangle){10, 90, 120, 50}, "#8#add body"))
            {
                buttonFlag = !buttonFlag;
            }

            if (buttonFlag == true)
            {
                if (GuiWindowBox((Rectangle){10, 170, 300, 300}, "Body Creation"))
                {
                    buttonFlag = !buttonFlag;
                }

                // Gui Value boxes layer
                if (GuiValueBox((Rectangle){30, 270, 120, 40}, "x ", &ValueGetX, 0, WINDOW_WIDTH, editValX))
                    editValX = !editValX;
                if (GuiValueBox((Rectangle){170, 270, 120, 40}, "y ", &ValueGetY, 0, WINDOW_HEIGHT, editValY))
                    editValY = !editValY;
                if (GuiValueBox((Rectangle){30, 330, 120, 40}, " ", &GetDencity, 0.0f, 500.0f, dencityEdit))
                    dencityEdit = !dencityEdit;
                if (GuiDropdownBox((Rectangle){30, 210, 120, 40}, "Rectangle;Circle;Polygon", &ValueDrop, DropEdit))
                    DropEdit = !DropEdit;

                GuiLabel((Rectangle){165, 335, 60, 25}, "<-- Dencity");

                if (GuiButton((Rectangle){30, 390, 120, 50}, "CREATE!!!")) // <-- If the button is true do the next:
                {
                    PhysicsBody newBody;
                    Vector2 BVector = {ValueGetX, ValueGetY}; // <-- Body Vector int

                    // Create a body based on the value of the drop down menu
                    switch (ValueDrop) 
                    {
                        case RECTANGLE_BODY:
                            newBody = CreatePhysicsBodyRectangle(BVector, RectWidth, RectHeight, GetDencity);
                            ValueGetX = newBody->position.x;
                            ValueGetY = newBody->position.y;
                            break;
                        case CIRCLE_BODY:
                            newBody = CreatePhysicsBodyCircle(BVector, circleRad, GetDencity);
                            break;
                        case POLYGON_BODY:
                            newBody = CreatePhysicsBodyPolygon(BVector, PolyRadius, PolySides, GetDencity);
                            break;
                        default:
                            break;
                    }

                    if (newBody != NULL)
                        bodiesList[BodiesCount] = newBody;

                }

            }
            
        EndDrawing();
    }
    
    // Memory liveration
    ClosePhysics();
    CloseWindow();
    return 0;
}
