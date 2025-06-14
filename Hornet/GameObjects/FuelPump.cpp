#include "FuelPump.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Explosion.h"
#include "../src/GameManager.h"

FuelPump::FuelPump(ObjectType objType) : GameObject(objType)
{
	m_width = 0;
	m_height = 0;
	m_proximityOffset = 300;			//ToDo: replace the magic number
	m_fuelValue = 0;
	m_fuelLevel = 0;
	m_frameCount = 0;
	m_currentFrame = 0;
	m_animationSpeed = 0;
	m_isRefueling = false;
	m_player = nullptr;
	m_baseFileName = "";
}

void FuelPump::Initialise(const char* imagefile, Vector2D position, double angle, double scale)
{
	GameObject::Initialise(imagefile, position, angle, scale);
	//Get the filename with the number and .png
	std::string file = imagefile;
	m_baseFileName = file.substr(0, file.length() - 5);		//ToDo: replace the magic number

	SetDrawDepth(9);										//ToDo: replace the magic number
	SetCollidable();
	SetHandleEvents();
	m_width = static_cast<int>(HtGraphics::instance.GetWidthOfPicture(m_images[m_imageNumber]) * m_scale);
	m_height = static_cast<int>(HtGraphics::instance.GetHeightOfPicture(m_images[m_imageNumber]) * m_scale);
	m_angle = rand() % 360;									//ToDo: replace the magic number
}

void FuelPump::Update(double frametime)
{
	if (IsActive())
	{
		Vector2D bottomLeft = Vector2D((m_position.XValue - m_width / 2.0), (m_position.YValue - m_height / 2.0));
		Vector2D topRight = Vector2D((m_position.XValue + m_width / 2.0), (m_position.YValue + m_height / 2.0));
		m_collisionShape.PlaceAt(bottomLeft, topRight);
		if (m_player != nullptr)
		{
			if ((m_position - m_player->GetPosition()).magnitude() > m_proximityOffset)
			{
				m_isRefueling = false;
			}
		}
		if (m_isRefueling)
		{
			Animate(frametime);
		}
	}
}

IShape2D& FuelPump::GetCollisionShape()
{
	return m_collisionShape;
}

void FuelPump::ProcessCollision(GameObject& other)
{
	if (other.GetType() == ObjectType::PLAYER)
	{
		if (m_player == nullptr)
		{
			m_player = dynamic_cast<Player*>(&other);
		}
		if (other.GetCollisionShape().Intersects(m_collisionShape))
		{
			//Collided with player
			m_player->AddDamage(20);								//ToDo: replace the magic number
			m_player->AddForce(m_player->GetVelocity() * 800);		//ToDo: replace the magic number
			Die();
		}

	}
}

void FuelPump::HandleEvent(Event event)
{
	if (event.type == EventType::REFUEL_REQUEST)
	{
		if (m_player == nullptr)
		{
			m_player = dynamic_cast<Player*>(event.pSource);
		}

		if ((m_position - event.position).magnitude() < m_proximityOffset)
		{
			//The player is in position to fuel
			m_isRefueling = true;
			m_player->AddFuel(0.5);									//ToDo: replace the magic number

		}
	}
}

void FuelPump::Render()
{
	HtGraphics::instance.DrawAt(m_position, m_images[m_imageNumber], m_scale);
	if (m_player != nullptr && m_isRefueling)
	{
		DrawRefuelLink(5);											//ToDo: replace the magic number

	}
}

void FuelPump::RenderDebug()
{
	GameObject::RenderDebug();
	GameObject::AddDebugLine("width: ", m_width);
	GameObject::AddDebugLine("height: ", m_height);
	AddDebugLine("Type:", "FUEL_PUMP");
	HtGraphics::instance.FillRect(m_collisionShape, HtGraphics::instance.LIGHTGREEN);
}

void FuelPump::SetAnimationSpeed(double animationSpeed)
{
	m_animationSpeed = animationSpeed;
}

void FuelPump::SetFuelValue(int value)
{
	m_fuelValue = value;
}

void FuelPump::SetFrameCount(int frameCount)
{
	m_frameCount = frameCount;
	//load the other images
	for (int i = 2; i <= frameCount; i++)
	{
		std::string filename = m_baseFileName + std::to_string(i) + ".png";
		m_images.push_back(HtGraphics::instance.LoadPicture(filename));
	}
}



void FuelPump::Animate(double frameTime)
{
	m_currentFrame = m_currentFrame + m_animationSpeed * frameTime;
	if (m_currentFrame < m_frameCount)
	{
		m_imageNumber = static_cast<int>(m_currentFrame);
	}
	else
	{
		m_imageNumber = m_frameCount - 1;
	}

}

void FuelPump::DrawRefuelLink(int thickness)
{
	Vector2D fuelPoint = m_position + Vector2D(0, m_height / 2.0);
	int i = 0;
	if (m_player != nullptr)
	{
		for (; i < thickness; i++)
		{
			HtGraphics::instance.DrawSegment(m_player->GetPosition() + Vector2D(i, i), fuelPoint + Vector2D(i, 0), HtGraphics::LIGHTGREEN);
			HtGraphics::instance.DrawSegment(m_player->GetPosition() - Vector2D(i, i), fuelPoint - Vector2D(i, 0), HtGraphics::LIGHTGREEN);
		}
		int pipe = thickness + 3;
		for (; i < pipe; i++)
		{
			HtGraphics::instance.DrawSegment(m_player->GetPosition() + Vector2D(i, i), fuelPoint + Vector2D(i, 0), HtGraphics::DARKGREEN);
			HtGraphics::instance.DrawSegment(m_player->GetPosition() - Vector2D(i, i), fuelPoint - Vector2D(i, 0), HtGraphics::DARKGREEN);
		}
	}

}

void FuelPump::Explode(double scale)
{
	Explosion* explosion = new Explosion();
	explosion->Initialise("../Program/assets/ExplosionImages/explosionA1.bmp", m_position, 0.0, scale);
	explosion->SetFrameCount(8);										//ToDo: replace the magic number
	explosion->SetAnimationSpeed(10.0);									//ToDo: replace the magic number
	explosion->SetSound("..\\Program\\assets\\explosion2.wav");
	ObjectManager::instance.AddItem(explosion);
}

void FuelPump::Die()
{
	//ToDo: replace the magic numbers
	Explode(1);
	Explode(5);
	Explode(15);
	Deactivate();

}

FuelPump::~FuelPump()
{
	m_player = nullptr;
}
