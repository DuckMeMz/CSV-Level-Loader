#pragma once
#include "GameObject.h"
#include "../Engine/HtAudio.h"

/// <summary>
/// The Projectile Class: this class is derived from GameObject and represents a prjectile in the game.
/// </summary>
class Projectile : public GameObject
{
public:
	/// <summary>
	/// The constructor initialises the Projectile with an object type. See objecttypes.h
	/// </summary>
	/// <param name="objType"></param>
	Projectile(ObjectType objType);

	/// <summary>
	/// The Initilise method should be called after creating the projectile
	/// </summary>
	/// <param name="image">The image used for the projectile</param>
	/// <param name="position">Starting Position</param>
	/// <param name="startVelocity">The angle of the projectile</param>
	/// <param name="scale">The scale of the projectile image</param>
	void Initialise(const char* imageFile, Vector2D position, double angle, double scale) override;

	void Update(double frameTime) override;
	void RenderDebug() override;
	IShape2D& GetCollisionShape() override;
	void ProcessCollision(GameObject& other) override;

	void SetStartVelocity(double speed);
	void SetLifeTime(double lifeTime);

	Vector2D GetVelocity() const { return m_velocity; }

	~Projectile();

private:
	Circle2D m_collisionShape;
	double m_speed;
	Vector2D m_velocity;
	double m_lifeTime;
	int m_width;
	int m_height;

	SoundIndex m_sound;
	int m_channel;
};

