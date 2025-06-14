#include "Fire.h"

Fire::Fire(ObjectType objType) : GameObject(objType)
{
	SetCollidable();
	SetDrawDepth(11);
}

void Fire::Initialise(const char* imagefile, Vector2D position, double angle, double scale)
{
	m_position = position;
	m_scale = scale;
	m_velocity = Vector2D(0, 0);
	m_frameCount = 0;
	m_currentFrame = 0;
	m_animationSpeed = 0;
	LoadImage(imagefile);
	//Get the filename with the number and .png
	std::string file = imagefile;
	m_baseFileName = file.substr(0, file.length() - 5);
}

void Fire::Update(double frameTime)
{
	m_position = m_position + m_velocity * frameTime;
	Animate(frameTime);

	m_collisionShape.PlaceAt(m_position, 28);
}

void Fire::Animate(double frameTime)
{
	m_currentFrame = m_currentFrame + m_animationSpeed * frameTime;
	if (m_currentFrame > m_frameCount-1)
	{
		Deactivate();

	}
	else
	{
		m_imageNumber = static_cast<int>(m_currentFrame);
	}
	
}

void Fire::Render()
{
	if (IsActive())
	{
		HtGraphics::instance.DrawAt(m_position, m_images[m_imageNumber], m_scale);
	}
}

void Fire::ProcessCollision(GameObject& other)
{
	bool hasCollided = other.GetType() == ObjectType::COLLISION_TILE || other.GetType() == ObjectType::FUEL_PUMP;
	if (hasCollided)
	{
		m_velocity = m_velocity * -1;
		m_animationSpeed = 40;
	}
}

IShape2D& Fire::GetCollisionShape()
{
	return m_collisionShape;
}

void Fire::SetVelocity(Vector2D velocity)
{
	m_velocity = velocity;
}

void Fire::SetFrameCount(int frameCount)
{
	m_frameCount = frameCount;
	//load the other images
	for (int i = 2; i <= frameCount; i++)
	{
		std::string filename = m_baseFileName + std::to_string(i) + ".bmp";
		m_images.push_back(HtGraphics::instance.LoadPicture(filename));
	}
}

void Fire::SetAnimationSpeed(double animationSpeed)
{
	m_animationSpeed = animationSpeed;
}


