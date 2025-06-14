#include "Player.h"
#include "../Engine/HtKeyboard.h"
#include "../Engine/HtCamera.h"
#include "../Engine/ObjectManager.h"
#include "../src/GameManager.h"
#include "../GameObjects/ProgressBar.h"
#include "../GameObjects/Explosion.h"
#include "../Engine/Force.h"
#include "../Engine/Timer.h"
#include "../Engine/Event.h"
#include "Projectile.h"
#include "Fire.h"
#include <algorithm>

Player::Player(ObjectType objType) : GameObject(objType)
{
	m_velocity.set(0.0, 0.0);
	m_fadeInTimer = 0.0;
	m_shootDelayTimer = 0;
	m_thrusterDelay = 0;
	m_thrustSoundChannel = -1;
	m_thrustSound = NO_SOUND_INDEX;

	m_hasObject = false;
	if (m_mass != 0)
	{
		m_inverseMass = 1 / m_mass;
	}
	else
	{
		m_inverseMass = 0.0;
	}
	m_collisionTimer = COLLISIONTIME;
	m_damage = 0.0;
	m_fuel = GameManager::instance.MAX_FUEL;
	m_ammunition = GameManager::instance.MAX_AMMUNITION;
	m_height = 0;
	m_width = 0;

}

void Player::Initialise(const char* imageFile, Vector2D position, double angle, double scale)
{
	GameObject::Initialise(imageFile, position, angle, scale);

	m_width = static_cast<int>(HtGraphics::instance.GetWidthOfPicture(m_images[m_imageNumber]) * m_scale);
	m_height = static_cast<int>(HtGraphics::instance.GetHeightOfPicture(m_images[m_imageNumber]) * m_scale);
	SetDrawDepth(10);
	SetCollidable();
	SetHandleEvents();
	m_previousPosition = position;
	
	m_thrustSound = HtAudio::instance.LoadSound(THRUST_SOUND);
	m_thrustVolume = 0;

	//This is here to make the assignment shorter
	AddComponentObjects();

}

void Player::LoadProjectiles(double projectileLifeTime, Vector2D projectileOffset, double shootDelay, 
							std::string projectileImage, double projectileScale, double projectileSpeed)
{
	m_projectileLifeTime = projectileLifeTime;
	m_projectileOffset = projectileOffset;
	m_shootDelay = shootDelay;
	m_projectileImage = projectileImage;
	m_projectileScale = projectileScale;
	m_projectileSpeed = projectileSpeed;
}

void Player::ProcessInput(double frameTime)
{
	if (m_fuel > 0)
	{
		Vector2D thrust;
		if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_LEFT))
		{
			
			m_angle = m_angle - TURNSPEED * frameTime;
			
		}
		if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_RIGHT))
		{
			m_angle = m_angle + TURNSPEED * frameTime;
			
		}
		if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_UP))
		{
			thrust.setBearing(m_angle, THRUST);
			Thrusters(frameTime, THUST_POWER, Vector2D(0, 0), THRUST_MAGNITUDE, THRUST_MULTIPLIER);
			//reduce the fuel
			m_fuel -= THRUST_FUEL * m_mass;
			if (m_thrustSoundChannel < 0)
			{
				// Minor issue here with pausing
				m_thrustSoundChannel = HtAudio::instance.Play(m_thrustSound, true);
			}
			if (m_thrustVolume < THRUST_VOLUME_THRESHOLD)
			{
				m_thrustVolume += THRUST_VOLUME_STEP * frameTime;
				if (m_thrustVolume > THRUST_VOLUME_THRESHOLD) m_thrustVolume = THRUST_VOLUME_THRESHOLD;
				HtAudio::instance.SetChannelVolume(m_thrustSoundChannel, m_thrustVolume / THRUST_VOLUME_THRESHOLD);
			}
		}
		else if (m_thrustSoundChannel >= 0)
		{
			// Minor issue here with pausing
			if (m_thrustVolume > 0)
			{
				m_thrustVolume -= THRUST_VOLUME_STEP * frameTime;
				HtAudio::instance.SetChannelVolume(m_thrustSoundChannel, m_thrustVolume / THRUST_VOLUME_THRESHOLD);
				if (m_thrustVolume <= 0)
				{
					HtAudio::instance.SetChannelVolume(m_thrustSoundChannel, m_thrustVolume / THRUST_VOLUME_THRESHOLD);
					HtAudio::instance.Stop(m_thrustSoundChannel);
					HtAudio::instance.SetChannelVolume(m_thrustSoundChannel, THRUST_CHANNEL);
					m_thrustVolume = 0;
					m_thrustSoundChannel = -1;
				}
			}
		}
		AddForce(thrust);
	}
	

	if (m_ammunition > 0) {

		if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_SPACE) && m_shootDelayTimer <= 0)
		{
			Shoot(frameTime);
		}

		m_shootDelayTimer -= frameTime;
	}

	if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_R))
	{
		//m_isRefuelingActive = true;
		Event event;
		event.type = EventType::REFUEL_REQUEST;
		event.pSource = this;
		event.position = m_position;
		ObjectManager::instance.HandleEvent(event);
	} 
	
	if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_P))
	{
		Event event;
		event.type = EventType::PICKUP_REQUEST;
		event.pSource = this;
		event.position = m_position;
		ObjectManager::instance.HandleEvent(event);
	}

}

