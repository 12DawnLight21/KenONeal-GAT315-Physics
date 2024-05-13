#pragma once
#include "raylib.h"

typedef struct khEditorData {
    float MinMassValue;
    float MaxMassValue;
    float GravitationValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float GravityValue;
    float DampingValue;
} khEditorData_t;

extern khEditorData_t khEditorData;

extern bool khEditorActive;
extern bool khEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct khBody_t* GetBodyIntersect(struct khBody_t* bodies, Vector2 position);
void DrawLineBodyToPosition(struct khBody_t* body, Vector2 position);