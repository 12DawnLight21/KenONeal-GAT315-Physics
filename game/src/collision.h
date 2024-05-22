#pragma once

#include "raylib.h"

typedef struct khContact khContact_t;
typedef struct khBody khBody;

void CreateContacts(khBody* bodies, khContact_t** contacts);
khContact_t* GenerateContact(khBody* body1, khBody* body2);

void SeparateContacts(khContact_t* contacts);
void ResolveContacts(khContact_t* contacts);