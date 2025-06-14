#pragma once
#include "GameObject.h"
#include "Player.h"

/// <summary>
/// The FuelPump class: this class is derived from GameObject and represents the fuel pumps in the game.
/// </summary>
class FuelPump : public GameObject
{
public:
	/// <summary>
	/// This constructor creates a FuelPump using the appropriate object type. See objecttypes.h
	/// Once created use the Initialise(...) method to set the image, position, angle and scale.
	/// Once Initialised use the setFuelValue(...) method to set the value of the fuel.
	/// If the fuel pump is animated use the setFrameCount(...) and setAnimationSpeed(...) to setup the animation.
	/// Once all the values are set use ObjectManager::instance.AddItem(...) to add the fuel pump pointer to the game
	/// </summary>
	/// <param name="objType"></param>
	FuelPump(ObjectType objType);

	/// <summary>
	/// The Initilise method should be called after creating the Fuel pump
	/// </summary>
	/// <param name="imagefile">: The image used for this pump: C-style string</param>
	/// <param name="position">: The position of this pump: Vector2D</param>
	/// <param name="angle">The angle of the pump image: double</param>
	/// <param name="scale">The amount that the image should be scaled by: double</param>
	void Initialise(const char* imagefile, Vector2D position, double angle, double scale) override;

	~FuelPump();
	virtual void Update(double frametime) override;
	virtual IShape2D& GetCollisionShape() override;
	void ProcessCollision(GameObject& other) override;
	void HandleEvent(Event event) override;
	void Render() override;
	void RenderDebug() override;

	/// <summary>
	/// The value of the fuel should be set when the fuel pump is created. 
	/// </summary>
	/// <param name="value">The value: int</param>
	void SetFuelValue(int value);

	/// <summary>
	/// Set the frame count for the refuel animation
	/// </summary>
	/// <param name="frameCount">The number of images in the animation: int</param>
	void SetFrameCount(int frameCount);

	/// <summary>
	/// Set the speed of the animation.
	/// </summary>
	/// <param name="animationSpeed">The animation speed w.r.t frame rate, higher is faster: double</param>
	void SetAnimationSpeed(double animationSpeed);



private:
	int m_width;
	int m_height;
	std::string m_baseFileName;
	Rectangle2D m_collisionShape;
	double m_proximityOffset;
	double m_animationSpeed;
	int m_fuelValue;
	int m_fuelLevel;
	int m_frameCount;
	double m_currentFrame;
	bool m_isRefueling;
	Player* m_player;

	void Animate(double frameTime);
	void DrawRefuelLink(int thickness);
	void Explode(double scale);
	void Die();
};

