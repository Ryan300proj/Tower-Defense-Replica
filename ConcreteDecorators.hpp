/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * ConcreteDecorators.hpp - Concrete tower decorator implementations
 * 
 * Purpose: Implements specific tower upgrade behaviors using the Decorator pattern,
 * including range, power, and special effect upgrades.
 * 
 * Dependencies: TowerDecorator.hpp, cmath
 */
/**
 * @file ConcreteDecorators.hpp
 * @brief Implementation of ConcreteDecorators header file
 */

 #pragma once
 #include "TowerDecorator.hpp"
 #include <cmath> // For sqrt() and pow() functions
 
 // Range Upgrade Decorator - Enhances tower range
 class RangeUpgradeDecorator : public TowerDecorator {
 public:
     RangeUpgradeDecorator(Tower* tower, float rangeBoost = 1.3f) 
     : TowerDecorator(tower), rangeBoost(rangeBoost) {}
     
     float getRange() const override {
         return decoratedTower->getRange() * rangeBoost; 
     }
     
     int getUpgradeCost() const override {
         // Range upgrades cost more
         return static_cast<int>(decoratedTower->getUpgradeCost() * 1.2f);
     }
     
     
 private:
     float rangeBoost;
 };
 
 // Power Upgrade Decorator - Enhances tower damage
 class PowerUpgradeDecorator : public TowerDecorator {
 public:
     PowerUpgradeDecorator(Tower* tower, float powerBoost = 1.5f) 
     : TowerDecorator(tower), powerBoost(powerBoost) {}
     
     float getPower() const override {
         return decoratedTower->getPower() * powerBoost;
     }
     
     int getUpgradeCost() const override {
         // Power upgrades cost more
         return static_cast<int>(decoratedTower->getUpgradeCost() * 1.3f);
     }
     
 private:
     float powerBoost;
 };
 
 // Fire Rate Upgrade Decorator - Enhances tower attack speed
 class FireRateUpgradeDecorator : public TowerDecorator {
 public:
     FireRateUpgradeDecorator(Tower* tower, float fireRateBoost = 1.4f) 
     : TowerDecorator(tower), fireRateBoost(fireRateBoost) {}
     
     float getFireRate() const override {
         return decoratedTower->getFireRate() * fireRateBoost;
     }
     
     int getUpgradeCost() const override {
         // Fire rate upgrades cost slightly more
         return static_cast<int>(decoratedTower->getUpgradeCost() * 1.25f);
     }
     
 private:
     float fireRateBoost;
 };
 
 // Special Effect Upgrade: Level 3 Critter Focus Decorator
 class Level3CritterFocusDecorator : public TowerDecorator {
 public:
     Level3CritterFocusDecorator(Tower* tower) 
     : TowerDecorator(tower) {}
     
     void findAndShootTarget(std::vector<std::unique_ptr<Critter>>& critters) override {
         // Prioritize level 3 critters if they exist
         Critter* level3Target = nullptr;
         
         for (auto& critter : critters) {
             if (!critter->isDead() && critter->getTexturePath() == "textures/critter3.png") {
                 sf::Vector2f critterPos = critter->getSprite().getPosition();
                 sf::Vector2f towerPos = getPosition();
                 
                 float distance = std::sqrt(
                     std::pow(towerPos.x - critterPos.x, 2.0f) + 
                     std::pow(towerPos.y - critterPos.y, 2.0f)
                 );
                 
                 if (distance <= getRange()) {
                     level3Target = critter.get();
                     break;
                 }
             }
         }
         
         if (level3Target) {
             // If we found a level 3 critter, direct the decorated tower to shoot
             decoratedTower->findAndShootTarget(critters);
             return;
         }
         
         // If no level 3 critter found, use normal targeting
         decoratedTower->findAndShootTarget(critters);
     }
     
     float getPower() const override {
         // Also grant a small power boost for this special focus
         return decoratedTower->getPower() * 1.2f;
     }
 };
 
 // Special Effect Upgrade: AoE Radius Enhancer Decorator
 class AoERadiusEnhancerDecorator : public TowerDecorator {
 public:
     AoERadiusEnhancerDecorator(Tower* tower, float radiusBoost = 1.4f) 
     : TowerDecorator(tower), radiusBoost(radiusBoost) {}
     
     float getRange() const override {
         return decoratedTower->getRange() * 1.1f; 
     }

     float getAoeRadius() const override {
        return decoratedTower->getAoeRadius() * radiusBoost;
    }
     
 private:
     float radiusBoost;
 };
 
 // Special Effect Upgrade: Refund Value Booster
 class RefundValueBoosterDecorator : public TowerDecorator {
 public:
     RefundValueBoosterDecorator(Tower* tower, float refundBoost = 1.2f) 
     : TowerDecorator(tower), refundBoost(refundBoost) {}
     
     int getRefundValue() const override {
         return static_cast<int>(decoratedTower->getRefundValue() * refundBoost);
     }
     
 private:
     float refundBoost;
 };