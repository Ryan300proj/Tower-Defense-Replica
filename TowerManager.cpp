/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TowerManager.cpp - Tower management implementation
 * 
 * Purpose: Implements tower placement logic, upgrade mechanics,
 * and coordinates tower-enemy interactions.
 * 
 * Dependencies: SFML 3.0, TowerManager.hpp, cmath, iostream
 */

 /**
 * @file TowerManager.cpp
 * @brief Handles tower placement, upgrading, and combat interactions
 */


 #include "TowerManager.hpp"
 #include <cmath>
 #include <iostream>
 #include <algorithm>
 #include <stdexcept>
 #include "TowerDecoratorFactory.hpp"
 #include "TargetingStrategyFactory.hpp"

TowerManager::TowerManager(MapGenerator& mapRef, CritterSpawner& spawnerRef)
    : mapGenerator(mapRef)
    , spawner(spawnerRef)
{
}

bool TowerManager::canPlaceTower(const sf::Vector2f& position) const {
    return !isPositionOnPath(position) && !isTowerPresent(position);
}

bool TowerManager::tryPlaceTower(const sf::Vector2f& position, int towerType) {
    sf::Vector2f snappedPos = snapToGrid(position);
    
    if (!canPlaceTower(snappedPos)) {
        return false;
    }

    // Get tower configuration based on tower type
    int cost;
    float range, power, fireRate;
    std::string texturePath;
    Tower::Type type;

    switch(towerType) {
        case 0: // Basic Tower
            cost = 100;
            range = 150.f;
            power = 50.f;
            fireRate = 2.f;
            texturePath = "textures/stage_1.png";
            type = Tower::Type::BASIC;
            break;
        case 1: // Area Tower
            cost = 150;
            range = 120.f;
            power = 75.f;
            fireRate = 1.6f;
            texturePath = "textures/area1.png";
            type = Tower::Type::AREA;
            break;
        case 2: // Special Tower 
            cost = 125;
            range = 140.f;
            power = 85.f;
            fireRate = 2.0f;
            texturePath = "textures/special1.png";
            type = Tower::Type::SPECIAL;
            break;
        default:
            return false;
    }

    // Check if player has enough currency
    if (spawner.getPlayerCurrency() < cost) {
        return false;
    }

    try {
        towers.push_back(std::make_unique<Tower>(
            snappedPos,
            cost,
            range,
            power,
            fireRate,
            texturePath,
            type
        ));

        spawner.adjustPlayerCurrency(-cost);
        notify();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error placing tower: " << e.what() << std::endl;
        return false;
    }
}

