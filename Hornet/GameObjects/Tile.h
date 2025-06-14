#pragma once
#include "GameObject.h"
class Tile : public GameObject
{
public:
	Tile(ObjectType objType);
	void Initialise(const char* imagefile, Vector2D position, double angle, double scale) override;
	void SetTileDimensions(int width, int height);
	void SetSourcePosition(Vector2D srcPosition);
		
	void Update(double frametime) override;
	void Render() override;
	IShape2D& GetCollisionShape() override;
	void ProcessCollision(GameObject& other) override;
	void RenderDebug() override;

private:
	Vector2D m_srcCentre;          //This is the location in the tileset that the tile image is drawn from
	Rectangle2D m_collisionShape;
	int m_width;
	int m_height;
};