void Player::Shoot(double frameTime)
{
	Vector2D projectileOffset;
	projectileOffset.setBearing(m_angle, m_projectileOffset.magnitude() * m_scale);

	Projectile* p_projectile = new Projectile(ObjectType::PROJECTILE_PLAYER);

	//set the bearing of the projectile using the angle of the ship and the projectile speed + the ship speed
	Vector2D projectileDirection;
	projectileDirection.setBearing(m_angle, m_projectileSpeed + m_velocity.magnitude());

	//Initialise the projectile and add it as a GameObject
	p_projectile->Initialise(m_projectileImage.c_str(), m_position + projectileOffset, m_angle, m_projectileScale);
	p_projectile->SetStartVelocity(m_projectileSpeed);
	p_projectile->SetLifeTime(m_projectileLifeTime);
	ObjectManager::instance.AddItem(p_projectile);

	m_ammunition -= AMMUNITION_PER_SHOT;
	GameManager::instance.SetPlayerAmmunition(static_cast<int>(m_ammunition));

	//Reset the shoot delay timer
	m_shootDelayTimer = m_shootDelay;
}

void Player::Thrusters(double frameTime, double power, Vector2D offset, double magnitude, double multiplier)
{
	m_thrusterDelay -= frameTime;
	if (m_thrusterDelay < 0)
	{
		Vector2D dir ;
		dir.setBearing(m_angle + THRUST_BEARING, magnitude);
		
		Fire* thrusters = new Fire(ObjectType::FIRE);
		thrusters->Initialise(EXPLOSION_IMAGE1.c_str(), m_position + dir * multiplier + offset, 0.0, power);
		thrusters->SetFrameCount(THRUST_FRAME_COUNT); 
		thrusters->SetAnimationSpeed(THRUST_ANIM_SPEED); 
		thrusters->SetVelocity(m_velocity + dir * THRUST_VELOCITY_MULTIPLIER); 
		ObjectManager::instance.AddItem(thrusters);
		m_thrusterDelay = THRUST_DELAY;  
	}
}


void Player::Update(double frameTime)
{

	if (IsActive())
	{
		
		m_fadeInTimer += frameTime;
		if (m_fadeInTimer < FADEIN_TIME)
		{
			m_transparency = 1 - (m_fadeInTimer / FADEIN_TIME);
		}
		else
		{
			m_transparency = 0.0;
		}

		ProcessInput(frameTime);

		//Add a gravity force
		Vector2D weight = Force::GenerateWeightForce(m_mass);
		AddForce(weight);


		//Add a drag force
		Vector2D drag = Force::GenerateDragForce(m_velocity, DRAG_FORCE);
		AddForce(drag);

		m_collisionTimer -= frameTime;
		if (m_collisionTimer <= 0)
		{
			//store the position for collisions
			m_previousPosition = m_position;
			m_collisionTimer = COLLISIONTIME;
		}

		//update the position
		IntegrateForces(frameTime);

		//Move the camera with the player
		Vector2D cameraPosition = Vector2D(m_position.XValue, m_position.YValue - CAMERA_POSITION_OFFSET);
		HtCamera::instance.PlaceAt(m_position);

		//update the position of the collision
		//for the radius of the collision use the largest dimension from width and height
		int radius = ((m_width > m_height) ? m_width : m_height) / 2;
		m_collisionShape.PlaceAt(m_position, radius);
		
		//Update any component objects
		for (GameObject* p_Object : m_componentObjects)
		{
			if (p_Object->GetType() == ObjectType::PROGRESS_BAR)
			{
				ProgressBar* pb = dynamic_cast<ProgressBar*>(p_Object);
				if (pb != nullptr)
				{
					switch (pb->GetType())
					{
					case ProgressBarType::DAMAGE:
						pb->SetProgress(static_cast<int>(m_damage));
						break;
					case ProgressBarType::FUEL:
						pb->SetProgress(static_cast<int>(m_fuel));
						break;
					case ProgressBarType::AMMUNITION:
						pb->SetProgress(static_cast<int>(m_ammunition));
					}
				}
			}
			p_Object->Update(frameTime);
		}

		if (GameManager::instance.GetTimeRemaining() == 0)
		{
			Die();
		}

		if (m_hasObject)
		{
			GameManager::instance.SetPlayerHasObject(true);
		}
	}
}

