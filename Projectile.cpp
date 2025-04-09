/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Projectile.cpp - Projectile system implementation
 * 
 * Purpose: Implements projectile movement, collision detection,
 * and damage application logic.
 * 
 * Dependencies: SFML 3.0, Projectile.hpp, cmath
 */

 /**
 * @file Projectile.cpp
 * @brief Implements projectile behavior and damage calculations
 */

#include "Projectile.hpp"
#include <cmath>

void Projectile::update(float deltaTime) {
    // Don't update if we've already hit or lost our target
    if (!target || hitTarget || target->isDead()) {
        hitTarget = true;
        return;
    }

    // Get current critter position
    sf::Vector2f targetPos = target->getSprite().getPosition();
    
    // Calculate direction
    sf::Vector2f direction = targetPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance > 0) {
        direction /= distance; // Normalize
        // Move towards target
        position += direction * speed * deltaTime;

        // Check if hit target - using slightly larger threshold to prevent rebounding
        if (distance < 15.0f) {
            dealDamage();
            hitTarget = true;
        }
    }
}

void Projectile::dealDamage() {
    // Safety check - make sure target exists and is alive
    if (!target || target->isDead()) return;

    // Deal damage based on tower type
    if (isSpecialTower && target->getTexturePath() == "textures/critter3.png") {
        // Special towers deal 1200% more damage to level 3 monsters
        target->takeDamage(damage * 12.0f, true);  // Pass true for special damage
    } else {
        // Normal damage for all other cases
        target->takeDamage(damage, false);  // Pass false for normal damage
    }

    // Handle AoE damage if applicable
    if (aoeRadius > 0.0f && allCritters) {
        sf::Vector2f impactPos = target->getSprite().getPosition();
        
        // Check all critters for AoE damage
        for (auto& critter : *allCritters) {
            // Skip invalid targets
            if (!critter || critter.get() == target || critter->isDead()) {
                continue;
            }

            // Calculate distance to this critter
            sf::Vector2f otherPos = critter->getSprite().getPosition();
            float dist = std::sqrt(
                std::pow(impactPos.x - otherPos.x, 2) + 
                std::pow(impactPos.y - otherPos.y, 2)
            );

            // If within AoE radius, deal splash damage
            if (dist <= aoeRadius) {
                critter->takeDamage(damage * 0.5f, false); // 50% splash damage, non-special
            }
        }
    }
}

void Projectile::draw(sf::RenderWindow& window) const {
    if (hitTarget) return;

    // Draw the projectile with different colors based on tower type
    sf::CircleShape projectileShape(5.0f);
    if (isSpecialTower) {
        projectileShape.setFillColor(sf::Color::Magenta);  // Purple/pink color for special tower
    } else {
        projectileShape.setFillColor(sf::Color::Yellow);   // Default yellow for other towers
    }
    
    // Set origin to center of the circle
    projectileShape.setOrigin(sf::Vector2f(5.0f, 5.0f));
    
    // Set position directly 
    projectileShape.setPosition(position);
    
    window.draw(projectileShape);

    // Draw AoE radius indicator for area effect towers
    if (aoeRadius > 0.0f && target) {
        sf::Vector2f targetPos = target->getSprite().getPosition();
        float dist = std::sqrt(
            std::pow(position.x - targetPos.x, 2) + 
            std::pow(position.y - targetPos.y, 2)
        );
        
        // Only show AoE indicator when projectile is close to target
        if (dist < 50.0f) {
            sf::CircleShape aoeIndicator(aoeRadius);
            aoeIndicator.setPosition(sf::Vector2f(targetPos.x - aoeRadius, targetPos.y - aoeRadius));
            aoeIndicator.setFillColor(sf::Color(255, 255, 0, 64));  // Semi-transparent yellow
            aoeIndicator.setOutlineColor(sf::Color(255, 255, 0, 128));
            aoeIndicator.setOutlineThickness(1.0f);
            window.draw(aoeIndicator);
        }
    }
}