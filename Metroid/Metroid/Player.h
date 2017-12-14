#pragma once
#include "HanabiWorld.h"
#include "HanabiSprite.h"
#include "Bullet.h"
#include <vector>
#include "CollisionBit.h"
#include "HanabiStateManager.h"

#define FIRERATE 0.1
#define MAXJUMPTIME 0.5
#define MAXUNTOUCHABLETIME 0.3 

#include "HanabiSound.h"

//Main character
class Player : public Sprite
{
private:
	Texture texture;

	Animation appearingAnimation;
	Animation standingAnimation;
	Animation movingAnimation;
	Animation jumpingAnimation;
	Animation standAndShootupAnimation;
	Animation moveAndShootAnimation;
	Animation moveAndShootupAnimation;
	Animation jumpAndShootupAnimation;
	Animation jumpAndShootAnimation;
	Animation rollingAnimation;
	Animation jumpAndRollAnimation;
	Animation beingHitAnimation;
	StateManager characterStateManager;

	Body* mainBody;
	Body* foot;
	Body* head;

	std::vector<Bullet*> bullets;

	World* world;
	
	//
	float lastShot;

	float jumpTime;

	bool isGrounded;
	bool isLookingup;
	bool isShooting;
	bool isRolling;
	bool canRoll;
	bool isHeadHit;
	int health;

	float beingHitTime;
	bool beingRightHit;

public:


public:
	Player();
	~Player();

	void Create(World *world, float x, float y);

	void HandleInput();

	void Render(SpriteBatch *batch);

	void Update(float dt);

	void Fire();

	void OnAppearing(float dt);

	void OnGrounded();

	void OnHitRollItem();

	void OnHitHealthItem();

	void OnHitEnemy(bool rightHit);

	void OnHeadHit();

	int GetHealth();

	void Release();

	
};

