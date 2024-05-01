#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum khBodyType
{
	STATIC,
	KINEMATIC,
	DYNAMIC
} khBodyType;

typedef struct khBody
{
	khBodyType body;
	// force -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)

	struct khBody* next;
	struct khBody* prev;
} khBody;

inline void ApplyForce(khBody* body, Vector2 force)
{
	body->force = Vector2Add(body->force, force);
}

inline void ClearForce(khBody* body)
{
	body->force = Vector2Zero();
}