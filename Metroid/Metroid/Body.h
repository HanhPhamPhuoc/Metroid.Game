#pragma once


#include "stdafx.h"

class Body
{
private:
	Vector2 _Position;
	Vector2 _Size;
	Vector2 _Velocity;
	Vector2 _LinearForce;

	float _Mass; //the mass of body
	

public:
	Body();
	Body(float x, float y, float width, float height, float vx, float vy);
	~Body();

	void SetVelocity(float vx, float vy);
	void SetSize(float width, float height);
	void SetPosition(float x, float y);
	void SetMass(float mass);
	void ApplyLinearForce(float xForce, float yForce);

	Vector2 GetVelocity();
	Vector2 GetSize();
	Vector2 GetPosition();
	Vector2 GetTotalVelocity();
	float GetMass();

	//Go to the next position
	//this function was done by multiplying its velocity and deltatime 
	void Next(float dt);

};

