/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * CritterSpawner.hpp - Enemy wave management
 * 
 * Purpose: Controls enemy wave spawning, difficulty progression,
 * and manages active enemies on the map.
 * 
 * Dependencies: SFML 3.0, Critter
 */
/**
 * @file CritterSpawner.hpp
 * @brief Implementation of Critter Spawner header file
 */

#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <queue>
#include <memory>
#include "Observer.hpp"

class Critter;

class CritterSpawner : public Observable {
    public:
       struct CritterType {
           std::string texturePath;
           float speed;
           float health;
           int reward;
       };
    
       CritterSpawner(const std::vector<sf::Vector2f>& waypoints);
       
       void startNextWave();
       void update(float deltaTime);
       void draw(sf::RenderWindow& window);
       bool isWaveComplete() const;
       int getCurrentWave() const { return currentWave; }
       int getPlayerCurrency() const { return playerCurrency; }
       
       // New methods for life points system
       int getLifePoints() const { return lifePoints; }
       bool isGameOver() const { return gameOver; }
       bool isGameComplete() const { return gameComplete; }

       void reset() {
        // Can't reassign pathWaypoints because it's a reference
        // Reset other members
        spawnTimer = 0.0f;
        currentWave = 1;
        waveInProgress = false;
        gameComplete = false;
        playerCurrency = 1000;
        lifePoints = 10;
        gameOver = false;
        activecritters.clear();
        spawnQueue = std::queue<CritterType>();
    }
       
       bool canStartNewWave() const { 
          if (gameOver || gameComplete || currentWave > 3) {  // Add gameOver check
              return false;
          }
          return !waveInProgress; 
       }
       
       std::vector<std::unique_ptr<Critter>>& getActiveCritters() {
           return activecritters;
       }
       
       void adjustPlayerCurrency(int amount) { playerCurrency += amount; }
    
       Critter* getCritterUnderMouse(const sf::Vector2f& mousePos) const;

    
    private:
       void setupWave();
       void populateWaveQueue(int wave);
    
       const std::vector<sf::Vector2f>& pathWaypoints;
       std::vector<std::unique_ptr<Critter>> activecritters;
       std::queue<CritterType> spawnQueue;
       float spawnTimer;
       const float SPAWN_DELAY = 1.0f; // Time between spawns
       
       int currentWave;
       bool waveInProgress;
       bool gameComplete;
       int playerCurrency;
       
       // New variables for life points system
       int lifePoints;
       bool gameOver;
    };