bool TowerManager::tryUpgradeTower(const sf::Vector2f& position) {
    sf::Vector2f snappedPos = snapToGrid(position);
    
    // Find the tower at this position
    auto it = std::find_if(towers.begin(), towers.end(),
        [&snappedPos](const std::unique_ptr<Tower>& tower) {
            return tower->getPosition() == snappedPos;
        });
    
    if (it == towers.end() || !(*it)->canUpgrade()) {
        return false;
    }

    // Get upgrade cost before applying decorators
    int upgradeCost = (*it)->getUpgradeCost();
    if (spawner.getPlayerCurrency() < upgradeCost) {
        return false;
    }

    try {
      
        
        // Apply the base upgrade first to update visuals
        if ((*it)->upgrade()) {
            // Apply decorators based on tower level and type
            Tower* upgradedTower = it->release(); // Release ownership from unique_ptr
            upgradedTower = TowerDecoratorFactory::applyLevelUpgrade(upgradedTower);
            
            // Replace the tower with the decorated version
            it = towers.erase(it);
            towers.push_back(std::unique_ptr<Tower>(upgradedTower));
            
            spawner.adjustPlayerCurrency(-upgradeCost);
            notify();
            return true;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error upgrading tower: " << e.what() << std::endl;
    }
    
    return false;
}




bool TowerManager::trySellTower(const sf::Vector2f& position) {
    sf::Vector2f snappedPos = snapToGrid(position);
    
    // Safe tower selling with error handling
    try {
        // Find the tower index
        auto it = std::find_if(towers.begin(), towers.end(), 
            [&snappedPos](const std::unique_ptr<Tower>& tower) {
                return tower->getPosition() == snappedPos;
            });
        
        // If tower found, sell it
        if (it != towers.end()) {
            int refundAmount = (*it)->getRefundValue();
            
            // Debug logging
            std::cout << "Tower sold at position (" 
                      << snappedPos.x << ", " 
                      << snappedPos.y 
                      << ") for: $" << refundAmount << std::endl;
            
            // Adjust currency before erasing to prevent potential state issues
            spawner.adjustPlayerCurrency(refundAmount);
            
            // Erase the tower
            towers.erase(it);
            notify();
            return true;
        }
        
        // No tower found at this position
        std::cout << "No tower found at position to sell" << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        // Comprehensive error logging
        std::cerr << "Critical error during tower selling: " << e.what() << std::endl;
        
        // Additional diagnostic information
        std::cerr << "Attempted to sell tower at: (" 
                  << snappedPos.x << ", " 
                  << snappedPos.y << ")" << std::endl;
        
        return false;
    }
}

Tower* TowerManager::getTowerAt(const sf::Vector2f& position) {
    sf::Vector2f snappedPos = snapToGrid(position);
    
    for (auto& tower : towers) {
        if (tower->getPosition() == snappedPos) {
            return tower.get();
        }
    }
    return nullptr;
}

sf::Vector2f TowerManager::snapToGrid(const sf::Vector2f& position) const {
    float x = std::floor(position.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2.f;
    float y = std::floor(position.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2.f;
    return sf::Vector2f(x, y);
}

void TowerManager::update(float deltaTime) {
    try {
        for (auto& tower : towers) {
            tower->update(deltaTime);
            tower->findAndShootTarget(spawner.getActiveCritters());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error during tower update: " << e.what() << std::endl;
    }
}

void TowerManager::draw(sf::RenderWindow& window) const {
    try {
        for (const auto& tower : towers) {
            tower->draw(window);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error during tower drawing: " << e.what() << std::endl;
    }
}

bool TowerManager::isPositionOnPath(const sf::Vector2f& position) const {
    // Convert world position to grid coordinates
    int gridX = static_cast<int>(position.x / TILE_SIZE);
    int gridY = static_cast<int>(position.y / TILE_SIZE);

    // Get the map from MapGenerator
    std::vector<std::vector<Tile>> gameMap = mapGenerator.getMap();

    // Check if the coordinates are within the map bounds
    if (gridY < 0 || gridY >= static_cast<int>(gameMap.size()) || 
        gridX < 0 || gridX >= static_cast<int>(gameMap[0].size())) {
        return false;
    }

    // Check if the tile at this position is a path tile
    return gameMap[gridY][gridX].getType() == Tile::TileType::Path ||
           gameMap[gridY][gridX].getType() == Tile::TileType::Beginning ||
           gameMap[gridY][gridX].getType() == Tile::TileType::End;
}

bool TowerManager::isTowerPresent(const sf::Vector2f& position) const {
    sf::Vector2f snappedPos = snapToGrid(position);
    for (const auto& tower : towers) {
        if (tower->getPosition() == snappedPos) {
            return true;
        }
    }
    return false;
}

// Tower Inspect Method
Tower* TowerManager::getTowerUnderMouse(const sf::Vector2f& mousePos) const {
    for (const auto& tower : towers) {
        sf::Vector2f towerPos = tower->getPosition();
        float distance = std::sqrt(
            std::pow(mousePos.x - towerPos.x, 2.0f) + 
            std::pow(mousePos.y - towerPos.y, 2.0f)
        );
        
        // If mouse is within a reasonable radius of tower (half a tile)
        if (distance <= TILE_SIZE / 2.0f) {
            return tower.get();
        }
    }
    return nullptr;
}


