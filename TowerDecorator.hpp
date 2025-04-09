/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TowerDecorator.hpp - Tower decorator pattern implementation
 * 
 * Purpose: Implements the Decorator pattern for Tower objects,
 * allowing dynamic extension of tower behaviors during gameplay.
 * 
 * Dependencies: tower.hpp, SFML 3.0
 */
/**
 * @file TowerDecorator.hpp
 * @brief Implementation of TowerDecorator header file
 */

 #pragma once
 #include <SFML/Graphics.hpp>
 #include "tower.hpp"
 
 // Abstract Tower Decorator base class
 class TowerDecorator : public Tower {
 public:
     TowerDecorator(Tower* tower) 
     : Tower(tower->getPosition(), 
             tower->getCost(), 
             tower->getRange(), 
             tower->getPower(), 
             tower->getFireRate(),
             tower->getTexturePath(),
             tower->getTowerType()),
       decoratedTower(tower) {}
       
     virtual ~TowerDecorator() {
         if (decoratedTower) {
             delete decoratedTower;
             decoratedTower = nullptr;
         }
     }
 
     // Override Tower methods to delegate to the wrapped tower
     void update(float deltaTime) override {
         decoratedTower->update(deltaTime);
     }
     
     void draw(sf::RenderWindow& window) const override {
         decoratedTower->draw(window);
     }
     
     bool upgrade() override {
         return decoratedTower->upgrade();
     }
     
     // Extend methods if needed, but for most we'll simply delegate
     void findAndShootTarget(std::vector<std::unique_ptr<Critter>>& critters) override {
         decoratedTower->findAndShootTarget(critters);
     }
     
     // Allow decorators to modify specific behaviors
     virtual int getRefundValue() const override {
         return decoratedTower->getRefundValue();
     }
     
     virtual int getUpgradeCost() const override {
         return decoratedTower->getUpgradeCost();
     }
     
     virtual void setSelected(bool selected) override {
         decoratedTower->setSelected(selected);
     }
     
     // Getter methods should be overridden to allow decorators to modify values
     virtual int getLevel() const override {
         return decoratedTower->getLevel();
     }
     
     virtual int getCost() const override {
         return decoratedTower->getCost();
     }
     
     virtual float getRange() const override {
         return decoratedTower->getRange();
     }
     
     virtual float getPower() const override {
         return decoratedTower->getPower();
     }
     
     virtual float getFireRate() const override {
         return decoratedTower->getFireRate();
     }
     
     virtual sf::Vector2f getPosition() const override {
         return decoratedTower->getPosition();
     }
     
     virtual bool canUpgrade() const override {
         return decoratedTower->canUpgrade();
     }
     
     virtual const sf::Sprite& getSprite() const override {
         return decoratedTower->getSprite();
     }
     
     virtual std::string getTexturePath() const override {
         return decoratedTower->getTexturePath();
     }
     
     virtual Tower::Type getTowerType() const override {
         return decoratedTower->getTowerType();
     }

     virtual float getAoeRadius() const override {
        return decoratedTower->getAoeRadius();
    }
    
    virtual float getSpecialMultiplier() const override {
        return decoratedTower->getSpecialMultiplier();
    }
     
 protected:
     Tower* decoratedTower;
 };