void Player::Render()
{
	//Call the base class Render function
	GameObject::Render();

	//render any component objects
	for (GameObject* p_Object : m_componentObjects)
	{
		p_Object->Render();
	}
}

void Player::RenderDebug()
{
	GameObject::RenderDebug();
	GameObject::AddDebugLine("Velocity: ", m_velocity);
	GameObject::AddDebugLine("width: ", m_width);
	GameObject::AddDebugLine("height: ", m_height);
	AddDebugLine("Type:", "PLAYER");
}



IShape2D& Player::GetCollisionShape()
{
	return m_collisionShape;
}

void Player::ProcessCollision(GameObject& other)
{
	bool damaged = false;
	switch (other.GetType())
	{
	case ObjectType::COLLISION_TILE: 
	{
		m_position = m_previousPosition;
		Vector2D friction = Force::GenerateFrictionForce(m_velocity, FRICTION);
		AddForce(friction);

		//slow it down a bit
		m_velocity *= COLLISION_SPEED_REDUCTION;
		//add some damage
		m_damage += DAMAGE_TILE;
		//create an explosion
		Explode(TILE_COLLISION_SCALE);

		damaged = true;
		break;
	}
	case ObjectType::PROJECTILE_ENEMY:
	{
		Projectile& b = dynamic_cast<Projectile&>(other);
		Vector2D b_velocity = b.GetVelocity();
		AddForce(b_velocity * HIT_FORCE_PROJECTILE);
		m_damage += DAMAGE_PROJECTILE;
		Explode(PROJECTILE_COLLISION_SCALE);

		damaged = true;
		break;
	}
	case ObjectType::PICK_UP:
		
		break;
	case ObjectType::BOUNDARY_FINISH:
		if (m_hasObject)
		{
			GameManager::instance.SetPlayerIsFinished(true);
			Deactivate();
			if (m_thrustSoundChannel >= 0)
			{
				HtAudio::instance.Stop(m_thrustSoundChannel);
				HtAudio::instance.SetChannelVolume(m_thrustSoundChannel, 1);
			}
		}
		else
		{
			//ToDo: Possibly indicate that the player needs to go down
		}
		break;
	case ObjectType::PROJECTILE_PLAYER:
		break;
	
		

	}

	if (damaged)
	{
		if (m_damage >= GameManager::instance.MAX_DAMAGE)
		{
			Die();
		}
		GameManager::instance.SetPlayerDamage(static_cast<int>(m_damage));
	}
	
}

void Player::AddComponentObject(GameObject* p_object)
{
	if (p_object)
	{
		m_componentObjects.push_back(p_object);
	}
}

void Player::AddForce(const Vector2D& force)
{
	//This adds forces to the spaceship
	m_sumOfForces += force;
}

void Player::AddMass(double mass)
{
	m_mass += mass;
	if (m_mass != 0.0)
	{
		m_inverseMass = 1 / m_mass;
	}
	else
	{
		m_inverseMass = 0.0;
	}
}

void Player::SetHasObject(bool hasObject)
{
	m_hasObject = hasObject;
}

void Player::AddDamage(double damage)
{
	m_damage += damage;
	m_damage = clamp(m_damage, 0.0, MAX_DAMAGE);
}

void Player::AddFuel(double fuel)
{
	m_fuel += fuel;
	m_fuel = clamp(m_fuel, 0.0, MAX_FUEL);
}

