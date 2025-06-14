#include "Explosion.h"

Explosion::Explosion() : GameObject(ObjectType::EXPLOSION)
{
    SetCollidable();
    SetDrawDepth(11);
	m_explosionSound = NO_SOUND_INDEX;
}

void Explosion::Initialise(const char* imageFile, Vector2D position, double angle, double scale)
{
	m_position = position;
	m_scale = scale;
	m_velocity = Vector2D(0, 0);
	m_frameCount = 0;
	m_currentFrame = 0;
	m_animationSpeed = 0;
	LoadImage(imageFile);
	//Get the filename with the number and extension e.g. .png
	std::string file = imageFile;
	m_baseImageFileName = file.substr(0, file.length() - 5);

	m_explosionSound = NO_SOUND_INDEX;
}

void Explosion::Update(double frameTime)
{
	m_position = m_position + m_velocity * frameTime;
	Animate(frameTime);

	m_collisionShape.PlaceAt(m_position, 24 * m_scale);
}

void Explosion::Animate(double frameTime)
{
	//On the first frame play the explosion sound
	if (m_currentFrame == 0 && m_explosionSound != NO_SOUND_INDEX)
	{
		m_explosionSoundChannel = HtAudio::instance.Play(m_explosionSound);
		HtAudio::instance.SetChannelVolume(m_explosionSoundChannel, 0.35);
	}

	m_currentFrame = m_currentFrame + m_animationSpeed * frameTime;
	if (m_currentFrame > m_frameCount - 1)
	{
		//Stop the explosion sound and deactivate
		if (m_explosionSoundChannel > -1)
		{
			HtAudio::instance.SetChannelVolume(m_explosionSoundChannel, 1);
			HtAudio::instance.Stop(m_explosionSoundChannel);
			m_explosionSound = NO_SOUND_INDEX;
		}
		Deactivate();
	}
	else
	{
		m_imageNumber = static_cast<int>(m_currentFrame);
	}
}

IShape2D& Explosion::GetCollisionShape()
{
    return m_collisionShape;
}

void Explosion::SetSound(const char* soundFile)
{
	m_explosionSound = HtAudio::instance.LoadSound(soundFile);
}

void Explosion::SetVelocity(Vector2D velocity)
{
	m_velocity = velocity;
}

void Explosion::SetFrameCount(int frameCount)
{
	m_frameCount = frameCount;
	//load the other images
	for (int i = 2; i <= frameCount; i++)
	{
		std::string filename = m_baseImageFileName + std::to_string(i) + ".bmp";
		m_images.push_back(HtGraphics::instance.LoadPicture(filename));
	}
}

void Explosion::SetAnimationSpeed(double animationSpeed)
{
	m_animationSpeed = animationSpeed;
}
