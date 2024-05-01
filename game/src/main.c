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
			body->mass = GetRandomFloatValue(1, 5);
		}
		
		// apply force
		khBody* body = khBodies;
		while (body)
		{
			ApplyForce(body, CreateVector2(0, -50));
			body = body->next;
		}

		// update bodies
		//khBody* body = khBodies;
		body = khBodies; 
		while (body)
		{
			ExplicitEuler(body, dt);
			ClearForce(body);
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
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, YELLOW);
			body = body->next;
		}

		EndDrawing();
	}

	CloseWindow();
	free(khBodies); // have to free (delete) bodies

	return 0;
}