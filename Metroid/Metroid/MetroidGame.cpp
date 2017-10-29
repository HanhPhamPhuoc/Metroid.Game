#include "MetroidGame.h"

MetroidGame::MetroidGame(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate)
	:Game(hInstance, windowName, screenWidth, screenHeight, isFullScreen, frameRate)
{

}
MetroidGame::~MetroidGame()
{
	batch.Release();
}

void MetroidGame::CreateGame()
{
	//create batch to draw everything
	batch.Create();

	//set cam position
	cam.SetPosition(320, 240);

	//set the camera to be used by this batch
	batch.SetCamera(&cam);

	//backGroundTexture = Texture("Resources/DemoScreen05.jpg");
	//backGroundSprite = Sprite(&backGroundTexture);
	//backGroundSprite.SetSize(1000, 480);
	//backGroundSprite.SetPosition(backGroundSprite.GetSize().X/2-320, 0);

	//create character
	characterTexture = Texture("Resources/samusaran_sheet.png");
	characterSprite = Sprite(&characterTexture, 16*9, 16*5, 244, 36, 17, 33, Vector2(0.2, 0.0));
	characterSprite2 = Sprite(&characterTexture, 16*15, 16*5, 244, 36, 17, 33, Vector2(0.0, 0.0));
	characterSprite3 = Sprite(&characterTexture, 16 * 5, 16 * 5, 244, 36, 17, 33, Vector2(0.0, 0.0));
	/*characterSprite4 = Sprite(&characterTexture, 16 * 8, 16 * 1, 244, 36, 17, 33, Vector2(0.0, 0.0), Vector2(0, 0));*/
	characterSprite.SetSize(34, 66);
	characterSprite2.SetSize(34, 66);
	characterSprite3.SetSize(34, 66);
	/*characterSprite4.SetSize(200, 10);*/

	//setup animation
	TexturePacker p = TexturePacker(&characterTexture, "Resources/samusaran_packer.xml");
	ani.AddRegion(p.GetRegion("charactermove"));
	
	//load map
	mapLoader.AddMap("map1", "Resources/map1.tmx");
	map = mapLoader.GetMap("map1");
	map->SetCamera(&cam);
}

void MetroidGame::UpdateGame(float dt)
{
	Collision collision;

	// Create gravity to make the character falls
	//characterSprite.SetYVelocity(-0.2);

	if (input.GetKey(DIK_RIGHT))
	{
		characterSprite.SetXVelocity(0.2);
		characterSprite.SetRegion(ani.Next(dt, true));
		if (!collision.checkCollision(characterSprite, characterSprite2, dt, 0) && !collision.checkCollision(characterSprite, characterSprite3, dt, 0))
		{
				characterSprite.SetPosition(characterSprite.GetPosition().X + characterSprite.GetVelocity().X * dt, characterSprite.GetPosition().Y + characterSprite.GetVelocity().Y * dt);
		}		
	}        

	if (input.GetKey(DIK_LEFT))
	{
		characterSprite.SetXVelocity(-0.2);
		characterSprite.SetRegion(ani.Next(dt, false));
		if (!collision.checkCollision(characterSprite, characterSprite2, dt, 0) && !collision.checkCollision(characterSprite, characterSprite3, dt, 0))
		{
			/*if (!collision.checkCollision(characterSprite, characterSprite3, dt, 0))*/
				characterSprite.SetPosition(characterSprite.GetPosition().X + characterSprite.GetVelocity().X * dt, characterSprite.GetPosition().Y + characterSprite.GetVelocity().Y * dt);
		}
	}

	//if (input.GetKey(DIK_UP))
	//{
	//	characterSprite.SetRegion(ani.Next(dt, false));

	//}

	if (characterSprite.GetPosition().X > cam.GetPosition().X)
	{
		cam.SetPosition(characterSprite.GetPosition().X, cam.GetPosition().Y);
	}

	if (characterSprite.GetPosition().X < cam.GetPosition().X - 250 && cam.GetPosition().X>320)
	{
		cam.SetPosition(cam.GetPosition().X - dt*0.2, cam.GetPosition().Y);
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
		ShutDownApplication();
	}


	batch.Begin();

	map->Render(batch);

	//batch.Draw(backGroundSprite);
	batch.Draw(characterSprite);
	batch.Draw(characterSprite2);
	batch.Draw(characterSprite3);
	

	batch.End();

}
