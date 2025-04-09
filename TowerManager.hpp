/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TowerManager.hpp - Tower placement and management
 * 
 * Purpose: Handles tower placement validation, upgrading, selling,
 * and coordinates tower interactions with enemies.
 * 
 * Dependencies: SFML 3.0, Tower, MapGenerator, CritterSpawner, Observer.hpp
 */
/**
 * @file TowerManager.hpp
 * @brief Implementation of Tower Manager header file
 */

 #pragma once
 #include <SFML/Graphics.hpp>
 #include <vector>
 #include <memory>
 #include "tower.hpp"
 #include "MapGenerator.h"
 #include "CritterSpawner.hpp"
 #include "Observer.hpp"
 #include "TowerDecoratorFactory.hpp"
 #include "TargetingStrategy.hpp"
 #include "TargetingStrategyFactory.hpp"

class TowerManager : public Observable {
public:
    TowerManager(MapGenerator& mapRef, CritterSpawner& spawnerRef);
    
    bool canPlaceTower(const sf::Vector2f& position) const;
    bool tryPlaceTower(const sf::Vector2f& position, int towerType);
    bool tryUpgradeTower(const sf::Vector2f& position);
    bool trySellTower(const sf::Vector2f& position);
    void reset() {
        towers.clear();
    }
    bool applyTowerDecorator(const sf::Vector2f& position, TowerDecoratorFactory::UpgradeType upgradeType);
    Tower* getTowerAt(const sf::Vector2f& position); // Returns false if max level
    sf::Vector2f snapToGrid(const sf::Vector2f& position) const; // Returns false if no tower
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    Tower* getTowerUnderMouse(const sf::Vector2f& mousePos) const; // Tower Inspect

   

private:
    static constexpr int TILE_SIZE = 64;
    static constexpr int BASIC_TOWER_COST = 100;
    
    std::vector<std::unique_ptr<Tower>> towers;
    MapGenerator& mapGenerator;
    CritterSpawner& spawner;

    bool isPositionOnPath(const sf::Vector2f& position) const;
    bool isTowerPresent(const sf::Vector2f& position) const;
};