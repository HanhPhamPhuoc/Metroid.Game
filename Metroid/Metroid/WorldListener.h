#pragma once
#include"WorldContactListener.h"
#include"Player.h"

class WorldListener: public WorldContactListener
{
public:
	WorldListener();
	~WorldListener();

	//Override OnContact of WorldContactListener
	void OnCollisionEnter(Body* bodyA, Body *bodyB) override;

	void OnSersorEnter(Body *bodyA, Body *bodyB) override;

};

