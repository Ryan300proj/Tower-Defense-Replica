/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Projectile.hpp - Tower projectile system
 * 
 * Purpose: Manages tower projectiles including movement, collision
 * detection, and damage application to enemies.
 * 
 * Dependencies: SFML 3.0, Critter
 */
/**
 * @file Projectile.hpp
 * @brief Implementation of Projectile header file
 */

#pragma once
#include <SFML/Graphics.hpp>
#include "Critter.hpp"
#include <vector>

class Projectile {
public:
    Projectile(const sf::Vector2f& origin, 
               Critter* target, 
               float damage, 
               float aoeRadius = 0.0f, // Splash damage radius
               std::vector<std::unique_ptr<Critter>>* allCritters = nullptr,
               bool isSpecial = false) // Special tower projectile
        : position(origin)
        , target(target)
        , speed(200.0f)
        , damage(damage)
        , aoeRadius(aoeRadius)
        , allCritters(allCritters)
        , hitTarget(false)
        , isSpecialTower(isSpecial)
    {}
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isActive() const { return !hitTarget; }

private:
    void dealDamage();

    sf::Vector2f position;
    Critter* target;
    float speed;
    float damage;
    float aoeRadius;
    std::vector<std::unique_ptr<Critter>>* allCritters;
    bool hitTarget;
    bool isSpecialTower;
};