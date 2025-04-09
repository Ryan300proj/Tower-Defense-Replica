/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TowerDecoratorFactory.hpp - Factory for creating tower decorators
 * 
 * Purpose: Provides a centralized way to apply decorators to towers
 * based on upgrade type or special conditions.
 * 
 * Dependencies: ConcreteDecorators.hpp
 */
/**
 * @file TowerDecoratorFactory.hpp
 * @brief Implementation of TowerDecoratorFactory header file
 */

 #pragma once
 #include "ConcreteDecorators.hpp"
 #include <string>
 #include <memory>
 
 class TowerDecoratorFactory {
 public:
     // Enum to make it easier to specify the type of upgrade
     enum UpgradeType {
         RANGE_BOOST,
         POWER_BOOST,
         FIRE_RATE_BOOST,
         LEVEL3_FOCUS,
         AOE_ENHANCE,
         REFUND_BOOST
     };
 
     // Factory method to apply decorator based on upgrade type
     static Tower* applyUpgrade(Tower* baseTower, UpgradeType upgradeType) {
         switch (upgradeType) {
             case RANGE_BOOST:
                 return new RangeUpgradeDecorator(baseTower);
             case POWER_BOOST:
                 return new PowerUpgradeDecorator(baseTower);
             case FIRE_RATE_BOOST:
                 return new FireRateUpgradeDecorator(baseTower);
             case LEVEL3_FOCUS:
                 return new Level3CritterFocusDecorator(baseTower);
             case AOE_ENHANCE:
                 return new AoERadiusEnhancerDecorator(baseTower);
             case REFUND_BOOST:
                 return new RefundValueBoosterDecorator(baseTower);
             default:
                 return baseTower; // No decorator applied
         }
     }
     
     // Apply decorator based on tower level and type
     static Tower* applyLevelUpgrade(Tower* baseTower) {
         int currentLevel = baseTower->getLevel();
         Tower::Type towerType = baseTower->getTowerType();
         
         // Apply different decorators based on tower level and type
         switch (currentLevel) {
             case 1:
                 // Level 1 to 2 upgrades
                 if (towerType == Tower::Type::BASIC) {
                     // Basic towers get range and fire rate
                     baseTower = new RangeUpgradeDecorator(baseTower, 1.2f);
                     baseTower = new FireRateUpgradeDecorator(baseTower, 1.3f);
                 }
                 else if (towerType == Tower::Type::AREA) {
                     // Area towers get enhanced AoE and power
                     baseTower = new AoERadiusEnhancerDecorator(baseTower, 1.3f);
                     baseTower = new PowerUpgradeDecorator(baseTower, 1.4f);
                 }
                 else if (towerType == Tower::Type::SPECIAL) {
                     // Special towers get power and level 3 focus
                     baseTower = new PowerUpgradeDecorator(baseTower, 1.6f);
                     baseTower = new Level3CritterFocusDecorator(baseTower);
                 }
                 break;
                 
             case 2:
                 // Level 2 to 3 upgrades (more powerful)
                 if (towerType == Tower::Type::BASIC) {
                     baseTower = new RangeUpgradeDecorator(baseTower, 1.3f);
                     baseTower = new PowerUpgradeDecorator(baseTower, 1.5f);
                     baseTower = new FireRateUpgradeDecorator(baseTower, 1.4f);
                 }
                 else if (towerType == Tower::Type::AREA) {
                     baseTower = new AoERadiusEnhancerDecorator(baseTower, 1.5f);
                     baseTower = new PowerUpgradeDecorator(baseTower, 1.6f);
                     baseTower = new RefundValueBoosterDecorator(baseTower, 1.3f);
                 }
                 else if (towerType == Tower::Type::SPECIAL) {
                     baseTower = new PowerUpgradeDecorator(baseTower, 2.0f);
                     baseTower = new Level3CritterFocusDecorator(baseTower);
                     baseTower = new FireRateUpgradeDecorator(baseTower, 1.5f);
                 }
                 break;
         }
         
         return baseTower;
     }
     
     // Create a fully decorated tower in one step for pre-built upgrades
     static std::unique_ptr<Tower> createDecoratedTower(const sf::Vector2f& position, Tower::Type towerType, int upgradeLevel) {
         // Base tower properties based on type
         int cost;
         float range, power, fireRate;
         std::string texturePath;
         
         switch(towerType) {
             case Tower::Type::BASIC:
                 cost = 100;
                 range = 150.f;
                 power = 50.f;
                 fireRate = 2.f;
                 texturePath = "textures/stage_1.png";
                 break;
             case Tower::Type::AREA:
                 cost = 150;
                 range = 120.f;
                 power = 75.f;
                 fireRate = 1.6f;
                 texturePath = "textures/area1.png";
                 break;
             case Tower::Type::SPECIAL:
                 cost = 125;
                 range = 140.f;
                 power = 85.f;
                 fireRate = 2.0f;
                 texturePath = "textures/special1.png";
                 break;
         }
         
         // Create base tower
         Tower* tower = new Tower(position, cost, range, power, fireRate, texturePath, towerType);
         
         // Apply upgrades based on requested level
         for (int i = 1; i < upgradeLevel; i++) {
             tower->upgrade(); // Level up the tower visually
             tower = applyLevelUpgrade(tower); // Apply appropriate decorators
         }
         
         return std::unique_ptr<Tower>(tower);
     }
 };