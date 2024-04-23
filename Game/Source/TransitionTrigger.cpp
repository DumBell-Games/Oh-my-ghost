#include "TransitionTrigger.h"
#include "Map.h"
#include "Physics.h"

#include "PropertiesStruct.h"

#include "App.h"
#include "Log.h"

#include "Textures.h"

TransitionTrigger::TransitionTrigger() : Entity(EntityType::TRANSITION)
{
}

TransitionTrigger::~TransitionTrigger()
{
}

bool TransitionTrigger::Awake()
{
    Properties p;
    app->map->LoadProperties(parameters,p);
    transition.targetDoorID = p.GetProperty("targetDoor")->intVal;
    transition.mapId = p.GetProperty("mapID")->intVal;
    id = parameters.attribute("id").as_int();
    int x = parameters.attribute("x").as_int();
    int y = parameters.attribute("y").as_int();
    int w = parameters.attribute("width").as_int();
    int h = parameters.attribute("height").as_int();
    rect = { x,y,w,h };

    SetPosition(iPoint( x + (w/2),y + (h/2) )); // La posicion no cambia?!?!?!?!?!

    contact = 10;

    texture = app->tex->Load("Assets/Maps/MapMetadata.png");



    return true;
}

bool TransitionTrigger::Start()
{
    pbody = app->physics->CreateRectangleSensor(position.x, position.y, rect.w, rect.h, bodyType::STATIC);
    pbody->listener = this;

    if (texture) 
        app->render->DrawTexture(texture, position.x, position.y);
    return true;
}

bool TransitionTrigger::Update(float dt)
{
    contact--;
    if (contact < 0) contact = 0;
    return true;
}

bool TransitionTrigger::CleanUp()
{
    app->tex->UnLoad(texture);
    texture = nullptr;
    return true;
}

void TransitionTrigger::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype == ColliderType::PLAYER) {
        if (contact == 0) { // Usado para evitar un bucle de teletransporte
            app->map->transitionData = transition;
            app->map->ChangeMap(transition.mapId);
            contact++;
        }
        contact++;
    }
}
