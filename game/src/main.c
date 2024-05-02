#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "integrator.h"
#include "world.h"

#include <stdlib.h>
#include <assert.h>

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60); // VR targets 100FPS

	// initialize world
	khGravity = (Vector2){ 0, 30 };

	// 'game loop'
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime(); // NO CLASSES 
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			khBody* body = CreateBody();
			body->position = position;
			body->mass = GetRandomFloatValue(1, 10);
			body->inverseMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 0.5f; // 0.5f
			body->gravityScale = 5; // anything i want but i think 5 works
			
			body->color = ColorFromHSV(GetRandomFloatValue(0, 255), GetRandomFloatValue(0, 255), GetRandomFloatValue(0, 255));
			
			ApplyForce(body, (Vector2) { GetRandomFloatValue(-100, 100), GetRandomFloatValue(-100, 100) }, FM_VELOCITY);
		}
		
		// apply force
		khBody* body = khBodies;
		while (body)
		{
			//ApplyForce(body, CreateVector2(0, -50), FM_FORCE);
			body = body->next;
		}

		// update bodies
		//khBody* body = khBodies;
		body = khBodies; 
		while (body)
		{
			Step(body, dt);
			body = body->next;
		}

		// render / draw
		BeginDrawing();
		ClearBackground(BLACK);

		//stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, RED);
		
		// draw bodies
		//khBody* body = khBodies;
		body = khBodies; 
		while (body)
		{
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
			body = body->next;
		}

		EndDrawing();
	}

	CloseWindow();
	free(khBodies); // have to free (delete) bodies

	return 0;
}