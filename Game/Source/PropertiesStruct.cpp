#include "PropertiesStruct.h"

bool LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;
    pugi::xml_node propertiesNode = node.child("properties").child("property");
    if (propertiesNode)
    {
        ret = true;
        for (/*Initial state already set above*/; propertiesNode; propertiesNode = propertiesNode.next_sibling("property"))
        {
            Properties::Property* p = new Properties::Property();

            SString nameAttr = propertiesNode.attribute("name").as_string();
            SString typeAttr = propertiesNode.attribute("type").as_string("string"); // Default to string for malformed properties

            p->name = nameAttr;
            if (typeAttr == "string") {
                p->strVal = propertiesNode.attribute("value").as_string();
            }
            else if (typeAttr == "bool") {
                p->value = propertiesNode.attribute("value").as_bool();
            }
            else if (typeAttr == "int") {
                p->intVal = propertiesNode.attribute("value").as_int();
            }
            else if (typeAttr == "float") {
                p->floatVal = propertiesNode.attribute("value").as_float();
            }

            properties.list.Add(p);
        }
    }
    return ret;
}