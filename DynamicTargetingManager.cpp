/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * DynamicTargetingManager.cpp - Tower targeting strategy manager implementation
 * 
 * Purpose: Implements the dynamic selection of targeting strategies based on
 * game conditions like critter health, position, and quantity.
 * 
 * Dependencies: DynamicTargetingManager.hpp, Tower, Critter
 */
/**
 * @file DynamicTargetingManager.cpp
 * @brief Implements the dynamic selection of targeting strategies
 */

 #include "DynamicTargetingManager.hpp"
 #include "tower.hpp" // For Tower class access
 #include "Critter.hpp" // For Critter class access
 #include <cmath>
 
 DynamicTargetingManager::DynamicTargetingManager(Tower* towerPtr)
     : tower(towerPtr),
       exitProximityStrategy(std::make_shared<ExitProximityStrategy>()),
       strongestStrategy(std::make_shared<StrongestFirstStrategy>()),
       weakestStrategy(std::make_shared<WeakestFirstStrategy>()),
       closestStrategy(std::make_shared<ClosestFirstStrategy>()),
       smartStrategy(std::make_shared<SmartTargetingStrategy>()),
       currentStrategyType(StrategyType::SMART)
 {
 }
 
 std::shared_ptr<TargetingStrategy> DynamicTargetingManager::selectStrategy(
     const std::vector<std::unique_ptr<Critter>>& critters)
 {
     // If no critters, use default smart strategy
     if (critters.empty()) {
         currentStrategyType = StrategyType::SMART;
         return smartStrategy;
     }
     
     // Game state analysis variables
     bool hasLevel3Critters = false;
     bool hasNearExitCritters = false;
     int crittersNearTower = 0;
     int totalCritters = 0;
     int maxWaypoint = 0;
     
     // Get tower position for distance calculations
     sf::Vector2f towerPos = tower->getPosition();
     float towerRange = tower->getRange();
     
     // First pass: find max waypoint to calculate exit proximity
     for (const auto& critter : critters) {
         if (critter->isDead()) continue;
         
         maxWaypoint = std::max(maxWaypoint, critter->getCurrentWaypoint());
     }
     
     // If no valid waypoint info, use a default value
     if (maxWaypoint == 0) {
         maxWaypoint = 10; // Reasonable default
     }
     
     // Second pass: analyze game state based on critters
     for (const auto& critter : critters) {
         if (critter->isDead()) continue;
         
         totalCritters++;
         
         // Check for special critters (Level 3)
         if (critter->getTexturePath() == "textures/critter3.png") {
             hasLevel3Critters = true;
         }
         
         // Check for critters near the exit (in last 30% of path)
         float progressRatio = static_cast<float>(critter->getCurrentWaypoint()) / maxWaypoint;
         if (progressRatio > 0.7f) {
             hasNearExitCritters = true;
         }
         
         // Check for critters near this tower
         sf::Vector2f critterPos = critter->getSprite().getPosition();
         float distance = std::sqrt(
             std::pow(towerPos.x - critterPos.x, 2.0f) + 
             std::pow(towerPos.y - critterPos.y, 2.0f)
         );
         
         if (distance <= towerRange) {
             crittersNearTower++;
         }
     }
     
     // Strategy selection logic based on analyzed conditions
     if (hasLevel3Critters && tower->getTowerType() == Tower::Type::SPECIAL) {
         // Special towers should target level 3 critters
         currentStrategyType = StrategyType::STRONGEST;
         return strongestStrategy;
     }
     else if (hasNearExitCritters) {
         // Critical situation: prioritize critters near exit
         currentStrategyType = StrategyType::EXIT_PROXIMITY;
         return exitProximityStrategy;
     }
     else if (crittersNearTower >= 5) {
         // Many critters in range: for area towers, target clusters
         if (tower->getTowerType() == Tower::Type::AREA) {
             currentStrategyType = StrategyType::CLOSEST;
             return closestStrategy;
         }
     }
     else if (totalCritters <= 3) {
         // Few critters: focus on strongest
         currentStrategyType = StrategyType::STRONGEST;
         return strongestStrategy;
     }
     
     // Default behavior: use smart balanced strategy
     currentStrategyType = StrategyType::SMART;
     return smartStrategy;
 }
 
 StrategyType DynamicTargetingManager::getCurrentStrategyType() const {
     return currentStrategyType;
 }