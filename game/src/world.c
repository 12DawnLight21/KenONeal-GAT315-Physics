#pragma once
#include "world.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

khBody* khBodies = NULL;
int khBodyCount = 0;
Vector2 khGravity; //= (Vector2){ 0, 30 }

khBody* CreateBody(Vector2 position, float mass, khBodyType bodyType)
{
	khBody* body = (khBody*)malloc(sizeof(khBody));
	assert(body);

	memset(body, 0, sizeof(khBody));
    body->position = position;
    body->mass = mass;
    body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
    body->type = bodyType;

	return body;
}

void AddBody(khBody* body)
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

void DestroyBody(khBody* body)
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
    if (!khBodies) return;

    khBody* body = khBodies;
    while (body)
    {
        khBody* next = body->next;
        free(body);
        body = next;
    }

    khBodies = NULL;
}