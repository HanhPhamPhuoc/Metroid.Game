#include "PlayScene.h"



PlayScene::PlayScene()
{
}


PlayScene::~PlayScene()
{
}

void PlayScene::SetBatch(SpriteBatch* batch)
{
	this->batch = batch;
	//set the camera to be used by this batch
	batch->SetCamera(&cam);
}

void PlayScene::Create()
{
	stateTime = 0;

	sdQuadTree.Load("Resources/map3SDQuadTree.xml", "Resources/map3.tmx");
	//load map
	mapLoader.AddMap("map1", "Resources/map3.tmx",1);
	map = mapLoader.GetMap("map1");
	map->SetCamera(&cam);
	map->SetSpaceDivisionQuadTree(&sdQuadTree);

	//world
	world.SetGravity(-10);
	world.SetContactListener(&worldListener);
	world.SetCamera(NULL);

	std::vector<Shape::Rectangle> platformRects = map->GetObjectGroup("Platform")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = platformRects.begin(); rect!= platformRects.end(); ++rect)
	{
		Platform platform(&world, rect->x, rect->y, rect->width, rect->height);
	}

	//get player position
	Shape::Rectangle playerRect = map->GetObjectGroup("Player")->GetRects().front();
	player.Create(&world, playerRect.x,playerRect.y);

	//set cam position
	cam.SetPosition(playerRect.x, playerRect.y + 110);

	//--------------------------ENEMIES-------------------------------
	enemiesTexture = Texture("Resources/enemies.png");

	//skree
	std::vector<Shape::Rectangle> skreeRects = map->GetObjectGroup("Skree")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = skreeRects.begin(); rect != skreeRects.end(); ++rect)
	{
		Skree *skree = new Skree();
		skree->Create(&world, &enemiesTexture, rect->x, rect->y);
		skrees.push_back(skree);
	}

	//zoomer
	std::vector<Shape::Rectangle> zoomerRects = map->GetObjectGroup("Zoomer")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = zoomerRects.begin(); rect != zoomerRects.end(); ++rect)
	{
		Zoomer *zoomer = new Zoomer();
		zoomer->Create(&world, &enemiesTexture, rect->x, rect->y, true);

		zoomers.push_back(zoomer);
	}



	//---------------------------ITEMS------------------------------------
	itemsTexture = Texture("Resources/items.png");

	//roll ability item
	Shape::Rectangle rollItemRect = map->GetObjectGroup("MaruMariItem")->GetRects().front();
	maruMariItem.Create(&world, &itemsTexture, rollItemRect.x, rollItemRect.y);

	//bomb ability item
	Shape::Rectangle bombItemRect = map->GetObjectGroup("BombItem")->GetRects().front();
	bombItem.Create(&world, &itemsTexture, bombItemRect.x, bombItemRect.y);

	//effects
	effectsTexture = Texture("Resources/metroidfullsheet.png");
	explosionEffect.Create(&effectsTexture);

	//--------------------------UI--------------------------------------
	font = Font("Arial");
	playerHealthLabel = Label("30", &font, cam.GetPosition().x-250, cam.GetPosition().y+300, 640, 480);
}

void PlayScene::HandlePhysics(float dt)
{
	//handle input of player
	player.HandleInput();
	
	//handle physics skrees
	for (std::vector<Skree*>::iterator it = skrees.begin(); it != skrees.end(); ++it)
	{
		(*it)->HandlePhysics(&player);
	}

	//handle physics zoomer
	for (std::vector<Zoomer*>::iterator it = zoomers.begin(); it != zoomers.end(); ++it)
	{
		(*it)->HandlePhysics();
	}


	//Update world
	world.Update(dt);


}

void  PlayScene::Render()
{
	//start drawing
	batch->Begin();

	//render player
	player.Render(batch);

	//render skrees
	for (std::vector<Skree*>::iterator it = skrees.begin(); it != skrees.end(); ++it)
	{
		(*it)->Render(batch);
	}

	//render zoomers
	for (std::vector<Zoomer*>::iterator it = zoomers.begin(); it != zoomers.end(); ++it)
	{
		(*it)->Render(batch);
	}
	
	//draw items
	batch->Draw(maruMariItem);
	batch->Draw(bombItem);


	for (std::vector<HealthItem*>::iterator it = healthItems.begin(); it != healthItems.end(); ++it)
	{
		(*it)->Render(batch);
	}
	
	explosionEffect.Render(batch);

	//draw bodies
	world.RenderBodiesDebug(batch);

	//render map
	map->Render(batch);

	//end drawing
	batch->End();
	

	//Label
	playerHealthLabel.Draw(&cam);
}


void PlayScene::Update(float dt)
{
	if (stateTime < PLAYERAPPEARINGTIME) //On player appearing, don't do anything except rendering
	{
		stateTime += dt;
		player.OnAppearing(dt);
		Render();
		return;
	}

	HandlePhysics(dt);

	player.Update(dt);

	//update skrees
	for (int i= 0; i < skrees.size(); i++) //not use iterator for the sake of erase dead skree (we can't delete an element in skrees if we use iterator loop) 
	{
		Skree* skree = skrees[i];
		skree->Update(dt);
		if (skree->IsDead())
		{
			if (skree->GetHealth()<=0)
			{
				explosionEffect.SetPosition(skree->GetPosition().x, skree->GetPosition().y);
				explosionEffect.Play();
				//instantiate health item
				HealthItem *healthItem = new HealthItem();
				healthItem->Create(&world, &itemsTexture, skree->GetPosition().x, skree->GetPosition().y);
				healthItems.push_back(healthItem);
			}
			//delete skree
			delete skree;
			skree = NULL;
			skrees.erase(skrees.begin() + i);

		}
	}

	//update zoomers
	for (std::vector<Zoomer*>::iterator it = zoomers.begin(); it != zoomers.end(); ++it)
	{
		(*it)->Update(dt);
	}

	explosionEffect.Update(dt);

	//update items
	maruMariItem.Update(dt);
	bombItem.Update(dt);

	for (int i =0; i< healthItems.size(); i++)
	{
		HealthItem* healthItem = healthItems[i];
		healthItem->Update(dt);
		if (healthItem->IsHitPlayer())
		{
			delete healthItem;
			healthItem = NULL;
			healthItems.erase(healthItems.begin() + i);
		}
	}

	//if (player.GetPosition().x > cam.GetPosition().x)
	//{
	//	cam.SetPosition(player.GetPosition().x, cam.GetPosition().y);
	//}

	//if (player.GetPosition().x < cam.GetPosition().x - 250)
	//{
	//	cam.SetPosition(player.GetPosition().x + 250, cam.GetPosition().y);
	//	if (cam.GetPosition().x < 320) cam.SetPosition(320, cam.GetPosition().y);
	//}

	cam.SetPosition(player.GetPosition().x,  cam.GetPosition().y);

	//update Label
	playerHealthLabel.SetText(std::to_string(player.GetHealth()));
	playerHealthLabel.SetPosition(cam.GetPosition().x - 250, cam.GetPosition().y + 200);



	//RENDER
	Render();
}

void PlayScene::Release()
{
	world.Release();
	player.Release();
	for (std::vector<Zoomer*>::iterator it = zoomers.begin(); it != zoomers.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}

	for (std::vector<Skree*>::iterator it = skrees.begin(); it != skrees.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}

	for (std::vector<HealthItem*>::iterator it = healthItems.begin(); it != healthItems.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}

	font.Release();
}
