#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "body.h"
#include "render.h"

#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data

bool khEditorActive = true;
bool khEditorIntersect = false;
khEditorData_t khEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 1500, 100 }; // 1st number make it bigger
Texture2D cursorTexture;

bool EditorActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/terminal/style_terminal.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    HideCursor();

    khEditorData.GravitationValue = 2;
    khEditorData.MassValue = 1.0f;
    
    khEditorData.BodyTypeEditMode = false;
    khEditorData.BodyTypeActive = 0;

    khEditorData.GravityValue = 0.0f;
    khEditorData.DampingValue = 0.0f;

    khEditorData.GravityScaleValue = 1.0f;
    khEditorData.StiffnessValue = 10.0f;
    khEditorData.RestitutionValue = 0.3f;

    khEditorData.BodyColor = WHITE;
    khEditorData.ColorTypeActive = 0;

    khEditorData.FixedTimeStamp = 0.01f; // supposed to be 1/50

    editorRect = (Rectangle){ anchor01.x - 120, anchor01.y, 456, 806 };
}

void UpdateEditor(Vector2 position) 
{
    // toggle show/hide editor box with key press
    if (IsKeyPressed(KEY_E)) khEditorData.EditorBoxActive = !khEditorData.EditorBoxActive;

    // check if cursor position is intersecting the editor box
    khEditorIntersect = khEditorData.EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

Color GetRandomColor()
{
    return (Color) { GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255 };
}

void DrawEditor(Vector2 position)
{
    if (khEditorData.BodyTypeEditMode) GuiLock();

    if (khEditorData.EditorBoxActive)
    {
        EditorActive = !GuiWindowBox((Rectangle) { anchor01.x - 120, anchor01.y, 456, 806 }, "Editor");

        // bodies
        GuiGroupBox((Rectangle) { anchor01.x + 16.5f, anchor01.y + 40, 304, 424 }, "Bodies");
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 120, 120, 16 }, "Mass", EDITOR_DATA(khEditorData.MassValue), 0.5f, 20);
        if (GuiDropdownBox((Rectangle) { anchor01.x + 40, anchor01.y + 60, 120, 24 }, "BT_DYNAMIC;BT_KINEMATIC;BT_STATIC", & khEditorData.BodyTypeActive, khEditorData.BodyTypeEditMode)) khEditorData.BodyTypeEditMode = !khEditorData.BodyTypeEditMode;
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 160, 120, 16 }, "Damping", EDITOR_DATA(khEditorData.DampingValue), 0, 40);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 200, 120, 16 }, "Gravity Scale", EDITOR_DATA(khEditorData.GravityScaleValue), 0, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 240, 120, 16 }, "Stiffness", EDITOR_DATA(khEditorData.StiffnessValue), 0, 40);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 280, 120, 16 }, "Restitution", EDITOR_DATA(khEditorData.RestitutionValue), 0, 2);
        GuiColorPicker((Rectangle) { anchor01.x + 160, anchor01.y + 320, 120, 120 }, EDITOR_DATA(khEditorData.BodyColor), khEditorData.BodyColor);

        if (GuiDropdownBox((Rectangle) { anchor01.x + 30, anchor01.y + 360, 120, 24 }, "Single Color;Multi Color", & khEditorData.ColorTypeActive, true))
        {
            if (khEditorData.ColorTypeActive == 0) {

            }
            else
            {
                khEditorData.BodyColor = GetRandomColor(); // Set random color for Multi Color
            }
        }

        // world
        GuiGroupBox((Rectangle) { anchor01.x + 16.5f, anchor01.y + 500, 304, 242 }, "World");
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 540, 120, 16 }, "Gravity", EDITOR_DATA(khEditorData.GravityValue), -10, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 580, 120, 16 }, "Gravitation", EDITOR_DATA(khEditorData.GravitationValue), -20, 20);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 620, 120, 16 }, "Time Step", EDITOR_DATA(khEditorData.FixedTimeStamp), 0.01f, 1);

        GuiToggle((Rectangle) { anchor01.x + 200, anchor01.y + 755, 120, 30 }, "Stop Simulation", & khEditorData.SimulationNotActive);
        GuiToggle((Rectangle) { anchor01.x - 111, anchor01.y + 40, 120, 30 }, "Shrink Mass", & khEditorData.MassShrink);
        if (GuiButton((Rectangle) { anchor01.x + 20, anchor01.y + 755, 120, 30 }, "Reset"))
        {
            khEditorData.Reset = true;
        }
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);

    GuiUnlock();
}

struct khBody* GetBodyIntersect(struct khBody* bodies, Vector2 position)
{
    for (khBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(khBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}