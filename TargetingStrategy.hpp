/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TargetingStrategy.hpp - Tower targeting strategy pattern
 * 
 * Purpose: Defines the Strategy pattern for tower targeting,
 * with an adaptive strategy that prioritizes targets based on multiple factors.
 * 
 * Dependencies: Critter.hpp, vector, memory
 */
/**
 * @file TargetingStrategy.hpp
 * @brief Implementation of Targeting Strategy header file
 */


 #ifndef TARGETING_STRATEGY_HPP
 #define TARGETING_STRATEGY_HPP
 
 #include "Critter.hpp"
 #include <vector>
 #include <memory>
 #include <stdexcept>
 
 /**
  * Strategy type enum to easily identify different strategies
  */
 enum class StrategyType {
     SMART,           // Multi-factor strategy
     EXIT_PROXIMITY,  // Target closest to exit
     STRONGEST,       // Target highest health
     WEAKEST,         // Target lowest health
     CLOSEST          // Target closest to tower
 };
 
 /**
  * Abstract base class for tower targeting strategies
  */
 class TargetingStrategy {
 public:
     virtual ~TargetingStrategy() = default;
     
     /**
      * Find a target for a tower based on the strategy's criteria
      * 
      * @param towerPosition Position of the tower
      * @param range Maximum range of the tower
      * @param critters Vector of available critters to target
      * @return Pointer to the best target, or nullptr if none found
      */
     virtual Critter* findTarget(const sf::Vector2f& towerPosition, 
                                float range,
                                std::vector<std::unique_ptr<Critter>>& critters) = 0;
                                
     // Get the strategy type
     virtual StrategyType getStrategyType() const = 0;
     
 protected:
     // Utility function to calculate distance between two points
     float calculateDistance(const sf::Vector2f& point1, const sf::Vector2f& point2) const;
 };
 
 /**
  * Strategy that prioritizes critters closest to the exit
  */
 class ExitProximityStrategy : public TargetingStrategy {
 public:
     Critter* findTarget(const sf::Vector2f& towerPosition, 
                         float range,
                         std::vector<std::unique_ptr<Critter>>& critters) override;
     
     StrategyType getStrategyType() const override { 
         return StrategyType::EXIT_PROXIMITY; 
     }
 };
 
 /**
  * Strategy that prioritizes strongest critters (highest health)
  */
 class StrongestFirstStrategy : public TargetingStrategy {
 public:
     Critter* findTarget(const sf::Vector2f& towerPosition, 
                         float range,
                         std::vector<std::unique_ptr<Critter>>& critters) override;
                         
     StrategyType getStrategyType() const override { 
         return StrategyType::STRONGEST; 
     }
 };
 
 /**
  * Strategy that prioritizes weakest critters (lowest health)
  */
 class WeakestFirstStrategy : public TargetingStrategy {
 public:
     Critter* findTarget(const sf::Vector2f& towerPosition, 
                         float range,
                         std::vector<std::unique_ptr<Critter>>& critters) override;
                         
     StrategyType getStrategyType() const override { 
         return StrategyType::WEAKEST; 
     }
 };
 
 /**
  * Strategy that prioritizes critters closest to the tower
  */
 class ClosestFirstStrategy : public TargetingStrategy {
 public:
     Critter* findTarget(const sf::Vector2f& towerPosition, 
                         float range,
                         std::vector<std::unique_ptr<Critter>>& critters) override;
                         
     StrategyType getStrategyType() const override { 
         return StrategyType::CLOSEST; 
     }
 };
 
 /**
  * A smart strategy that considers multiple factors to determine priority
  */
 class SmartTargetingStrategy : public TargetingStrategy {
 public:
     Critter* findTarget(const sf::Vector2f& towerPosition, 
                         float range,
                         std::vector<std::unique_ptr<Critter>>& critters) override;
     
     StrategyType getStrategyType() const override { 
         return StrategyType::SMART; 
     }
                         
 private:
     // Calculate a priority score for a critter (higher = higher priority)
     float calculatePriorityScore(const sf::Vector2f& towerPosition,
                                  Critter* critter,
                                  int currentWaypoint,
                                  int maxWaypoint);
 };
 
 #endif // TARGETING_STRATEGY_HPP