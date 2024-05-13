#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "body.h"
#include "render.h"

bool khEditorActive = true;
bool khEditorIntersect = false;
khEditorData_t khEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 900, 100 };
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
    khEditorData.MinMassValue = 1.0f;
    khEditorData.MaxMassValue = 3.0f;
    khEditorData.BodyTypeEditMode = false;
    khEditorData.BodyTypeActive = 0;
    khEditorData.GravityValue = 0.0f;
    khEditorData.DampingValue = 0.0f;
}

void UpdateEditor(Vector2 position)
{
    //
    
}

void DrawEditor(Vector2 position)
{
    if (khEditorData.BodyTypeEditMode) GuiLock();

    if (EditorActive)
    {
        EditorActive = !GuiWindowBox((Rectangle) { anchor01.x, anchor01.y, 336, 576 }, "Editor");
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 120, 120, 16 }, "Min Mass", NULL, & khEditorData.MinMassValue, 0, 5);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 160, 120, 16 }, "Max Mass", NULL, & khEditorData.MaxMassValue, 0, 5);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 400, 120, 16 }, "Gravitation", NULL, & khEditorData.GravitationValue, 0, 20);
        GuiGroupBox((Rectangle) { anchor01.x + 16.5f, anchor01.y + 40, 304, 224 }, "Bodies");
        GuiGroupBox((Rectangle) { anchor01.x + 16.5f, anchor01.y + 320, 304, 232 }, "World");
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 360, 120, 16 }, "Gravity", NULL, & khEditorData.GravityValue, 0, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 200, 120, 16 }, "Damping", NULL, & khEditorData.DampingValue, 0, 40);
        if (GuiDropdownBox((Rectangle) { anchor01.x + 100, anchor01.y + 60, 120, 24 }, "BT_DYNAMIC;BT_KINEMATIC;BT_STATIC", & khEditorData.BodyTypeActive, khEditorData.BodyTypeEditMode)) khEditorData.BodyTypeEditMode = !khEditorData.BodyTypeEditMode;
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);

    GuiUnlock();
}

khBody_t* GetBodyIntersect(khBody_t* bodies, Vector2 position)
{
    for (khBody_t* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(khBody_t* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}