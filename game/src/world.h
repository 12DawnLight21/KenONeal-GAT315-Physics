#pragma once
#include "raylib.h"
#include "body.h"

typedef struct khBody_t khBody_t; // c version of forward declaration

extern khBody_t* khBodies; // extern - prevents it from being defined outside of here
extern int khBodyCount;
extern Vector2 khGravity;

khBody_t* CreateBody(Vector2 position, float mass, khBodyType bodyType);
void AddBody(khBody_t* body);
void DestroyBody(khBody_t* body);
void DestroyAllBodies();