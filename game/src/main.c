#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "integrator.h"
#include "world.h"
#include "force.h"
#include "editor.h"
#include "render.h"
#include "spring.h"

#include <stdlib.h>
#include <assert.h>

// Function to draw a fading line segment
void DrawFadingLine(Vector2 start, Vector2 end, Color color, float fade)
{
	DrawLineEx(start, end, 2, Fade(color, fade * 255)); // Multiply fade with 255
}

int main(void)
{
	khBody_t* selectedBody = NULL; // or highlighted body ig
	khBody_t* connectBody = NULL;

	InitWindow(1280, 1280, "Ken ONeal - Physics Engine");
	InitEditor();
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
		khScreenZoom -= GetMouseWheelMove() * 0.2f; 
		khScreenZoom = Clamp(khScreenZoom, 0.1f, 10); 
		UpdateEditor(position);

		selectedBody = GetBodyIntersect(khBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass) + 5, YELLOW);
		}

		// create body
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Color ccolor = ColorFromHSV(GetRandomFloatValue(0, 255), GetRandomFloatValue(0, 255), GetRandomFloatValue(0, 255));

			for (int i = 0; i < 1; i++)
			{
				float angle = GetRandomFloatValue(0, 360);
				khBody_t* body = CreateBody(ConvertScreenToWorld(position), khEditorData.MinMassValue, khEditorData.BodyTypeActive);
				
				body->damping = khEditorData.DampingValue; 
				body->gravityScale = khEditorData.GravityValue;
				body->color = ccolor;

				AddBody(body);
			}
		}

		// connect springs
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				khSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
				AddSpring(spring);
			}
		}
		
		// apply force
		//khBody_t* body = khBodies;

		// apply gravity
		ApplyGravitation(khBodies, khEditorData.GravitationValue);
		ApplySpringForce(khSprings);

		// update bodies
		for (khBody_t* body = khBodies; body; body = body->next)
		{
			Step(body, dt);
			//body = body->next;

			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass), body->color);
		}

		// render / draw
		BeginDrawing();
		ClearBackground(BLACK);

		// Render trails
		//for (khBody* particle = khBodies; particle; particle = particle->next)
		//{
		//	for (int i = 0; i < 50 - 1; i++)
		//	{
		//		if (!Vector2IsZero(particle->trail[i]) && !Vector2IsZero(particle->trail[i + 1]))
		//		{
		//			// Draw the fading line with proper fading effect
		//			DrawFadingLine(particle->trail[i], particle->trail[i + 1], particle->color, (float)(50 - i) / (float)50);
		//		}
		//	}
		//}

		//stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		// draw bodies
		for (khBody_t* body = khBodies; body; body = body->next)
		{
			DrawCircle((int)body->position.x, (int)body->position.y, (int)body->mass, body->color);
		}

		// draw springs
		for (khSpring_t* spring = khSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body1->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		DrawEditor(position);

		EndDrawing();
	}

	CloseWindow();
	free(khBodies); // have to free (delete) bodies

	return 0;
}