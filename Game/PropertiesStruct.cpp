#include "PropertiesStruct.h"

Properties::Property* Properties::GetProperty(const char* name)
{
    if (list.Count() == 0) return nullptr; // If no properties have been set return nullptr
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}