Vector2D Player::GetVelocity() const
{
	return m_velocity;
}

void Player::IntegrateForces(double frameTime)
{
	//Force = mass * acceleration therefore acceleration = force / mass (second law of motion)
	m_acceleration = m_sumOfForces * m_inverseMass;

	//Integrate the acceleration to find the new velocity
	m_velocity += m_acceleration * frameTime;

	//Integrate the velocity to find the new position for the ship
	m_position += m_velocity * frameTime;

	//Clear the forces
	m_sumOfForces = Vector2D(0.0, 0.0);

}

void Player::Explode(double scale)
{
	Explosion* explosion = new Explosion();
	explosion->Initialise(EXPLOSION_IMAGE1.c_str(), m_position, 0.0, scale);
	explosion->SetFrameCount(EXPLOSION_FRAME_COUNT);
	explosion->SetAnimationSpeed(EXPLOSION_ANIM_SPEED);
	explosion->SetSound(EXPLOSION_SOUND.c_str());
	ObjectManager::instance.AddItem(explosion);
}

void Player::Die()
{
	for (int i = 1; i < DIE_EXPLODE_COUNT; i++)
	{
		if (i % DIE_EXPLODE_STEP == 0)
		{
			Explode(double(i));
		}
		
	}
	Deactivate();
	if (m_thrustSoundChannel >= 0)
	{
		HtAudio::instance.Stop(m_thrustSoundChannel);
		HtAudio::instance.SetChannelVolume(m_thrustSoundChannel, 1);
	}
	GameManager::instance.SetPlayerKilled(true);
}

void Player::AddComponentObjects()
{
	//These are only done here to reduce the need in the assignment
	//Add a damage progress bar to the player
	ProgressBar* playerDamageBar = new ProgressBar(ObjectType::PROGRESS_BAR);
	playerDamageBar->Initialise(Vector2D(-1200, 950), 200, 30, ProgressBarType::DAMAGE);
	playerDamageBar->SetBackgroundColour(Colour(255, 150, 150, 150));
	playerDamageBar->SetFillColour(Colour(255, 255, 100, 100));
	playerDamageBar->SetBorderColour(Colour(255, 255, 255, 255));
	playerDamageBar->SetRange(0, static_cast<int>(MAX_DAMAGE), 0);
	playerDamageBar->SetLabelImage(DAMAGE_LABEL, DAMAGE_LABEL_SCALE);
	AddComponentObject(playerDamageBar);

	//Add a fuel progress bar to the player
	ProgressBar* playerFuelBar = new ProgressBar(ObjectType::PROGRESS_BAR);
	playerFuelBar->Initialise(Vector2D(-1200, 900), 200, 30, ProgressBarType::FUEL);
	playerFuelBar->SetBackgroundColour(Colour(255, 150, 150, 150));
	playerFuelBar->SetFillColour(Colour(255, 100, 255, 100));
	playerFuelBar->SetBorderColour(Colour(255, 255, 255, 255));
	playerFuelBar->SetRange(0, static_cast<int>(MAX_FUEL), static_cast<int>(MAX_FUEL));
	playerFuelBar->SetLabelImage(FUEL_LABEL, FUEL_LABEL_SCALE);
	AddComponentObject(playerFuelBar);

	//Add an ammunition bar to the player
	ProgressBar* playerAmmoBar = new ProgressBar(ObjectType::PROGRESS_BAR);
	playerAmmoBar->Initialise(Vector2D(-1200, 850), 200, 30, ProgressBarType::AMMUNITION);
	playerAmmoBar->SetBackgroundColour(Colour(255, 150, 150, 150));
	playerAmmoBar->SetFillColour(Colour(255, 175, 275, 100));
	playerAmmoBar->SetBorderColour(Colour(255, 255, 255, 255));
	playerAmmoBar->SetRange(0, static_cast<int>(MAX_AMMO), static_cast<int>(MAX_AMMO));
	playerAmmoBar->SetLabelImage(AMMO_LABEL, AMMO_LABEL_SCALE);
	AddComponentObject(playerAmmoBar);
}


Player::~Player()
{
	for (GameObject* p_Object : m_componentObjects)
	{
		delete p_Object;
	}
	m_componentObjects.clear();
}


double Player::clamp(double value, double min, double max) {
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}
	else {
		return value;
	}
}