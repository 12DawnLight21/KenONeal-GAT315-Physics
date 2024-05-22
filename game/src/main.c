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
#include "contact.h"
#include "collision.h"

#include <stdlib.h>
#include <assert.h>

int main(void)
{
	khBody* selectedBody = NULL; // or highlighted body ig
	khBody* connectBody = NULL;
	khContact_t* contacts = NULL;

	float fixedTimeStep = 1.0f / khEditorData.FixedTimeStamp;
	float timeAccumulator = 0;

	InitWindow(1980, 1280, "Ken ONeal - Physics Engine");
	InitEditor();
	SetTargetFPS(60); // VR targets 100FPS

	// initialize world
	khGravity = (Vector2){ 0,-1 };

	Color bodyColor = BLANK;

	// 'game loop'
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime(); 
		float fps = (float)GetFPS();
		khGravity = (Vector2){ 0, -khEditorData.GravityValue };
		bodyColor = khEditorData.BodyColor;

		Vector2 position = GetMousePosition();
		khScreenZoom -= GetMouseWheelMove() * 0.2f; 
		khScreenZoom = Clamp(khScreenZoom, 0.1f, 10); 
		UpdateEditor(position);

		if (khEditorData.Reset)
		{
			DestroyAllBodies();
			DestroyAllSprings();
			khEditorData.Reset = false;
		}

		if (!khEditorIntersect)
		{
			selectedBody = GetBodyIntersect(khBodies, position);
			if (selectedBody)
			{
				Vector2 screen = ConvertWorldToScreen(selectedBody->position);
				DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass) + 5, YELLOW);
			}

			// create body
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT))
			{
				for (int i = 0; i < 1; i++)
				{
					khBody* body = CreateBody(ConvertScreenToWorld(position), khEditorData.MassValue, khEditorData.BodyTypeActive);
					body->damping = khEditorData.DampingValue; 
					body->gravityScale = khEditorData.GravityScaleValue;
					body->restitution = khEditorData.RestitutionValue;
					body->color = bodyColor;

					AddBody(body);
				}
			}
		}

		// connect springs
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (connectBody)
		{
			Vector2 world = ConvertScreenToWorld(position);
			if (connectBody->type == BT_STATIC || connectBody->type == BT_KINEMATIC)
			{
				connectBody->position = world;
			}
			else
			{
				ApplySpringForcePosition(world, connectBody, 0, khEditorData.StiffnessValue, 5);
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody) 
		{
			if (selectedBody && selectedBody != connectBody)
			{
				khSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), khEditorData.StiffnessValue);
				AddSpring(spring);
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) 
		{
			selectedBody = NULL; 
			connectBody = NULL;
		}
		
		timeAccumulator += dt;

		while (timeAccumulator >= khEditorData.FixedTimeStamp)
		{
			timeAccumulator -= khEditorData.FixedTimeStamp;

			if (!khEditorData.SimulationNotActive)
			{
				// apply gravity n force
				ApplyGravitation(khBodies, khEditorData.GravitationValue);
				ApplySpringForce(khSprings);

				// update bodies
				for (khBody* body = khBodies; body; body = body->next)
				{
					Step(body, dt);

					// makes the body shrink :D
					if (khEditorData.MassShrink)
					{
						body->mass -= body->mass * 0.02;
						//if (body->mass <= 0.2) DestroyBody(body); // causes a read-access error in body.c when applying force
					}

					Vector2 screen = ConvertWorldToScreen(body->position);
					DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
				}

				DestroyAllContacts(&contacts);
				CreateContacts(khBodies, &contacts);
				SeparateContacts(contacts);
				ResolveContacts(contacts);
			}

			else
			{
				continue;
			}
		}

		// render / draw
		BeginDrawing();
		ClearBackground(BLACK);

		//stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", khEditorData.FixedTimeStamp), 10, 30, 20, LIME);

		// draw bodies
		for (khBody* body = khBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}

		// draw contacts
		for (khContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		// draw springs
		for (khSpring_t* spring = khSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		DrawEditor(position);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}