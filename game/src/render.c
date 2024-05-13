#include "render.h"
#include "raymath.h"

Vector2 khScreenSize = { 1280, 720 };
float khScreenZoom = 1.0f;
float khViewSize = 25.0f;

Vector2 ConvertScreenToWorld(Vector2 screen)
{
	float ratio = khScreenSize.x / khScreenSize.y;
	Vector2 extents = (Vector2){ khViewSize * ratio, khViewSize };
	extents = Vector2Scale(extents, khScreenZoom);

	Vector2 lower = Vector2Negate(extents);
	Vector2 upper = extents;

	float nx = screen.x / khScreenSize.x;
	float ny = (khScreenSize.y - screen.y) / khScreenSize.y;

	Vector2 world;
	world.x = ((1 - nx) * lower.x) + (nx * upper.x);
	world.y = ((1 - ny) * lower.y) + (ny * upper.y);

	return world;
}

Vector2 ConvertWorldToScreen(Vector2 world)
{
	float ratio = khScreenSize.x / khScreenSize.y;
	Vector2 extents = (Vector2){ khViewSize * ratio, khViewSize };
	extents = Vector2Scale(extents, khScreenZoom);

	Vector2 lower = Vector2Negate(extents);
	Vector2 upper = extents;

	float nx = (world.x - lower.x) / (upper.x - lower.x);
	float ny = (world.y - lower.y) / (upper.y - lower.y);

	Vector2 screen;
	screen.x = nx * khScreenSize.x;
	screen.y = (1.0f - ny) * khScreenSize.y;

	return screen;
}

float ConvertWorldToPixel(float world)
{
	float ratio = khScreenSize.x / khScreenSize.y;
	Vector2 extents = (Vector2){ khViewSize * ratio, khViewSize };
	extents = Vector2Scale(extents, khScreenZoom);

	float nx = world / extents.x;
	float pixel = nx * khScreenSize.x;

	return pixel;
}
