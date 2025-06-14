#include "Tile.h"
#include "../Engine/HtCamera.h"

Tile::Tile(ObjectType objType) : GameObject(objType)
{
	SetDrawDepth(2);
	m_width = 0;
	m_height = 0;
}

void Tile::Initialise(const char* imagefile, Vector2D position, double angle, double scale)
{
	GameObject::Initialise(imagefile, position, angle, scale);

	if (GetType() == ObjectType::COLLISION_TILE)
	{
		SetCollidable();
	}
}

void Tile::SetTileDimensions(int width, int height)
{
	m_width = static_cast<int>(width * m_scale);
	m_height = static_cast<int>(height * m_scale);
	if (IsCollidable())
	{
		Vector2D bottomLeft(m_position.XValue - (m_width / 2), m_position.YValue - (m_width / 2));
		Vector2D topRight(m_position.XValue + (m_height / 2), m_position.YValue + (m_height / 2));
		m_collisionShape.PlaceAt(bottomLeft, topRight);
	}
}

void Tile::SetSourcePosition(Vector2D srcPosition)
{
	//Set the position in the tileset to take the image from
	m_srcCentre = srcPosition;
}

void Tile::Update(double frametime)
{
}

void Tile::Render()
{
	//Only render the tile if it's in the camera view
	Rectangle2D cameraAreaRect = HtCamera::instance.GetCameraArea();
	Vector2D bottomLeft(m_position.XValue - (m_height / 2), m_position.YValue - (m_height / 2));
	Vector2D topRight(m_position.XValue + (m_height / 2), m_position.YValue + (m_height / 2));
	Rectangle2D tileRect = Rectangle2D(bottomLeft, topRight);
	if (tileRect.Intersects(cameraAreaRect))
	{
		HtGraphics::instance.DrawFromTileset(m_images[0], m_srcCentre, m_position, m_width, m_height, m_scale, 0);
	}
}

IShape2D& Tile::GetCollisionShape()
{
	return m_collisionShape;
}

void Tile::ProcessCollision(GameObject& other)
{
}

void Tile::RenderDebug()
{
	if (IsCollidable())
	{
		GameObject::RenderDebug();
		GameObject::AddDebugLine("width: ", m_width);
		GameObject::AddDebugLine("height: ", m_height);
		AddDebugLine("Type:", "COLLISION_TILE");
	}
}
