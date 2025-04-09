/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * CritterSpawner.cpp - Enemy wave system implementation
 * 
 * Purpose: Implements wave spawning mechanics, enemy type generation,
 * and manages active enemies during gameplay.
 * 
 * Dependencies: SFML 3.0, CritterSpawner.hpp, Critter.hpp, CritterFactory.hpp
 */

 /**
 * @file CritterSpawner.cpp
 * @brief Manages enemy wave generation and spawning
 */

 #include "CritterSpawner.hpp"
 #include "Critter.hpp"
 #include "CritterFactory.hpp"
 
 CritterSpawner::CritterSpawner(const std::vector<sf::Vector2f>& waypoints)
     : pathWaypoints(waypoints), 
       spawnTimer(0.0f),
       currentWave(1),
       waveInProgress(false),
       gameComplete(false),
       playerCurrency(1000),
       lifePoints(10),    // Start with 10 life points
       gameOver(false)
 {
 }
 
 void CritterSpawner::startNextWave() {
     if (canStartNewWave()) {
         setupWave();
     }
 }
 
 void CritterSpawner::setupWave() {
     populateWaveQueue(currentWave);
     waveInProgress = true;
 }
 
 void CritterSpawner::populateWaveQueue(int wave) {
     // Use the CritterFactory to create the appropriate wave
     spawnQueue = CritterFactory::createWave(wave);
 }
 
 void CritterSpawner::update(float deltaTime) {
    // Update existing critters
    for (auto it = activecritters.begin(); it != activecritters.end();) {
        (*it)->update(deltaTime);
        
        if ((*it)->isDead()) {
            // Give player reward for killing critter
            playerCurrency += (*it)->getReward();
            it = activecritters.erase(it);
            notify();
        }
        else if ((*it)->hasReachedEnd()) {
            // Apply penalties based on critter type
            std::string texturePath = (*it)->getTexturePath();
            
            // Apply currency penalty (existing logic)
            if (texturePath == "textures/critter.png") {
                playerCurrency -= 10;
                // New: Deduct 1 life point for level 1 critter
                lifePoints -= 1;
            } else if (texturePath == "textures/critter2.png") {
                playerCurrency -= 50;
                // New: Deduct 2 life points for level 2 critter
                lifePoints -= 2;
            } else if (texturePath == "textures/critter3.png") {
                playerCurrency -= 100;
                // New: Deduct 3 life points for level 3 critter
                lifePoints -= 3;
            }
            
            // Check if player has lost the game
            if (lifePoints <= 0) {
                lifePoints = 0;  // Ensure it doesn't go negative
                gameOver = true;
                // Clear the active critters and spawn queue when game is over
                spawnQueue = std::queue<CritterType>();
                waveInProgress = false;
            }
            
            it = activecritters.erase(it);
            notify();
        } else {
            ++it;
        }
    }

    // Only process spawning if game is not over
    if (!gameOver) {
        // Spawn new critters on timer
        if (!spawnQueue.empty()) {
            spawnTimer += deltaTime;
            if (spawnTimer >= SPAWN_DELAY) {
                const CritterType& type = spawnQueue.front();
                activecritters.push_back(std::make_unique<Critter>(
                    pathWaypoints,
                    type.texturePath,
                    type.speed,
                    type.health,
                    type.reward
                ));
                spawnQueue.pop();
                spawnTimer = 0.0f;
                notify();
            }
        }
        
        // Check if wave is complete
        if (waveInProgress && spawnQueue.empty() && activecritters.empty()) {
            waveInProgress = false;
            
            if (currentWave < 3) {
                currentWave++;  // Increment wave if not yet at wave 3
            } else if (currentWave == 3) {
                gameComplete = true;  // Only set game complete after wave 3 is finished
            }
        }
    }
}
 
void CritterSpawner::draw(sf::RenderWindow& window) {
    for (const auto& critter : activecritters) {
        window.draw(critter->getSprite());
        critter->drawHealthBar(window);  // Draw health bar after the sprite
    }
}
 
 bool CritterSpawner::isWaveComplete() const {
     return !waveInProgress;
 }


// Critter Inspect Feature
 Critter* CritterSpawner::getCritterUnderMouse(const sf::Vector2f& mousePos) const {
    for (const auto& critter : activecritters) {
        if (critter->isDead()) continue;
        
        // Get the critter's sprite bounds
        sf::FloatRect bounds = critter->getSprite().getGlobalBounds();
        
        // Check if the mouse position is inside the sprite bounds
        if (bounds.contains(mousePos)) {
            return critter.get();
        }
    }
    
    // No critter found under the mouse
    return nullptr;
}