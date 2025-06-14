#pragma once
#include "Vector2D.h"
#include "../GameObjects/GameObject.h"

const double PIXELS_PER_METER = 30;

struct Force
{
	static Vector2D GenerateFrictionForce(const Vector2D velocity, const double K);
	static Vector2D GenerateDragForce(const Vector2D velocity, const double K);
	static Vector2D GenerateWeightForce(double mass);
	static Vector2D GenerateGravitationalForce(const GameObject& a, const GameObject& b, double G);
	static Vector2D GenerateSpringForce(const Vector2D position, const Vector2D anchorPosition, const double restLength, const double K);

};

