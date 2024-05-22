#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

khContact_t* khContacts = NULL;

void AddContact(khContact_t* contact, khContact_t** contacts)
{
    assert(contact);

    // add element to linked list
    contact->next = *contacts;

    // set head of elements to new element
    *contacts = contact;
}

void DestroyAllContacts(khContact_t** contacts)
{
    if (!*contacts) return;

    khContact_t* contact = *contacts;
    while (contact)
    {
        khContact_t* next = contact->next;
        free(contact);
        contact = next;
    }

    *contacts = NULL;
}