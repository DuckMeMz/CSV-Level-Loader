#include "Projectile.h"
#include "../Engine/ObjectManager.h"
#include "../src/GameManager.h"
#include "Fire.h"

const double FADEOUT_TIME = 0.5;

Projectile::Projectile(ObjectType objType) : GameObject(objType)
{
	m_width = 0;
	m_height = 0;
	m_speed = 0;
	m_velocity = Vector2D(0,0);
	m_lifeTime = 0.0;
	m_sound = NO_SOUND_INDEX;
	m_channel = -1;
}

void Projectile::Initialise(const char* imageFile, Vector2D position, double angle, double scale)
{
	GameObject::Initialise(imageFile, position, angle, scale);

	SetCollidable();
	SetDrawDepth(10);
	m_width = int(HtGraphics::instance.GetWidthOfPicture(m_images[m_imageNumber]) * m_scale);
	m_height = int(HtGraphics::instance.GetHeightOfPicture(m_images[m_imageNumber]) * m_scale);
	m_sound = HtAudio::instance.LoadSound("..\\Program\\assets\\enemy_projectile.mp3");
	m_channel = HtAudio::instance.Play(m_sound);
	
}

void Projectile::Update(double frameTime)
{
	if (IsActive())
	{
		m_position = m_position + m_velocity * frameTime;
		m_lifeTime -= frameTime;

		if (m_lifeTime <= 0.0)
		{
			Deactivate();
			
		}
		if (m_lifeTime < FADEOUT_TIME)
		{
			m_transparency = 1- (m_lifeTime / FADEOUT_TIME);
		}

		//for the radius of the collision use the largest dimension from width and height
		int radius = ((m_width > m_height) ? m_width : m_height) / 2;
		m_collisionShape.PlaceAt(m_position, radius);
	}
	
}

IShape2D& Projectile::GetCollisionShape()
{
	return m_collisionShape;
}

void Projectile::ProcessCollision(GameObject& other)
{
	double explosionScale = 0;
	bool validCollision = false;
	if (GetType() == ObjectType::PROJECTILE_PLAYER && other.GetType() == ObjectType::PLAYER)
	{
		return;
	}
	if (other.GetType() == ObjectType::COLLISION_TILE || other.GetType() == ObjectType::PLAYER)
	{
		explosionScale = 0.5;
		validCollision = true;
	}
	if (other.GetType() == ObjectType::PROJECTILE_PLAYER)
	{
		explosionScale = 0.8;
		validCollision = true;
	}

	if (other.GetType() == ObjectType::REACTOR && GetType() == ObjectType::PROJECTILE_PLAYER)
	{
		explosionScale = 1.0;
		validCollision = true;
	}

	if (other.GetType() == ObjectType::FIRE && GetType() == ObjectType::PROJECTILE_ENEMY)
	{
		explosionScale = 0.5;
		validCollision = true;
		GameManager::instance.UpdateScore(20);
	}

	if (validCollision)
	{
		Fire* explosion = new Fire(ObjectType::FIRE);
		explosion->Initialise("../Program/assets/ExplosionImages/explosionA1.bmp", m_position, 0.0, explosionScale);
		explosion->SetFrameCount(8);
		explosion->SetAnimationSpeed(15);
		ObjectManager::instance.AddItem(explosion);
		Deactivate();
		
	}

	
}

void Projectile::SetStartVelocity(double speed)
{
	m_velocity.setBearing(m_angle, speed);
}

void Projectile::SetLifeTime(double lifeTime)
{
	m_lifeTime = lifeTime;
}

Projectile::~Projectile()
{
	HtAudio::instance.Stop(m_channel);
}

void Projectile::RenderDebug()
{
	GameObject::RenderDebug();
	GameObject::AddDebugLine("Velocity: ", m_velocity);
	GameObject::AddDebugLine("width: ", m_width);
	GameObject::AddDebugLine("height: ", m_height);
	AddDebugLine("Type:", "PROJECTILE");
}
