/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TargetingStrategy.cpp - Tower targeting strategy implementation
 * 
 * Purpose: Implements different targeting strategies for towers
 * that prioritize targets based on various criteria.
 * 
 * Dependencies: TargetingStrategy.hpp, cmath, limits, algorithm
 */
 /**
 * @file TargetingStrategy.cpp
 * @brief Implements different targeting strategies for towers
 */

 #include "TargetingStrategy.hpp"
 #include <cmath>
 #include <limits>
 #include <algorithm>

// Utility function implementation
float TargetingStrategy::calculateDistance(const sf::Vector2f& point1, const sf::Vector2f& point2) const {
    return std::sqrt(std::pow(point1.x - point2.x, 2.0f) + std::pow(point1.y - point2.y, 2.0f));
}

// ExitProximityStrategy Implementation
Critter* ExitProximityStrategy::findTarget(
    const sf::Vector2f& towerPosition, 
    float range,
    std::vector<std::unique_ptr<Critter>>& critters) 
{
    Critter* bestTarget = nullptr;
    int highestWaypoint = -1;
    
    for (auto& critter : critters) {
        if (critter->isDead()) continue;

        sf::Vector2f critterPos = critter->getSprite().getPosition();
        float distance = calculateDistance(towerPosition, critterPos);
        
        // Only consider critters in range
        if (distance <= range) {
            int currentWaypoint = critter->getCurrentWaypoint();
            if (currentWaypoint > highestWaypoint) {
                highestWaypoint = currentWaypoint;
                bestTarget = critter.get();
            }
        }
    }
    
    return bestTarget;
}

// StrongestFirstStrategy Implementation
Critter* StrongestFirstStrategy::findTarget(
    const sf::Vector2f& towerPosition, 
    float range,
    std::vector<std::unique_ptr<Critter>>& critters) 
{
    Critter* bestTarget = nullptr;
    float highestHealth = -1.0f;
    
    for (auto& critter : critters) {
        if (critter->isDead()) continue;

        sf::Vector2f critterPos = critter->getSprite().getPosition();
        float distance = calculateDistance(towerPosition, critterPos);
        
        // Only consider critters in range
        if (distance <= range) {
            float health = critter->getHealth();
            if (health > highestHealth) {
                highestHealth = health;
                bestTarget = critter.get();
            }
        }
    }
    
    return bestTarget;
}

// WeakestFirstStrategy Implementation
Critter* WeakestFirstStrategy::findTarget(
    const sf::Vector2f& towerPosition, 
    float range,
    std::vector<std::unique_ptr<Critter>>& critters) 
{
    Critter* bestTarget = nullptr;
    float lowestHealth = std::numeric_limits<float>::max();
    
    for (auto& critter : critters) {
        if (critter->isDead()) continue;

        sf::Vector2f critterPos = critter->getSprite().getPosition();
        float distance = calculateDistance(towerPosition, critterPos);
        
        // Only consider critters in range
        if (distance <= range) {
            float health = critter->getHealth();
            if (health < lowestHealth && health > 0) {
                lowestHealth = health;
                bestTarget = critter.get();
            }
        }
    }
    
    return bestTarget;
}

// ClosestFirstStrategy Implementation
Critter* ClosestFirstStrategy::findTarget(
    const sf::Vector2f& towerPosition, 
    float range,
    std::vector<std::unique_ptr<Critter>>& critters) 
{
    Critter* bestTarget = nullptr;
    float closestDistance = range + 1.0f; // Initialize with value outside range
    
    for (auto& critter : critters) {
        if (critter->isDead()) continue;

        sf::Vector2f critterPos = critter->getSprite().getPosition();
        float distance = calculateDistance(towerPosition, critterPos);
        
        // Only consider critters in range
        if (distance <= range && distance < closestDistance) {
            closestDistance = distance;
            bestTarget = critter.get();
        }
    }
    
    return bestTarget;
}

// SmartTargetingStrategy Implementation
Critter* SmartTargetingStrategy::findTarget(
    const sf::Vector2f& towerPosition, 
    float range,
    std::vector<std::unique_ptr<Critter>>& critters) 
{
    Critter* bestTarget = nullptr;
    float highestPriority = -1.0f;
    
    // Find the maximum waypoint in all critters to calculate relative progress
    int maxWaypoint = 0;
    for (auto& critter : critters) {
        if (!critter->isDead()) {
            maxWaypoint = std::max(maxWaypoint, critter->getCurrentWaypoint());
        }
    }
    
    // Evaluate each critter
    for (auto& critter : critters) {
        if (critter->isDead()) continue;

        sf::Vector2f critterPos = critter->getSprite().getPosition();
        float distance = calculateDistance(towerPosition, critterPos);

        // Only consider critters in range
        if (distance <= range) {
            float priority = calculatePriorityScore(
                towerPosition, 
                critter.get(), 
                critter->getCurrentWaypoint(),
                maxWaypoint
            );
            
            if (priority > highestPriority) {
                bestTarget = critter.get();
                highestPriority = priority;
            }
        }
    }

    return bestTarget;
}

float SmartTargetingStrategy::calculatePriorityScore(
    const sf::Vector2f& towerPosition,
    Critter* critter,
    int currentWaypoint,
    int maxWaypoint)
{
    // Get critter properties
    sf::Vector2f critterPos = critter->getSprite().getPosition();
    float health = critter->getHealth();
    float maxHealth = 800.0f; // Estimate of the maximum possible health
    
    // Calculate normalized factors (0.0 to 1.0)
    float exitProximityFactor = (float)currentWaypoint / std::max(1, maxWaypoint); // Higher = closer to exit
    float strengthFactor = health / maxHealth; // Higher = stronger
    float distanceFactor = 1.0f - (calculateDistance(towerPosition, critterPos) / 200.0f); // Higher = closer to tower
    
    // Calculate priority score with exit proximity as highest priority
    // Then strength, then proximity to tower
    float priority = (exitProximityFactor * 3.0f) + (strengthFactor * 2.0f) + distanceFactor;
    
    // Give special priority to critters that are both strong and near the exit
    if (exitProximityFactor > 0.7f && strengthFactor > 0.7f) {
        priority *= 1.5f;
    }
    
    return priority;
}