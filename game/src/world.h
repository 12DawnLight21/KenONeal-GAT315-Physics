#pragma once
#include "raylib.h"
#include "body.h"

typedef struct khBody khBody; // c version of forward declaration

extern khBody* khBodies; // extern - prevents it from being defined outside of here
extern int khBodyCount;
extern Vector2 khGravity;

khBody* CreateBody(Vector2 position, float mass, khBodyType bodyType);
void AddBody(khBody* body);
void DestroyBody(khBody* body);
void DestroyAllBodies();