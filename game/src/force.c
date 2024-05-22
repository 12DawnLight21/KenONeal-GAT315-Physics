#include "force.h"
#include "body.h"

void ApplyGravitation(khBody* bodies, float strength)
{
    for (khBody* body1 = bodies; body1; body1 = body1->next) 
    {
        for (khBody* body2 = bodies; body2; body2 = body2->next) 
        {
            if (body1 == body2) continue; //<if bodies are the same, continue>

            Vector2 direction = Vector2Subtract(body1->position, body2->position); //< calculate direction(body position - body position)
            float distance = Vector2Length(direction); // <get length of vector>

            if (distance < 1) distance = 1; // <clamp distance to a minumum of 1>
            float force = (body1->mass * body2->mass / (distance * distance)) * strength; // <(mass1 * mass2 / (distance * distance)) * strength>;

            direction = Vector2Scale(Vector2Normalize(direction), force); // <scale normalized direction vector by force>;

            ApplyForce(body1, Vector2Scale(direction, -1), FM_FORCE); // ApplyForce(<apply force to body 1, one of them is negative>);
            
            ApplyForce(body2, direction, FM_FORCE); // ApplyForce(<apply force to body 2, one of them is negative>);
        }
    }
}