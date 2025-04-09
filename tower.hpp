/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Tower.hpp - Defense tower class definition
 * 
 * Purpose: Defines tower types and their behaviors including targeting,
 * shooting mechanics, and upgrade system.
 * 
 * Dependencies: SFML 3.0, Projectile, Critter
 */
/**
 * @file tower.hpp
 * @brief Implementation of Tower header file
 */

 #pragma once
 #include <SFML/Graphics.hpp>
 #include <vector>
 #include <string>
 #include <memory>
 #include "Critter.hpp"
 #include "Projectile.hpp"
 #include "TargetingStrategy.hpp"
 #include "TargetingStrategyFactory.hpp"
 #include "DynamicTargetingManager.hpp"
class DynamicTargetingManager;

class Tower {
public:
    enum class Type {
        BASIC,  // Single-target
        AREA,   // Splash damage
        SPECIAL  // Specific enemies (Level 3 ennemies (yellow))
    };

    Tower(const sf::Vector2f& position, 
          int baseCost, 
          float baseRange, 
          float basePower, 
          float baseFireRate,
          const std::string& texturePath,
          Type towerType);
    
    virtual ~Tower() = default;

    // Core tower methods
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window) const;
    virtual bool upgrade(); // Returns false if max level reached

    // Projectile-related methods
    virtual void updateProjectiles(float deltaTime);
    virtual void drawProjectiles(sf::RenderWindow& window) const;
    virtual void findAndShootTarget(std::vector<std::unique_ptr<Critter>>& critters);

    // Selection
    virtual void setSelected(bool selected);

    // Getters
    virtual int getLevel() const { return currentLevel; }
    virtual int getCost() const { return cost; }
    virtual int getRefundValue() const;
    virtual float getRange() const { return range; }
    virtual sf::Vector2f getPosition() const { return sprite.getPosition(); }
    virtual bool canUpgrade() const { return currentLevel < MAX_LEVEL; }
    virtual int getUpgradeCost() const;
    virtual float getFireRate() const { return fireRate; }
    virtual float getPower() const { return power; }
    virtual const sf::Sprite& getSprite() const { return sprite; }
    virtual std::string getTexturePath() const { return texturePath; }
    virtual Type getTowerType() const { return towerType; }  
    virtual StrategyType getCurrentStrategyType() const;
    virtual float getAoeRadius() const { return aoeRadius; }
    virtual float getSpecialMultiplier() const { return 12.0f; } // The multiplier used in Projectile.cpp
protected:
    // Tower properties
    static const int MAX_LEVEL = 3;
    static const int LEVEL_2_COST = 100;
    static const int LEVEL_3_COST = 200;

private:
    // Core stats
    int currentLevel;
    int cost;
    float range;
    float power;
    float fireRate;
    float fireTimer;  // Tracks time between shots
    float lastShotTime; 

    // Tower identity
    Type towerType;
    std::string texturePath;

    // Graphics
    sf::Texture texture;
    sf::Sprite sprite;  
    
    
    // Area effect properties
    bool isSelected = false;
    float aoeRadius = 0.0f;  // Base radius for area effect
    static constexpr float UNIT_DISTANCE = 64.0f;  // One unit = one tile size
    

    

    

    
    // Projectiles
    std::vector<std::unique_ptr<Projectile>> projectiles;


    // Targeting Manager
    std::unique_ptr<DynamicTargetingManager> targetingManager;
   

    // Helper methods
    void loadTexture();
    Critter* findBestTarget(std::vector<std::unique_ptr<Critter>>& critters);
    std::string getUpgradedTexturePath() const;


};