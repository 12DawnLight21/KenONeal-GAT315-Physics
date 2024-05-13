#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum khBodyType // bodyType
{
	BT_DYNAMIC,
	BT_KINEMATIC,
	BT_STATIC
} khBodyType;

typedef enum khForceMode // forceMode
{
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} khForceMode;

typedef struct khBody_t
{
	khBodyType type; // type instead of body

	// force -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 accleration;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)
	float gravityScale;
	float damping;
	Color color;

	Vector2 trail[50]; // trail length

	struct khBody_t* next;
	struct khBody_t* prev;
} khBody_t;

inline void ApplyForce(khBody_t* body, Vector2 force, khForceMode forcemode)
{
	if (body->type != BT_DYNAMIC) return;

	switch (forcemode)
	{
		case FM_FORCE:
			body->force = Vector2Add(body->force, force);
			break;

		case FM_IMPULSE:
			// applies a sudden change in momentum (velocity)
			body->velocity = Vector2Scale(force, body->inverseMass);
			break;

		case FM_VELOCITY:
			body->velocity = force;
			break;
	}

}

inline void ClearForce(khBody_t* body)
{
	body->force = Vector2Zero();
}

void Step(khBody_t* body, float timestep);