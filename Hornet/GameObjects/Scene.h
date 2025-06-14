#pragma once
#include "GameObject.h"

/// <summary>
/// The Scene class: this class is derived from GameObject and represents a background scene in the game.
/// </summary>
class Scene : public GameObject
{
public:
	/// <summary>
	/// This constructor creates a scene using the appropriate object type. See objecttypes.h
	/// Once created use the Initialise(...) method to set the image, position, angle and scale.
	/// Once all the values are set use ObjectManager::instance.AddItem(...) to add a Scene pointer to the game
	/// </summary>
	Scene(ObjectType objType);

	/// <summary>
	/// The Initilise method should be called after creating the Scene
	/// </summary>
	/// <param name="imageFile">: The image used for this scene: C-style string</param>
	/// <param name="position">: The position of this scene: Vector2D</param>
	/// <param name="angle">: The angle of the Scene image: double</param>
	/// <param name="scale">: The amount that the image should be scaled by: double</param>
	void Initialise(const char* imagefile, Vector2D position, double angle, double scale) override;

	virtual void Update(double frametime);
	virtual IShape2D& GetCollisionShape();
private:
	Rectangle2D m_collision;
	int m_width;
	int m_height;
};

