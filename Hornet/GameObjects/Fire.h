#pragma once
#include "GameObject.h"
#include <string>

class Fire : public GameObject
{
public:
	Fire(ObjectType objType);
	void Initialise(const char* imagefile, Vector2D position, double angle, double scale);

	void Update(double frameTime) override;
	void Render() override;
	void ProcessCollision(GameObject& other) override;
	IShape2D& GetCollisionShape() override;

	void SetVelocity(Vector2D velocity);
	void SetFrameCount(int frameCount);
	void SetAnimationSpeed(double animationSpeed);

private:
	Vector2D m_velocity;
	std::string m_baseFileName;
	int m_frameCount;
	double m_animationSpeed; 
	double m_currentFrame;
	Circle2D m_collisionShape;

	void Animate(double frameTime);
};

