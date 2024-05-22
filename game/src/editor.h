#pragma once
#include "raylib.h"

typedef struct khEditorData {
    // editor values
    bool EditorBoxActive;
    bool Reset;
    bool SimulationNotActive;
    bool MassShrink;

    // body values
    float MassValue;
    float GravitationValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    Color BodyColor;
    int ColorTypeActive;
    
    // world values
    float GravityValue;
    float DampingValue;
    float GravityScaleValue;
    float StiffnessValue;
    float RestitutionValue;
    float FixedTimeStamp;
} khEditorData_t;

extern khEditorData_t khEditorData;

extern bool khEditorActive;
extern bool khEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct khBody* GetBodyIntersect(struct khBody* bodies, Vector2 position);
void DrawLineBodyToPosition(struct khBody* body, Vector2 position);