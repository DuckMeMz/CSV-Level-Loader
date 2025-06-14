#pragma once
#include "GameObject.h"
#include "Player.h"

/// <summary>
/// The PickUp class: this class is derived from GameObject and represents the pick ups in the game.
/// </summary>
class PickUp : public GameObject
{
public:
	/// <summary>
	/// This constructor creates a pick up using the appropriate object type. See objecttypes.h
    /// Once created use the Initialise(...) method to set the image, position, angle and scale.
	/// Once all the values are set use ObjectManager::instance.AddItem(...) to add the pick up pointer to the game
	/// </summary>
	/// <param name="objType"></param>
	PickUp(ObjectType objType);

	/// <summary>
	/// The Initilise method should be called after creating the PickUp
	/// </summary>
	/// <param name="imagefile">: The image used for this pick up: C-style string</param>
    /// <param name="position">: The position of this pick up: Vector2D</param>
    /// <param name="angle">: The angle of the pick up image: double</param>
    /// <param name="scale">: The amount that the image should be scaled by: double</param>
	void Initialise(const char* imagefile, Vector2D position, double angle, double scale) override;
	
	virtual void Update(double frametime) override;
	virtual void ProcessCollision(GameObject& other);
	void HandleEvent(Event event) override;
	
	virtual IShape2D& GetCollisionShape() override;
	void Render() override;
	void RenderDebug() override;
	void SetValue(int value);

	~PickUp();

private:
	int m_width;
	int m_height;
	SoundIndex m_pickupSound;
	int m_scoreValue;
	
	
	Rectangle2D m_collisionShape;
	double m_proximityOffset;
	bool m_isInPosition;
	double m_fadeOutTimer;
	Player* m_player;

	void DrawPickupLink(int thickness);
};

