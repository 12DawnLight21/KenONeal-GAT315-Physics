#pragma once
#include "body.h"

extern khBody* khBodies; // extern - prevents it from being defined outside of here
extern int khBodyCount;

khBody* CreateBody();
void DestroyBody(khBody* body);
void DestroyAllBodies();