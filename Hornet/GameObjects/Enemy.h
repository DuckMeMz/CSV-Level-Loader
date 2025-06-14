#pragma once
#include "../Engine/Vector2D.h"
#include "../Engine/HtGraphics.h"
#include "../Engine/HtAudio.h"
#include "GameObject.h"
#include "../Engine/ObjectManager.h"
#include <vector>

/// <summary>
/// The Enemy class: this class is derived from GameObject and represents the enemies in the game.
/// </summary>
class Enemy : public GameObject
{
public:
    /// <summary>
    /// This constructor creates an Enemy using the appropriate object type. See objecttypes.h
    /// Once created use the Initialise(...) method to set the image, position, angle and scale.
	/// Once initialised use the LoadProjectiles(...) method to enable combat.
    /// Once projectiles are loaded set the score value of this enemy using the setScoreValue(...) method.
    /// If required used the FlipHorizontal(...) method to flip the orientaton of the enemy.
    /// Once all the values are set use ObjectManager::instance.AddItem(...) to add an enemy pointer to the game
    /// </summary>
    Enemy(ObjectType objType);

    /// <summary>
    /// The Initilise method should be called after creating the Enemy
    /// </summary>
    /// <param name="imageFile">: The image used for this player: C-style string</param>
    /// <param name="position">: The position of this enemy: Vector2D</param>
    /// <param name="angle">: The angle of the enemy image: double</param>
    /// <param name="scale">: The amount that the image should be scaled by: double</param>
    void Initialise(const char* imageFile, Vector2D position, double angle, double scale) override;

    /// <summary>
    /// This method sets the variable required for the enemy projectiles
    /// </summary>
    /// <param name="projectileLifeTime">: How long the Projectile ramains in flight once fired: double</param>
    /// <param name="projectileOffset">: The (x,y) offset from the centre of the enemy image: Vector2D</param>
    /// <param name="projectileBearing">: The direction the projectile will be fired in: double</param>
    /// <param name="shootDelay">: How long before another shot can be fired. Required to prevent spamming: double</param>
    /// <param name="projectileImage">: The FILE_NAME of the image used to represent the projectile: C++ string</param>
    /// <param name="projectileScale">: The scale of the projectile image: double</param>
    /// <param name="projectileSpeed">: How fast the projectile travels once fired: double</param>
    void LoadProjectiles(double projectileLifeTime, Vector2D projectileOffset, double projectileBearing, 
                    double shootDelay, std::string projectileImage, double projectileScale, double projectileSpeed);
    
    void FlipVertical(bool flip);
    /// <summary>
    /// Flips the horizontal orientation of the enemy.
    /// </summary>
    /// <param name="flip">Flips if set to true: bool</param>
    void FlipHorizontal(bool flip);
    
    /// <summary>
    /// The score value of the enemy should be set when the enemy is created. 
    /// </summary>
    /// <param name="scoreValue">: The score awarded to the player for eliminating this enemy: int</param>
    void SetScoreValue(int scoreValue);

    void Update(double frameTime) override;
    void Render() override;
    void RenderDebug() override;
    IShape2D& GetCollisionShape() override;
    void ProcessCollision(GameObject& other) override;
    
    
 private:
    int m_width;
    int m_height;
    Circle2D m_collisionShape;
    int m_scoreValue;
    bool m_isFlipped_H;
    bool m_isFlipped_V;
    double m_damage;
   

    //projectiles
    double m_projectileLifeTime;
    std::string m_projectileImage;
    double m_projectileScale;
    Vector2D m_projectileOffset;
    double m_shootDelayTimer;
    double m_shootDelay;
    double m_projectileSpeed;
    double m_projectileAngle;
    
    double GenerateRandomDelay();
    void Shoot();
    void Explode();
};