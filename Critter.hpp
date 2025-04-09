/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Critter.hpp - Enemy entity class definition
 * 
 * Purpose: Defines the enemy units that traverse the map. Handles movement along
 * waypoints, health/damage system, and reward mechanics when defeated.
 * 
 * Dependencies: SFML 3.0 (Graphics, System), vector, string
 */
/**
 * @file Critter.hpp
 * @brief Implementation of Critter header file
 */

#ifndef CRITTER_HPP
#define CRITTER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Critter {
public:
    // Creates a critter with specified path, texture, stats and reward value
    Critter(const std::vector<sf::Vector2f>& waypoints,
            const std::string& texturePath,  
            float moveSpeed, 
            float hp, 
            int rewardPoints);  
    // Updates critter position and state based on time passed
    void update(float deltaTime);
    // Returns the sprite for rendering
    const sf::Sprite& getSprite() const;
    // Returns true if critter has reached the end of path
    bool hasReachedEnd() const;
     // Applies damage to the critter. Special damage gets unique visual effect
    void takeDamage(float damage, bool isSpecialDamage = false);  // Updated with new parameter
    // Returns true if health <= 0
    bool isDead() const;
    // Returns gold value when killed 
    int getReward() const;
    // Returns texture path for critter type identification
    std::string getTexturePath() const;

    // Returns the current waypoint index the critter is moving towards
    int getCurrentWaypoint() const { return currentTarget; }

    // Returns the current health of the critter
    float getHealth() const { return health; }

    // Returns speed info for critter inspect --
    float getSpeed() const { return speed; }

    // Draw health bar above the critter
    void drawHealthBar(sf::RenderWindow& window) const;

    // Get maximum health for this critter type
    float getMaxHealth() const;

private:
    std::vector<sf::Vector2f> path;
    int currentTarget;
    float speed;
    float health;
    int reward;
    sf::Texture texture;
    sf::Sprite sprite;
    bool reachedEnd;
    bool dead;
    std::string texturePath;
    
    // Flash effect variables
    float flashTimer = 0.0f;
    const float FLASH_DURATION = 0.2f; // Flash lasts 0.2 seconds
};

#endif // CRITTER_HPP