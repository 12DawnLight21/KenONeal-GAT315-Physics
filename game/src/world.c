#pragma once
#include "world.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

khBody_t* khBodies = NULL;
int khBodyCount = 0;
Vector2 khGravity; //= (Vector2){ 0, 30 }

khBody_t* CreateBody(Vector2 position, float mass, khBodyType bodyType)
{
	khBody_t* body = (khBody_t*)malloc(sizeof(khBody_t));
	assert(body);

	memset(body, 0, sizeof(khBody_t));
    body->position = position;
    body->mass = mass;
    body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
    body->type = bodyType;

	return body;
}

void AddBody(khBody_t* body)
{
    assert(body);

    // add element to linked list
	body->prev = NULL;
	body->next = khBodies;

	if (khBodies)
	{
		khBodies->prev = body;
	}

	// set head of elements to new element
	khBodies = body;
	khBodyCount++;
}

void DestroyBody(khBody_t* body)
{
	assert(body);

    if (body->prev)
    {
        body->prev->next = body->next; 
    }
    
    if (body->next) 
    {
        body->next->prev = body->prev; 
    }
    
    if (body == khBodies) 
    {
        khBodies = body->next; 
    }
 
    khBodyCount--; 

    free(body); 
}

void DestroyAllBodies()
{

}