#pragma once
#include"Vector3.h"
#include"Vector4.h"
#include"common.h"
#include"Mesh.h"
#include"HeightMap.h"
#include<Cmath>
class ParticleSystem;
class HeightMap;

class Particles
{

public:
	Mesh* shape;
	Particles();
	Particles(Vector3 position, Vector3 velocity, Vector4 colour, float lifeLength, Vector3 scale);
	Particles(Vector3 position, Vector3 velocity, Vector4 colour, float lifeLength);
	~Particles() {}
	void Update(float msec);
	friend class ParticleSystem;
	void Draw();
	Vector3 GetScale() {
		return scale;

	}
	Vector3 Getposition() {
		return position;
	}

	Vector3 GetVelocity() {
		return velocity;
	}
	Vector4 GetColour() {
		return colour;
	}
protected:
	Vector3 originalPosition;
	Vector3 position;
	Vector3 velocity;
	Vector4 colour;
	float length;
	Vector3 scale;
	float durationtime;
	void SetPosition(Vector3 position)
	{
		this->position = position;
	}
	void SetVelocity(Vector3 velocity)
	{
		this->velocity = velocity;

	}

	void SetColour(Vector4 colour)
	{
		this->colour = colour;

	}

	void Setlength(float length)
	{
		this->length = length;
	}
	void SetScale(Vector3 scale)
	{
		this->scale = scale;
	}
	void SetOrginalPosition(Vector3 position)
	{
		this->originalPosition = position;
	}




};