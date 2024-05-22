#pragma once
#include "raylib.h"

typedef struct khContact
{
    struct khBody* body1;
    struct khBody* body2;

    float restitution;
    float depth;
    Vector2 normal;

    struct khContact* next;
} khContact_t;

void AddContact(khContact_t* contact, khContact_t** contacts);
void DestroyAllContacts(khContact_t** contacts);