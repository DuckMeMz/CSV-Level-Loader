#include "PickUp.h"
#include "../src/GameManager.h"
#include "Player.h"
#include "../Engine/Force.h"
#include <iostream>
#include "PickUp.h"

const double FADEOUT_TIME = 2.0;

PickUp::PickUp(ObjectType objType) : GameObject(objType)
{
	m_proximityOffset = 300;
	m_scale = 0;
	m_width = 0;
	m_height = 0;
	m_angle = 0;
	m_scoreValue = 100;
	m_isInPosition = false;
	m_player = nullptr;
	m_fadeOutTimer = FADEOUT_TIME;
	m_pickupSound = HtAudio::instance.LoadSound("..\\Program\\assets\\pickup.mp3");
}

void PickUp::Initialise(const char* imagefile, Vector2D position, double angle, double scale)
{
	GameObject::Initialise(imagefile, position, angle, scale);
	SetDrawDepth(9);
	SetCollidable();
	SetHandleEvents();
	m_width = static_cast<int>(HtGraphics::instance.GetWidthOfPicture(m_images[m_imageNumber]) * scale);
	m_height = static_cast<int>(HtGraphics::instance.GetHeightOfPicture(m_images[m_imageNumber]) * scale);
}

void PickUp::SetValue(int value)
{
	m_scoreValue = value;
}

PickUp::~PickUp()
{

}

void PickUp::Update(double frametime)
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
				m_isInPosition = false;
			}
		}
		
		if (m_isInPosition)
		{
			m_fadeOutTimer -= frametime;
			if (m_fadeOutTimer > 0)
			{
				m_transparency = 1 - ( m_fadeOutTimer / FADEOUT_TIME);
			}
			
			if (m_transparency >= 0.9)
			{
				HtAudio::instance.Play(m_pickupSound);
				m_player->AddMass(1);
				m_player->SetHasObject(true);
				Deactivate();
			}
			
		}
		
	}
}

void PickUp::ProcessCollision(GameObject& other)
{
	if (other.GetType() == ObjectType::PROJECTILE_PLAYER)
	{
		//ToDo: Explode, and show Game Over it's your own fault
	}
}

void PickUp::HandleEvent(Event event)
{
	if (event.type == EventType::PICKUP_REQUEST)
	{
		if (m_player == nullptr)
		{
			m_player = dynamic_cast<Player*>(event.pSource);
		}

		if ((m_position - event.position).magnitude() < m_proximityOffset)
		{
			//The player has requested pickup and is in position
			m_isInPosition = true;
		}

		
	}

}

void PickUp::DrawPickupLink(int thickness)
{
	if (m_player != nullptr)
	{
		Vector2D pickupBase = m_position + Vector2D(0, -(m_height / 2.0));
		int i = 0;
		for (; i < thickness; i = i + 5)
		{
			HtGraphics::instance.DrawSegment(m_player->GetPosition(), pickupBase + Vector2D(i, 0), HtGraphics::YELLOW);
			HtGraphics::instance.DrawSegment(m_player->GetPosition(), pickupBase - Vector2D(i, 0), HtGraphics::YELLOW);
		}
		int pipe = thickness + 10;
		for (; i < pipe; i++)
		{
			HtGraphics::instance.DrawSegment(m_player->GetPosition(), pickupBase + Vector2D(i, 0), HtGraphics::DARKGREEN);
			HtGraphics::instance.DrawSegment(m_player->GetPosition(), pickupBase - Vector2D(i, 0), HtGraphics::DARKGREEN);
		}
	}
}

void PickUp::Render()
{
	HtGraphics::instance.DrawAt(m_position, m_images[m_imageNumber], m_scale, m_angle, m_transparency);
	if (m_player != nullptr && m_isInPosition)
	{
		DrawPickupLink(m_width/2);
	}
}

IShape2D& PickUp::GetCollisionShape()
{
	return m_collisionShape;
}

void PickUp::RenderDebug()
{
	GameObject::RenderDebug();
	GameObject::AddDebugLine("width: ", m_width);
	GameObject::AddDebugLine("height: ", m_height);
	AddDebugLine("Type:", "PICK_UP");
	HtGraphics::instance.FillRect(m_collisionShape, HtGraphics::instance.LIGHTGREEN);
}




