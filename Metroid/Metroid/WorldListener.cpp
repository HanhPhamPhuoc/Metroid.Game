#include "WorldListener.h"



WorldListener::WorldListener()
{
}


WorldListener::~WorldListener()
{
}

void WorldListener::OnContact(Body* bodyA, Body *bodyB)
{
	if (bodyA->GetID()._Equal("Foot") && !bodyB->GetID()._Equal("Player"))
	{
		bodyA->GetExtra()->isGrounded = true;


	}
}