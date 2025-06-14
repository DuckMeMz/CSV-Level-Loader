#pragma once
#include "GameObject.h"
#include "../Engine/Shapes.h"

/// <summary>
/// The BoundaryObject Class: this class is derived from GameObject and represents invisible boundaries in the game.
/// There are two boundary types BOUNDARY and BOUNDARY_FINISH, see objecttypes.h. 
/// BOUNDARY is used to keep the player in the designated play area, 
/// whereas BOUNDARY_FINISH represents the target level complete area.
/// </summary>
class BoundaryObject : public GameObject
{
public:
	/// <summary>
	/// This constructor initialises the BoundaryObject with an object type. See objecttypes.h
	/// Once created use the Initialise(...) method to set the position, width and height.
	/// Once all the values are set use ObjectManager::instance.AddItem(...) to add the boundary pointer to the game
	/// </summary>
	/// <param name="objType">The type of the oject: ObjectType</param>
	BoundaryObject(ObjectType objType);

	/// <summary>
	/// The initialise method should be called after creating the boundary object
	/// </summary>
	/// <param name="position">: The position of the boundary: Vector2D</param>
	/// <param name="width">: The width of the boundary: int</param>
	/// <param name="height">: The height of the boundary: int</param>
	void Initialise(Vector2D position, int width, int height);

	void Update(double framrate) override;
	void Render() override;
	void RenderDebug() override;
	IShape2D& GetCollisionShape() override;
	void ProcessCollision(GameObject& other) override;
	

private:
	int m_width;
	int m_height;
	Vector2D m_bottomLeft;
	Vector2D m_topRight;
	Rectangle2D m_boundary;
};

