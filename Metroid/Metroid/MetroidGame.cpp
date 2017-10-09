#include "MetroidGame.h"

MetroidGame::MetroidGame(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate)
	:Game(hInstance, windowName, screenWidth, screenHeight, isFullScreen, frameRate)
{

}
MetroidGame::~MetroidGame()
{
	batch.Release();
}

void MetroidGame::InitGame()
{
	Game::InitGame();
}
void MetroidGame::RunGame()
{
	Game::RunGame();
}


void MetroidGame::CreateGame()
{
	//
	batch.Create();

	//this is example 
	SetBackGroundImage(L"Resources/DemoScreen05.JPG");
	world = World(100, 100, DeltaTime);
	obj = Object(300, 300, 10, 10);
	obj.SetVelocity(Vector2(0.1f,0));
	ani = Animation(L"Resources/character.png", 75, 86, 27, 7, 20);
	obj.SetTexture(ani.GetKeyAnimation());
	world.AddObject(obj);
}

void MetroidGame::UpdateGame()
{
	if (Input::GetKey(DIK_RIGHT))
	{
		obj.SetPosition(Vector2( DeltaTime*obj.GetVelocity().X + obj.GetPosition().X, obj.GetPosition().Y));
		obj.SetTexture(ani.GetKeyAnimation());
		ani.Next(DeltaTime, 1);
	}

	if (Input::GetKey(DIK_LEFT))
	{
		obj.SetPosition(Vector2(obj.GetPosition().X -DeltaTime*obj.GetVelocity().X , obj.GetPosition().Y));
		obj.SetTexture(ani.GetKeyAnimation());
		ani.Next(DeltaTime, 0);
	}

	batch.Begin();

	batch.Draw(*obj.GetTexture(), obj.GetPosition().X, obj.GetPosition().Y);



	batch.End();

	//world.Update(GetDeltaTime());

}