#include "Zoomer.h"



Zoomer::Zoomer()
{
}


Zoomer::~Zoomer()
{
}

void Zoomer::Create(World *world, Texture *zoomerTexture, float x, float y, bool initalDirection)
{
	this->world = world;
	TexturePacker p = TexturePacker(zoomerTexture, "Resources/enemies_packer.xml");

	zoomerAnimation.AddRegion(p.GetRegion("zoomer"));
	zoomerAnimation.SetFrameInterval(0.04);

	SetRegion(*zoomerAnimation.GetKeyAnimation());
	SetSize(25, 25);
	SetPosition(x, y);

	//setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.position.Set(x, y);
	bodyDef.size.Set(25, 25);
	body = world->CreateBody(bodyDef);
	body->categoryBits = ZOOMER_BIT;
	body->maskBits = PLAYER_BIT | PLATFORM_BIT | BULLET_BIT;
	body->PutExtra(this);

	prevCollisionDirection.x = NOT_COLLIDED;
	prevCollisionDirection.y = -prevVelocity.y * 100;
	curCollisionDirection.x = NOT_COLLIDED;
	curCollisionDirection.y = -prevVelocity.y * 100;

	//set up intial velocity/direction
	if (initalDirection)
	{
		prevVelocity.x = 0.7f;
		prevVelocity.y = -0.7f;

		body->SetVelocity(0.7f, -0.7f);
	}
	else
	{
		prevVelocity.x = -0.7f;
		prevVelocity.y = -0.7f;

		body->SetVelocity(-0.7f, -0.7f);
	}
	cooldownAfterCollisionChange = 3;
	health = 2;
}

void Zoomer::HandlePhysics()
{

}

void Zoomer::Render(SpriteBatch *batch)
{
	if (isDead)
		return;

	batch->Draw(*this);
}

void Zoomer::Update(float dt)
{
	if (isDead) return;

	if (health == 0)
	{
		isDead = true;
		world->DestroyBody(body);
		return;
	}

	if (hitBulletTime == -1) //-1 means not being hit by bullet
	{
		SetRegion(*zoomerAnimation.Next(dt));
	}
	else
	{
		if (hitBulletTime < MAXHITBULLETTIME)
		{
			hitBulletTime += dt;
		}
		else
		{
			hitBulletTime = -1;
			body->SetBodyType(Body::BodyType::Kinematic);
		}
	}

	//set sprite position
	if (body != NULL)
		this->SetPosition(body->GetPosition().x, body->GetPosition().y);

	//body->SetVelocity(0.5f, -0.5f);
	if (body->GetBodyType() != Body::BodyType::Static)
	{
		StickToGround();
	}
	SetRegion(*zoomerAnimation.Next(dt));

}

void Zoomer::SetCurCollisionDirection(Vector2 collisionDirection, int source)
{
	switch (source)
	{

	case 0:
		if (abs(collisionDirection.x) == abs(prevCollisionDirection.y) && abs(collisionDirection.y) == abs(prevCollisionDirection.x))
		{
			if (prevCollisionDirection.x != NOT_COLLIDED)
			{
				curCollisionDirection.y = collisionDirection.y;
			}
			else
			{
				curCollisionDirection.x = collisionDirection.x;
			}
			prevSource = 0;
		}
		else
		{
			curCollisionDirection = collisionDirection;
			prevCollisionDirection = curCollisionDirection;
			prevSource = 0;
		}
		break;

	case 1:
		curCollisionDirection = collisionDirection;
		prevSource = 1;
		break;

	case 2:
		prevSource = 1;
		cooldownAfterCollisionChange = 3;
		break;
	}
}

void Zoomer::StickToGround()
{

	if (curCollisionDirection.x != NOT_COLLIDED && curCollisionDirection.y != NOT_COLLIDED && cooldownAfterCollisionChange == 0)
	{
		if (prevCollisionDirection.x == NOT_COLLIDED)
		{
			body->SetVelocity(prevVelocity.x, -prevVelocity.y);
			prevVelocity.y = -prevVelocity.y;
			prevCollisionDirection.x = curCollisionDirection.x;
			prevCollisionDirection.y = NOT_COLLIDED;
		}
		else
		{
			body->SetVelocity(-prevVelocity.x, prevVelocity.y);
			prevVelocity.x = -prevVelocity.x;
			prevCollisionDirection.y = curCollisionDirection.y;
			prevCollisionDirection.x = NOT_COLLIDED;
		}

		cooldownAfterCollisionChange = 3;
	}
	else
	{
		//(prevCollisionDirection.x != curCollisionDirection.x || prevCollisionDirection.y != curCollisionDirection.y) && cooldownAfterCollisionChange == 0
		if (prevSource == -1 && cooldownAfterCollisionChange == 0)
		{
			//prevCollisionDirection.x == curCollisionDirection.x && prevCollisionDirection.y != curCollisionDirection.y
			if (prevCollisionDirection.x == NOT_COLLIDED)
			{
				body->SetVelocity(-prevVelocity.x, prevVelocity.y);
				prevCollisionDirection.x = prevVelocity.x * 100;
				prevVelocity.x = -prevVelocity.x;
				//prevCollisionDirection.y = curCollisionDirection.y;
				prevCollisionDirection.y = NOT_COLLIDED;
				cooldownAfterCollisionChange = 3;
				prevSource = -1;
			}
			else
			{
				//prevCollisionDirection.x != curCollisionDirection.x && prevCollisionDirection.y == curCollisionDirection.y
				if (prevCollisionDirection.y == NOT_COLLIDED)
				{
					body->SetVelocity(prevVelocity.x, -prevVelocity.y);
					prevCollisionDirection.x = NOT_COLLIDED;
					prevCollisionDirection.y = prevVelocity.y * 100;
					prevVelocity.y = -prevVelocity.y;
					cooldownAfterCollisionChange = 3;
					prevSource = -1;
				}
			}
		}
		else
		{
			body->SetVelocity(prevVelocity.x, prevVelocity.y);
			if (cooldownAfterCollisionChange > 0)
				cooldownAfterCollisionChange--;
			prevSource = -1;
		}
	}
}

void Zoomer::OnHitBullet()
{
	if (hitBulletTime != -1) return;
	health--;
	hitBulletTime = 0;
	//stop this body a little bit 
	body->SetBodyType(Body::BodyType::Static);
}