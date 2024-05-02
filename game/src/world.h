#pragma once
//#include "body.h"
#include "raylib.h"

typedef struct khBody khBody; // c version of forward declaration

extern khBody* khBodies; // extern - prevents it from being defined outside of here
extern int khBodyCount;
extern Vector2 khGravity;

khBody* CreateBody();
void DestroyBody(khBody* body);
void DestroyAllBodies();