/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TargetingStrategyFactory.hpp - Factory for targeting strategies
 * 
 * Purpose: Provides a centralized way to create targeting strategy objects.
 * 
 * Dependencies: TargetingStrategy.hpp
 */
/**
 * @file TargetingStrategyFactory.hpp
 * @brief Implementation of Targeting Strategy Factory header file
 */


 #ifndef TARGETING_STRATEGY_FACTORY_HPP
 #define TARGETING_STRATEGY_FACTORY_HPP
 
 #include "TargetingStrategy.hpp"
 #include <memory>
 #include <stdexcept>
 
 /**
  * Factory class to create targeting strategy objects
  */
 class TargetingStrategyFactory {
 public:
     /**
      * Create a targeting strategy based on the given strategy type
      * 
      * @param type The type of strategy to create
      * @return A shared_ptr to the created strategy
      */
     static std::shared_ptr<TargetingStrategy> createStrategy(StrategyType type) {
         switch (type) {
             case StrategyType::SMART:
                 return std::make_shared<SmartTargetingStrategy>();
             case StrategyType::EXIT_PROXIMITY:
                 return std::make_shared<ExitProximityStrategy>();
             case StrategyType::STRONGEST:
                 return std::make_shared<StrongestFirstStrategy>();
             case StrategyType::WEAKEST:
                 return std::make_shared<WeakestFirstStrategy>();
             case StrategyType::CLOSEST:
                 return std::make_shared<ClosestFirstStrategy>();
             default:
                 throw std::invalid_argument("Unknown strategy type");
         }
     }
     
     /**
      * Get the name of a strategy type for UI display
      * 
      * @param type The strategy type
      * @return A human-readable name for the strategy
      */
     static std::string getStrategyName(StrategyType type) {
         switch (type) {
             case StrategyType::SMART:
                 return "Smart Targeting";
             case StrategyType::EXIT_PROXIMITY:
                 return "Target Closest to Exit";
             case StrategyType::STRONGEST:
                 return "Target Strongest";
             case StrategyType::WEAKEST:
                 return "Target Weakest";
             case StrategyType::CLOSEST:
                 return "Target Closest";
             default:
                 return "Unknown Strategy";
         }
     }
 };
 
 #endif // TARGETING_STRATEGY_FACTORY_HPP