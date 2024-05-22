#pragma once
#include "raylib.h"
#include <stdlib.h>

typedef struct khSpring
{
	struct khBody* body1;
	struct khBody* body2;
	float restLength;
	float k; // stiffness

	struct khSpring* next;
	struct khSpring* prev;

} khSpring_t;

extern khSpring_t* khSprings;

khSpring_t* CreateSpring(struct khBody* body1, struct khBody* body2, float restLength, float k);
void AddSpring(khSpring_t* spring);
void DestroySpring(khSpring_t* spring);
void DestroyAllSprings();

void ApplySpringForce(khSpring_t* springs); // pass it the head
void ApplySpringForcePosition(Vector2 position, struct khBody* body, float restLength, float k, float damping);