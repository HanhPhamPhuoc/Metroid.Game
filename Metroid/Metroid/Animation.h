//a class to easily manage the animation of an object 
#pragma once

#include "stdafx.h"
#include "Texture.h"
class Animation
{
public:
	Texture _CurrentSprite; 
	int _Index;						// Current sprite index
	int _Width;								// Sprite width
	int _Height;							// Sprite height
	int _Count;								// Number of sprites
	int _SpritePerRow;						// Number of sprites per row
	float _LeftOffset;				//The left offset in case the first sprite is not placed at (0,0)
	float _TopOffset;				//The top offset in case the first sprite is not placed at (0,0)
	LPCWSTR _FilePath;
public:
	Animation();
	Animation(LPDIRECT3DDEVICE9 d3ddev, LPWSTR FilePath, int Width, int Height, int Count, int SpritePerRow);
	~Animation();
	Animation(const Animation &ani);
	Animation& operator=(const Animation &ani);
	

	Texture GetKeyAnimation();
	void Next();
	void Reset();
	void SetOffset(float leftOffset, float topOffset);
};

