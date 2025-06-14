#pragma once
#include "GameObject.h"
#include "../Engine/HtAudio.h"
#include <list>

/// <summary>
/// The Player class: this class is derived from GameObject and represents the player in the game.
/// </summary>
class Player : public GameObject
{
public:
	/// <summary>
	/// This constructor creates the Player using the appropriate object type. See objecttypes.h
	/// Once created use the Initialise(...) method to set the image, position and scale.
	/// Once initialised use the LoadProjectiles(...) method to enable combat.
	/// Once all the values are set use ObjectManager::instance.AddItem(...) to add a player pointer to the game
	/// </summary>
	Player(ObjectType objType);

	/// <summary>
	/// The Initilise method should be called after creating the player
	/// </summary>
	/// <param name="imageFile">: The image used for the player, loaded using the Load method: string</param>
	/// <param name="position">:The starting position of the player: Vector2D</param>
	/// <param name="angle">: The starting angle of the player: double</param>
	/// <param name="scale">: The amount that the image should be scaled by: double</param>
	void Initialise(const char* imageFile, Vector2D position, double angle = 0, double scale = 1.0) override;

	/// <summary>
	/// This method sets the variable required for the player projectiles
	/// </summary>
	/// <param name="projectileLifeTime">: How long the Projectile ramains in flight once fired: double</param>
	/// <param name="projectileOffset">: The (x,y) offset from the centre of the player image: Vector2D</param>
	/// <param name="shootDelay">: How long before another shot can be fired. Required to prevent spamming: double</param>
	/// <param name="projectileImage">: The projectile image file: string</param>
	/// <param name="projectileScale">: The scale of the projectile image: double</param>
	/// <param name="projectileSpeed">: How fast the projectile travels once fired: double</param>
	void LoadProjectiles(double projectileLifeTime, Vector2D projectileOffset, double shootDelay,
					std::string projectileImage, double projectileScale, double projectileSpeed);

	void Update(double frameTime) override;
	void Render() override;
	void RenderDebug() override;
	IShape2D& GetCollisionShape() override;
	void ProcessCollision(GameObject& other) override;

	Vector2D GetVelocity() const;

	void AddComponentObject(GameObject* p_object);
	void AddForce(const Vector2D& force);
	void AddMass(double mass);

	void SetHasObject(bool hasObject);
	void AddDamage(double damage);
	void AddFuel(double fuel);

	~Player() override;
private:
	int m_width;
	int m_height;
	Circle2D m_collisionShape;
	Vector2D m_velocity;
	Vector2D m_acceleration;
	double m_fadeInTimer;
	double m_collisionTimer;
	double m_thrusterDelay;
	
	Vector2D m_previousPosition;
	double m_inverseMass;
	Vector2D m_sumOfForces;
	
	//Component objects
	std::list<GameObject*> m_componentObjects;
	
	//projectiles
	double m_projectileLifeTime;
	std::string m_projectileImage;
	double m_projectileScale;
	Vector2D m_projectileOffset;
	double m_shootDelayTimer;
	double m_shootDelay;
	double m_projectileSpeed;

	//GamePlayStats
	double m_damage;
	double m_fuel;
	double m_ammunition;
	bool m_hasObject;

	//Sounds
	SoundIndex m_thrustSound;
	int m_thrustSoundChannel;
	double m_thrustVolume;
	
	void Shoot(double frameTime);
	void Thrusters(double frameTime, double power, Vector2D offset, double magnitude, double multiplier);
	void Explode(double scale);

	void ProcessInput(double frameTime);
	void IntegrateForces(double frameTime);
	void Die();

	//This is moved here to make the assignment shorter
	//These can be added from a config entry
	void AddComponentObjects();

	//A method to clamp a double value within a specified range
	double clamp(double value, double min, double max);

	//Player constants there's only one player so this is fine
	const double TURNSPEED = 110.0;
	const double FRICTION = 0.2;
	const double DRAG_FORCE = 0.002;
	const int THRUST_FRAME_COUNT = 8;
	const int THRUST_ANIM_SPEED = 20;
	const double THRUST_VELOCITY_MULTIPLIER = 150;
	const double THRUST_DELAY = 0.05;
	const double THRUST = 1000.0;
	const double THUST_POWER = 0.6;
	const double THRUST_MAGNITUDE = 1.9;
	const double THRUST_MULTIPLIER = 34;
	const std::string THRUST_SOUND = "..\\Program\\assets\\thrustloop.wav";
	const double THRUST_VOLUME_THRESHOLD = 100;
	const double THRUST_VOLUME_STEP = 300;
	const int THRUST_CHANNEL = 1;
	const double THRUST_BEARING = 180;
	const double THRUST_FUEL = 0.02;
	const double TURN_THRUST_FUEL = 0.01;
	const double FADEIN_TIME = 2.0;
	const double COLLISIONTIME = 0.05;
	const double DAMAGE_TILE = 10.0;
	const double DAMAGE_PROJECTILE = 10.0;
	const int AMMUNITION_PER_SHOT = 5;
	const double HIT_FORCE_PROJECTILE = 50.0;
	const double CAMERA_POSITION_OFFSET = 200.0;
	const double COLLISION_SPEED_REDUCTION = -0.7;
	const double TILE_COLLISION_SCALE = 2.0;
	const double PROJECTILE_COLLISION_SCALE = 1.0;
	const double MAX_FUEL = 100;
	const double MAX_DAMAGE = 100;
	const double MAX_AMMO = 100;
	const int DIE_EXPLODE_COUNT = 15;
	const int DIE_EXPLODE_STEP = 5;
	const std::string EXPLOSION_IMAGE1 = "../Program/assets/ExplosionImages/explosionA1.bmp";
	const int EXPLOSION_FRAME_COUNT = 8;
	const int EXPLOSION_ANIM_SPEED = 20;
	const std::string EXPLOSION_SOUND = "..\\Program\\assets\\explosion2.wav";
	const std::string DAMAGE_LABEL = "../Program/assets/DamageLabel.png";
	const double DAMAGE_LABEL_SCALE = 0.4;
	const std::string FUEL_LABEL = "../Program/assets/FuelLabel.png";
	const double FUEL_LABEL_SCALE = 0.4;
	const std::string AMMO_LABEL = "../Program/assets/AmmoLabel.png";
	const double AMMO_LABEL_SCALE = 0.4;
};

