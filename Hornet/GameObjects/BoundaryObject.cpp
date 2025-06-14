#include "BoundaryObject.h"
#include "Player.h"
#include "../src/GameManager.h"

BoundaryObject::BoundaryObject(ObjectType objType): GameObject(objType)
{
	SetCollidable();
}

void BoundaryObject::Initialise(Vector2D position, int width, int height)
{
	m_width = width;
	m_height = height;
	m_position = position;
	m_bottomLeft = Vector2D(position.XValue - width, position.YValue - height);
	m_topRight = Vector2D(position.XValue + width, position.YValue + height);;
	m_boundary = Rectangle2D(m_bottomLeft, m_topRight);
}


void BoundaryObject::Update(double framrate)
{
	if (IsActive())
	{
		m_boundary.PlaceAt(m_bottomLeft, m_topRight);
	}
}

void BoundaryObject::Render()
{
	//we do nothing here as the boundaries aren't rendered
}

IShape2D& BoundaryObject::GetCollisionShape()
{
	return m_boundary;
}

void BoundaryObject::ProcessCollision(GameObject& other)
{
	if (other.GetType() == ObjectType::PLAYER)
	{
		if (GetType() == ObjectType::BOUNDARY)
		{
			Player* player = dynamic_cast<Player*>(&other);
			Vector2D playerVelocity = player->GetVelocity();
			player->AddForce(-playerVelocity * 400);
		}
		
	}
	
}

void BoundaryObject::RenderDebug()
{
	GameObject::RenderDebug();
	GameObject::AddDebugLine("width: ", m_width);
	GameObject::AddDebugLine("height: ", m_height);
	AddDebugLine("Type:", "BOUNDARY");
}


