#pragma once
#include "spring.h"
#include "body.h"

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

}


void ApplySpringForce(khSpring_t* springs)
{
    /*for (ncSpring_t* spring = springs; spring; spring = spring->next)
    {
        Vector2 direction = <get direction vector from body2 to body1>
            if (direction.x == 0 && direction.y == 0) continue;

        float length = <get length from direction>
            float x = <compute displacement from current length to resting length>;
        float force = <compute force using product of displacement and stiffness(k)>;

        Vector2 ndirection = <get direction normal>

            ApplyForce(spring->body1, <apply force>, FM_FORCE);
        ApplyForce(spring->body2, <apply negative force>, FM_FORCE);
    }*/
}