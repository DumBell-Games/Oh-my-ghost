#include "TransitionTrigger.h"

TransitionTrigger::TransitionTrigger() : Entity(EntityType::TRANSITION)
{
}

TransitionTrigger::~TransitionTrigger()
{
}

bool TransitionTrigger::Awake()
{

    transition.doorId = parameters.attribute("doorID").as_int(0);

    return true;
}

bool TransitionTrigger::Start()
{
    return true;
}

bool TransitionTrigger::Update(float dt)
{
    return true;
}

bool TransitionTrigger::CleanUp()
{
    return true;
}

void TransitionTrigger::OnCollision(PhysBody* physA, PhysBody* physB)
{
}
