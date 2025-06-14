#pragma once
#include "GameObject.h"
#include "../Engine/HtAudio.h"
#include <string>
class Explosion : public GameObject
{
public:
	Explosion();
	void Initialise(const char* imageFile, Vector2D position, double angle, double scale);
	void Update(double frameTime)override;
	void Animate(double frameTime);
	IShape2D& GetCollisionShape()override;
	
	void SetSound(const char* soundFile);
	void SetVelocity(Vector2D velocity);
	void SetFrameCount(int frameCount);
	void SetAnimationSpeed(double animationSpeed);

private:
	Vector2D m_velocity;
	int m_frameCount;
	std::string m_baseImageFileName;
	double m_animationSpeed;
	double m_currentFrame;
	Circle2D m_collisionShape;
	SoundIndex m_explosionSound;
	int m_explosionSoundChannel;
};

