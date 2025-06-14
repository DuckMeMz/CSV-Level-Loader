#include "../src/GameManager.h"
#include "../Engine/HtCamera.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Explosion.h"
#include <iostream>
#include <random>

Enemy::Enemy(ObjectType objType) : GameObject(objType)
{
	m_isFlipped_H = false;
	m_isFlipped_V = false;
	m_scoreValue = 0;	
	m_width = 0;
	m_height = 0;
	m_damage = 0;
}

void Enemy::Initialise(const char* imageFile, Vector2D position, double angle, double scale)
{
	GameObject::Initialise(imageFile, position, angle, scale);
	SetDrawDepth(9);
	SetCollidable();
	SetHandleEvents();
	m_width = static_cast<int>(HtGraphics::instance.GetWidthOfPicture(m_images[m_imageNumber]) * m_scale);
	m_height = static_cast<int>(HtGraphics::instance.GetHeightOfPicture(m_images[m_imageNumber]) * m_scale);
}

void Enemy::LoadProjectiles(double projectileLifeTime, Vector2D projectileOffset, double projectileBearing,
						double shootDelay, std::string projectileImage, double projectileScale, double projectileSpeed)
{
	m_projectileLifeTime = projectileLifeTime;
	m_projectileOffset = projectileOffset;
	m_projectileAngle = projectileBearing;
	m_shootDelay = shootDelay;
	m_projectileImage = projectileImage;
	m_projectileScale = projectileScale;
	m_projectileSpeed = projectileSpeed;
	m_shootDelayTimer = 0;
}

void Enemy::FlipVertical(bool flip)
{
 	m_isFlipped_V = flip;
}

void Enemy::FlipHorizontal(bool flip)
{
	m_isFlipped_H = flip;
}

void Enemy::Update(double frameTime)
{
	if (IsActive())
	{
		m_collisionShape.PlaceAt(m_position, m_width / 2.0);

		m_shootDelayTimer -= frameTime;
		
		Rectangle2D cameraAreaRect = HtCamera::instance.GetCameraArea();
		if (m_shootDelayTimer <= 0 && m_collisionShape.Intersects(cameraAreaRect))
		{
			Shoot();
			m_shootDelayTimer = GenerateRandomDelay();
		}

		if (m_damage == 100)
		{
			Explode();
		}
	}
}

void Enemy::Render()
{
	if (m_imageNumber >= 0 && m_imageNumber<int(m_images.size()))
	{
		HtGraphics::instance.DrawAt(m_position, m_images[m_imageNumber], m_scale, m_angle, m_transparency, m_isFlipped_H);
	}
	else
	{
		HtGraphics::instance.WriteTextCentered(m_position, "No image", HtGraphics::instance.LIGHTGREEN, 0);
	}
}

void Enemy::ProcessCollision(GameObject& other)
{
	if (other.GetType() == ObjectType::PROJECTILE_PLAYER)
	{
		
		m_damage = 100;
	}
	if (other.GetType() == ObjectType::FIRE)
	{
		m_damage = m_damage + 2;
	}
}

void Enemy::SetScoreValue(int scoreValue)
{
	m_scoreValue = scoreValue;
}


void Enemy::RenderDebug()
{
	GameObject::RenderDebug();
	GameObject::AddDebugLine("width: ", m_width);
	GameObject::AddDebugLine("height: ", m_height);
	AddDebugLine("Type:", "ENEMY");
}

IShape2D& Enemy::GetCollisionShape()
{
	return m_collisionShape;
}


double Enemy::GenerateRandomDelay()
{
	// Create a random device to seed the random number generator
	std::random_device rd;
	// Initialise a Mersenne Twister random number generator
	std::mt19937 gen(rd());
	// Define a uniform real distribution between m_shootDelay/2 and m_shootDelay
	std::uniform_real_distribution<double> dis(m_shootDelay/2, m_shootDelay);

	// Generate a random double between 0 and 2.0
	return dis(gen);
}

void Enemy::Shoot()
{
	Projectile* p_projectile = new Projectile(ObjectType::PROJECTILE_ENEMY);
	
	//set the bearing of the projectile using the BEARING
	Vector2D projectileDirection;
	projectileDirection.setBearing(m_projectileAngle, m_projectileSpeed);

	//Initialise the projectile and add it as a GameObject
	p_projectile->Initialise(m_projectileImage.c_str(), m_position + m_projectileOffset, m_projectileAngle, m_projectileScale);
	p_projectile->SetStartVelocity(m_projectileSpeed);
	p_projectile->SetLifeTime(m_projectileLifeTime);
	ObjectManager::instance.AddItem(p_projectile);

	//Reset the shoot delay timer
	m_shootDelayTimer = m_shootDelay;
}

void Enemy::Explode()
{
	Explosion* explosion = new Explosion();
	explosion->Initialise("..\\Program\\assets\\ExplosionImages\\explosionA1.bmp", m_position, 0.0, 5.0);
	explosion->SetFrameCount(8);
	explosion->SetAnimationSpeed(8.0);
	explosion->SetSound("..\\Program\\assets\\explosion1.wav");
	ObjectManager::instance.AddItem(explosion);

	GameManager::instance.UpdateScore(m_scoreValue);
	Deactivate();
}


