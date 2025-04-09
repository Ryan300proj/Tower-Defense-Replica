/**
 * COMP 345 - Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Critter.cpp - Enemy entity implementation
 * 
 * Purpose: Implements enemy behavior including waypoint movement,
 * damage handling, and health/reward systems.
 * 
 * Dependencies: SFML 3.0, Critter.hpp, stdexcept, cmath
 */

/**
 * @file Critter.cpp
 * @brief Implementation of the Critter class
 */

#include "Critter.hpp"
#include <stdexcept>
#include <cmath>
// Initializes critter with movement path and stats
Critter::Critter(const std::vector<sf::Vector2f>& waypoints,
    const std::string& texturePath,
    float moveSpeed,
    float hp,
    int rewardPoints)
: path(waypoints),
currentTarget(0),
speed(moveSpeed),
health(hp),
reward(rewardPoints),
texture(),
sprite(texture),
reachedEnd(false),
dead(false),
texturePath(texturePath),
flashTimer(0.0f)
{
    if (path.empty()) {
        throw std::runtime_error("ERROR: Critter path is empty! Check MapGenerator.");
    }

    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("ERROR: Could not load " + texturePath + "! Ensure it exists.");
    }

    sprite.setTexture(texture, true);
    sprite.setScale(sf::Vector2f(0.5f, 0.5f));
    sprite.setPosition(path.front());
}

void Critter::update(float deltaTime) {
    // Handle flash effect
    if (flashTimer > 0) {
        flashTimer -= deltaTime;
        if (flashTimer <= 0) {
            sprite.setColor(sf::Color::White);
        }
    }
    
    if (reachedEnd || currentTarget >= static_cast<int>(path.size()) || dead) return;
    // Calculate direction to next waypoint
    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f targetPos  = path[currentTarget];

    sf::Vector2f direction = targetPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.f) {
        direction /= distance; // Normalize direction
    }

    sprite.move(direction * speed * deltaTime);
    // Check if reached waypoint
    if (distance < speed * deltaTime) {
        ++currentTarget;
        if (currentTarget >= static_cast<int>(path.size())) {
            reachedEnd = true;
        }
    }
}

const sf::Sprite& Critter::getSprite() const {
    return sprite;
}

bool Critter::hasReachedEnd() const {
    return reachedEnd;
}

void Critter::takeDamage(float damage, bool isSpecialDamage) {
    health -= damage;
    
    // Visual feedback with different colors based on damage type
    if (isSpecialDamage) {
        sprite.setColor(sf::Color(255, 0, 255, 255));  // Bright magenta for special tower hits
    } else {
        sprite.setColor(sf::Color(255, 0, 0, 255));    // Bright red for normal hits
    }
    flashTimer = FLASH_DURATION;  // Reset flash timer
    
    if (health <= 0) {
        dead = true;
    }
}

bool Critter::isDead() const {
    return dead;
}

int Critter::getReward() const {
    return reward;
}

std::string Critter::getTexturePath() const {
    return texturePath;
}



void Critter::drawHealthBar(sf::RenderWindow& window) const {
    // Don't draw health bars for dead critters
    if (dead) return;
    
    // Get current sprite position and bounds
    sf::Vector2f position = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    // Calculate health bar dimensions
    const float BAR_WIDTH = bounds.size.x;  // SFML 3.0 uses size.x
    const float BAR_HEIGHT = 5.0f;
    const float BAR_OFFSET = 5.0f;  // Distance above sprite
    
    // bound size and position
    float centerX = bounds.position.x + bounds.size.x / 2.0f;
    
    // Background (empty) health bar - red
    sf::RectangleShape backgroundBar(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
    backgroundBar.setFillColor(sf::Color(200, 0, 0, 200));  // Semi-transparent red
    
    // Position using the calculated center X position
    backgroundBar.setPosition(
        sf::Vector2f(
            centerX - BAR_WIDTH/2,  // Center horizontally using the sprite's center
            position.y - bounds.size.y/2 - BAR_HEIGHT - BAR_OFFSET  // Position above sprite
        )
    );
    
    // Calculate filled portion of health bar
    float healthRatio = health / getMaxHealth();
    float fillWidth = BAR_WIDTH * healthRatio;
    
    // Filled health bar - green
    sf::RectangleShape fillBar(sf::Vector2f(fillWidth, BAR_HEIGHT));
    fillBar.setFillColor(sf::Color(0, 200, 0, 200));  // Semi-transparent green
    fillBar.setPosition(
        sf::Vector2f(
            backgroundBar.getPosition().x,  // Same x as background
            backgroundBar.getPosition().y   // Same y as background
        )
    );
    
    // Draw both bars
    window.draw(backgroundBar);
    window.draw(fillBar);
}

float Critter::getMaxHealth() const {
    if (texturePath == "textures/critter.png") {
        return 100.0f + 10.0f;  // Basic critter approximate base health
    } else if (texturePath == "textures/critter2.png") {
        return 200.0f + 25.0f;  // Medium critter approximate base health
    } else if (texturePath == "textures/critter3.png") {
        return 800.0f + 200.0f;  // Strong critter approximate base health
    }
    return 100.0f;  // Fallback
}