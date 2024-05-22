#pragma once
#include "spring.h"
#include "body.h"
#include "editor.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

khSpring_t* khSprings = NULL;

khSpring_t* CreateSpring(struct khBody* body1, struct khBody* body2, float restLength, float k)
{
    khSpring_t* spring = (khSpring_t*)malloc(sizeof(khSpring_t));
    assert(spring);

    memset(spring, 0, sizeof(khSpring_t));
    spring->body1 = body1;
    spring->body2 = body2;
    spring->restLength = restLength;
    spring->k = k;

    return spring;
}

void AddSpring(khSpring_t* spring)
{
    assert(spring);

    // add element to linked list
    spring->prev = NULL;
    spring->next = khSprings;

    if (khSprings)
    {
        khSprings->prev = spring;
    }

    // set head of elements to new element
    khSprings = spring;
    //khBodyCount++;
}

void DestroySpring(khSpring_t* spring)
{
    assert(spring);

    if (spring->prev)
    {
        spring->prev->next = spring->next;
    }

    if (spring->next)
    {
        spring->next->prev = spring->prev;
    }

    if (spring == khSprings)
    {
        khSprings = spring->next;
    }

    //khBodyCount--;

    free(spring);
}

void DestroyAllSprings()
{
    if (!khSprings) return;

    khSpring_t* spring = khSprings; 
    while (spring) 
    {
        khSpring_t* next = spring->next;
        free(spring);
        spring = next;
    }

    khSprings = NULL;
}


void ApplySpringForce(khSpring_t* springs)
{
    for (khSpring_t* spring = springs; spring; spring = spring->next)
    {
        Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position); //<get direction vector from body2 to body1>
            if (direction.x == 0 && direction.y == 0) continue;

        float length = Vector2Length(direction); //<get length from direction>
            float x = length - spring->restLength; //<compute displacement from current length to resting length>;
        float force = -spring->k * x; //<compute force using product of displacement and stiffness(k)>;

        Vector2 ndirection = Vector2Normalize(direction); //<get direction normal>

        ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE); //<apply force>, FM_FORCE);
        ApplyForce(spring->body2, Vector2Scale(Vector2Negate(ndirection), force), FM_FORCE); //<apply negative force>, FM_FORCE);
    }
}

void ApplySpringForcePosition(Vector2 position, khBody* body, float restLength, float k, float damping)
{
    // Check if the body pointer is null; if so, exit the function
    if (!body) return;

    // Calculate the direction vector from the body's position to the given position
    Vector2 direction = Vector2Subtract(position, body->position);

    // If the direction vector is zero (i.e., positions are the same), exit the function
    if (direction.x == 0 && direction.y == 0) return;

    // Calculate the length of the direction vector (distance between the two positions)
    float length = Vector2Length(direction);

    // Calculate the displacement from the rest length
    float x = length - restLength;//<get displacement>

    // Apply Hooke's Law (f = -kx) to determine the spring force
    float force = -k * x; //<Hooke’s Law>

    // Normalize the direction vector
    Vector2 ndirection = Vector2Normalize(direction); //<normalize vector>

    // Calculate the damping force (opposing force due to velocity)
    float dampingForce = damping * Vector2DotProduct(body->velocity, ndirection); //<damping * DotProduct(body velocity, ndirection)>

    // Calculate the total force by combining spring force and damping force
    float totalForce = force + dampingForce;//<force + damping force>

    // Apply the total force to the body in the direction of the normalized vector
    ApplyForce(body, Vector2Scale(ndirection, -totalForce), FM_FORCE); //ApplyForce(body, <scale the normalized direction vector by - totalForce >, FM_FORCE);
}