/**********************************
 *
 * CraBOOM engine THE epic physics program - Legacy code
 * Code by: @RafaDEV99 (Rafael Tangyan Guskov)
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
#include "Includes/physac.h"

#define RAYGUI_IMPLEMENTATION
#include "Includes/raygui.h"

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

int circleRad = 0;

int PolySides = 0;
int PolyRadius = 0;

int ValueGetX = 0;
int ValueGetY = 0;

PhysicsBody bodiesList[55] = { 0 };
// TODO: Rectangle BodieRect[16] = { 0 };

// Function yoinked from the Physac examples :3
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

// TODO: add more to this function :P
void DrawBodySubGui(int valueBodyIndex, float x_obj_position, float y_obj_position)
{
    // NOTE: When the Object is selected (or just created), it will show a diferent GUI
    // WARNING: Its better to use the enum the provided in the enums section (You can olso include a new instanciator function)

    const char* ObjectName = GetBodyTypeName(valueBodyIndex);

    float WBoxWidth = 300.0f;
    float WBoxHeight = 200.0f;

    GuiWindowBox((Rectangle){10.0f, 490.0f, WBoxWidth, WBoxHeight}, TextFormat("%s properties", ObjectName));

    char valueBoxTextX[16] = "X ";
    char valueBoxTextY[16] = "Y ";

    char valueBuff[64];
    int value = 0;

    // TODO: 
    switch (valueBodyIndex) 
    {
        case RECTANGLE_BODY:
            // GuiValueBox((Rectangle){x_obj_position - 20, y_obj_position - 20, 120, 50}, valueBoxTextX, &value, 0, WINDOW_WIDTH, editVal);
            break;
        case CIRCLE_BODY:
            break;
        case POLYGON_BODY:
            break;
        default:
            fprintf(stderr, "can´t detect that buddy!");
            break;
    }
}

int main()
{
    // Initialization functions
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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

    // Booleans for managing the state of two value boxes:
    bool editValX = false;
    bool editValY = false;

    bool dencityEdit = false;

    float gravityX = 0.0f;
    float gravityY = 0.0f;

    Vector2 MousePosition = { 0.0f, 0.0f };

    while (!WindowShouldClose()) 
    {
        // Update values
        delta = GetFrameTime();
        BodiesCount = GetPhysicsBodiesCount();
        MousePosition = GetMousePosition();

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

            DrawBodySubGui(ValueDrop, 0, 0);

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
    
    ClosePhysics();
    CloseWindow();
    return 0;
}
