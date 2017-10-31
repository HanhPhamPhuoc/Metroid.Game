#pragma once

#include "stdafx.h"
#include "Texture.h"

// draw a portion of image 
class TextureRegion
{
private:
	Texture *_Texture; //we don't initialize this variable by default, just use this to hold the reference to a texture
						//if you do want to allocate a new memory for this texture, consider use _CreateNewTexture variable
	Vector2 _RectSize; //the width and height of the rectangle portion in the image
	Vector2 _RectPosition; //the top left position of portion we want to draw
	Vector2 _ScaleFactor; //scale

	//bool _CreateNewTexture; //if true, allocate a new memory for the texture

public:
	TextureRegion();
	//create a texture from file
	TextureRegion(Texture* texture, /*bool createNew,*/ float rectLeft, float rectTop, float rectWidth, float rectHeight);
	TextureRegion(const TextureRegion &TextureRegion);
	~TextureRegion();

	TextureRegion& operator=(const TextureRegion &TextureRegion);

	//all get functions
	const Vector2& GetRectSize() const;
	const Vector2& GetRectPosition() const;
	Texture* GetTexture() const;
	const Vector2& GetScale() const;

	//all set functions
	void SetRectPosition(float rectX, float rectY);
	void SetRectSize(float rectWidth, float rectHeight);
	void SetTexture(Texture *Texture);
	//Flip the texture. This function is done by multiplying the scale x or y with -1. 
	void Flip(bool flipX, bool flipY);
};

