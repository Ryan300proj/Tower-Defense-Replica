/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * DynamicTargetingManager.hpp - Tower targeting strategy manager
 * 
 * Purpose: Dynamically selects the most appropriate targeting strategy
 * based on the current game conditions.
 * 
 * Dependencies: TargetingStrategy.hpp, tower.hpp
 */
/**
 * @file DynamicTargetingManager.hpp
 * @brief Implementation of Dynamic Targeting Manager header file
 */

 #ifndef DYNAMIC_TARGETING_MANAGER_HPP
 #define DYNAMIC_TARGETING_MANAGER_HPP
 
 #include "TargetingStrategy.hpp"
 #include "TargetingStrategyFactory.hpp"
 #include <memory>
 #include <vector>
 
 // Forward declarations
 class Tower;
 class Critter;
 
 /**
  * Manages the dynamic selection of targeting strategies for towers
  */
 class DynamicTargetingManager {
 public:
     /**
      * Constructor
      * 
      * @param towerPtr Pointer to the tower this manager is attached to
      */
     DynamicTargetingManager(Tower* towerPtr);
     
     /**
      * Selects the most appropriate strategy based on current game conditions
      * 
      * @param critters Vector of critters currently in play
      * @return A shared pointer to the selected strategy
      */
     std::shared_ptr<TargetingStrategy> selectStrategy(
         const std::vector<std::unique_ptr<Critter>>& critters);
         
     /**
      * Gets the current strategy type
      */
     StrategyType getCurrentStrategyType() const;
     
 private:
     Tower* tower;
     
     // Cache of strategy objects to avoid constant creation/destruction
     std::shared_ptr<ExitProximityStrategy> exitProximityStrategy;
     std::shared_ptr<StrongestFirstStrategy> strongestStrategy;
     std::shared_ptr<WeakestFirstStrategy> weakestStrategy;
     std::shared_ptr<ClosestFirstStrategy> closestStrategy;
     std::shared_ptr<SmartTargetingStrategy> smartStrategy;
     
     // Tracks the current strategy for status reporting
     StrategyType currentStrategyType;
 };
 
 #endif // DYNAMIC_TARGETING_MANAGER_HPP