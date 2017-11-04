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

	//create character
	characterTexture = Texture("Resources/samusaran_sheet.png");


	object1 = Object(&characterTexture, 16 * 8, 16 * 5, 244, 36, 17, 33);
	object2 = Object(&characterTexture, 16 * 30, 16 * 5, 244, 36, 17, 33);
	object3 = Object(&characterTexture, 16 * 11, 16 * 5, 244, 36, 17, 33);

	object1.SetSize(34, 66);
	object2.SetSize(34, 66);
	object3.SetSize(34, 66);

	object1.FitBody();
	object2.FitBody();
	object3.FitBody();

	object1.GetBody().SetBodyType(Body::BodyType::Dynamic);
	object1.GetBody().SetLinearDrag(10, 0.2);
	object1.GetBody().SetMass(2);
	object2.GetBody().SetBodyType(Body::BodyType::Dynamic);

	//setup animation
	TexturePacker p = TexturePacker(&characterTexture, "Resources/samusaran_packer.xml");
	ani.AddRegion(p.GetRegion("charactermove"));
	
	//load map
	mapLoader.AddMap("map1", "Resources/map2.tmx");
	map = mapLoader.GetMap("map1");
	map->SetCamera(&cam);

	world.SetGravity(-10);
	world.AddBody(&object1.GetBody());
	world.AddBody(&object2.GetBody());
	world.AddBody(&object3.GetBody());

	
	world.AddBody(map->GetObjectGroup("Land")->GetBodies());
	world.AddBody(map->GetObjectGroup("Wall 1")->GetBodies());
	world.AddBody(map->GetObjectGroup("Wall 2")->GetBodies());
	world.AddBody(map->GetObjectGroup("Wall 3")->GetBodies());
	world.AddBody(map->GetObjectGroup("Platform 1")->GetBodies());
	world.AddBody(map->GetObjectGroup("Platform 2")->GetBodies());
}

void MetroidGame::UpdateGame(float dt)
{
	//object1.GetBody().SetVelocity(0, -100000);
	if (input.GetKey(DIK_RIGHT))
	{
		object1.GetBody().SetVelocity(5, object1.GetBody().GetVelocity().y);
		object1.SetRegion(ani.Next(dt, true));
	}        

	if (input.GetKey(DIK_LEFT))
	{
		object1.GetBody().SetVelocity(-5, object1.GetBody().GetVelocity().y);
		object1.SetRegion(ani.Next(dt, false));
	}

	if (input.GetKeyDown(DIK_SPACE))
	{
		object1.GetBody().SetVelocity(object1.GetBody().GetVelocity().x, 8);
	}

	object2.SetVelocity(-2, 0);


	if (object1.GetPosition().x > cam.GetPosition().x)
	{
		cam.SetPosition(object1.GetPosition().x, cam.GetPosition().y);
	}
	


	if (input.GetKey(DIK_UP))
	{	
		cam.SetPosition(cam.GetPosition().x, cam.GetPosition().y + +dt*200);
	}

	if (input.GetKey(DIK_DOWN))
	{
		cam.SetPosition(cam.GetPosition().x, cam.GetPosition().y - dt * 200);
	}

	if (input.GetKeyDown(DIK_ESCAPE))
	{
		ShutDownApplication();
	}

	//Update world
	world.Update(dt);

	object1.Update(dt);

	//start drawing
	batch.Begin();
	
	//render map
	map->Render(batch);

	//
	batch.Draw(object1);
    batch.Draw(object2);
	batch.Draw(object3);

	//
	world.RenderBodiesDebug(batch);

	//end drawing
	batch.End();

}
