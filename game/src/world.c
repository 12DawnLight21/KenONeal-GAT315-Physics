#pragma once
#include "world.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

khBody* khBodies = NULL;
int khBodyCount = 0;

khBody* CreateBody()
{
	khBody* body = (khBody*)malloc(sizeof(khBody));
	assert(body);

	memset(body, 0, sizeof(khBody));

	body->prev = NULL;
	body->next = khBodies;

	if (khBodies)
	{
		khBodies->prev = body;
	}

	khBodies = body;
	khBodyCount++;

	return body;
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

}