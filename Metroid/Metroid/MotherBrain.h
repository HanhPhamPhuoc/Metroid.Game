#pragma once
#include "HanabiWorld.h"
#include "HanabiSprite.h"
#include<vector>
#include "CollisionBit.h"



class MotherBrain : public Sprite
{

private:
	Animation animation;
	Animation weakAnimation;
	Body* body;


	int health;

	bool isDead;

public:
	MotherBrain();
	~MotherBrain();
	void Create(World *world, Texture *skreeTexture, int x, int y);

	void Render(SpriteBatch *batch);

	void Update(float dt);

	void OnHitBullet();

	void OnHitBomb();

	bool IsDead();

	int GetHealth();
};

