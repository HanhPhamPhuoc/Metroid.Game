#include "MetroidGame.h"

MetroidGame::MetroidGame(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate)
	:Game(hInstance, windowName, screenWidth, screenHeight, isFullScreen, frameRate)
{

}
MetroidGame::~MetroidGame()
{
	batch.Release();
}

void MetroidGame::RunGame()
{
	Game::RunGame();
}


void MetroidGame::CreateGame()
{
	batch.Create();

	////this is example 
	//world = World(100, 100, DeltaTime);
	/*obj = Object(300, 300, 10, 10);
	obj.SetVelocity(Vector2(0.1f,0));*/
	//obj.SetTexture(ani.GetKeyAnimation());
	//world.AddObject(obj);


	cam.SetPosition(0, 0);

	batch.SetCamera(&cam);

	texture = Texture(L"Resources/DemoScreen05.jpg");
	sprite = Sprite(&texture);
	sprite.SetSize(1000, 480);
	sprite.SetPosition(sprite.GetSize().X/2-320, 0);

	texture2 = Texture(L"Resources/character.png");
	sprite2 = Sprite(&texture2, -220, -120, 0, 0, 75, 86, 75, 86);
	ani = Animation(&sprite2, 27, 7, 20);
	//sprite2.SetRotation(sprite.GetRotation() + 20);
}

//void MetroidGame::Resize(float x, float y)
//{
//	cam.Resize(x, y);
//}

void MetroidGame::UpdateGame(float dt)
{

	//sprite.SetRotation(sprite.GetRotation()+5*dt);

	if (input.GetKey(DIK_RIGHT))
	{
		ani.Next(dt, true);
		sprite2.SetPosition(sprite2.GetPosition().X+dt*0.2, sprite2.GetPosition().Y);
	}

	if (input.GetKey(DIK_LEFT))
	{
		ani.Next(dt, false);
		sprite2.SetPosition(sprite2.GetPosition().X - dt*0.2, sprite2.GetPosition().Y);
	
	}

	if(sprite2.GetPosition().X > cam.GetPosition().X)
	{
		cam.SetPosition(sprite2.GetPosition().X, cam.GetPosition().Y);
	}

	if (sprite2.GetPosition().X < cam.GetPosition().X - 250 && cam.GetPosition().X>0)
	{
		cam.SetPosition(cam.GetPosition().X-dt*0.2, cam.GetPosition().Y);
	}

	if (input.GetKey(DIK_UP))
	{	
		cam.SetPosition(cam.GetPosition().X, cam.GetPosition().Y + +dt*0.2);
	}

	if (input.GetKey(DIK_DOWN))
	{
		cam.SetPosition(cam.GetPosition().X, cam.GetPosition().Y - dt * 0.2);
	}

	if (input.GetKeyDown(DIK_ESCAPE))
	{
		input.ShutDownApplication();
	}


	batch.Begin();


	batch.Draw(sprite);
	//batch.Draw(sprite2);
	batch.Draw(*ani.GetKeyAnimation());

	


	batch.End();

}
