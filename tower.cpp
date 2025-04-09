/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * tower.cpp - Defense tower implementation
 * 
 * Purpose: Implements tower behavior including targeting, shooting,
 * upgrading, and damage calculations.
 * 
 * Dependencies: SFML 3.0, tower.hpp, cmath, stdexcept, algorithm
 */
/**
 * @file Tower.cpp
 * @brief Implementation of defensive towers and their combat mechanics
 */

 #include "tower.hpp"
 #include "TargetingStrategy.hpp"
 #include "TargetingStrategyFactory.hpp"
 #include "DynamicTargetingManager.hpp"
 #include <cmath>
 #include <stdexcept>
 #include <algorithm>
 
 
 Tower::Tower(const sf::Vector2f& position, 
     int baseCost, 
     float baseRange, 
     float basePower, 
     float baseFireRate,
     const std::string& texPath,
     Type towerType)
 : currentLevel(1)
 , cost(baseCost)
 , range(baseRange)
 , power(basePower)
 , fireRate(baseFireRate)
 , fireTimer(0.0f)
 , lastShotTime(0.0f)  
 , towerType(towerType)
 , texturePath(texPath)
 , texture()
 , sprite(texture)  
 , isSelected(false)
 , aoeRadius(towerType == Type::AREA ? UNIT_DISTANCE : 0.0f)
 , targetingManager(new DynamicTargetingManager(this))
 {
     loadTexture();
     sprite.setPosition(position);
 }
 
 void Tower::loadTexture() {
     if (!texture.loadFromFile(texturePath)) {
         throw std::runtime_error("Failed to load tower texture: " + texturePath);
     }
     sprite.setTexture(texture, true);
     
     // Scale sprite to fit one tile (64x64)
     const float TILE_SIZE = 64.0f;
     const float TEXTURE_SIZE = 16.0f; // Assuming 16x16 textures
     const float SCALE_FACTOR = TILE_SIZE / TEXTURE_SIZE;
     sprite.setScale(sf::Vector2f(SCALE_FACTOR, SCALE_FACTOR));
     
     // Center the sprite origin
     sf::FloatRect bounds = sprite.getLocalBounds();
     sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
 }
 
 void Tower::update(float deltaTime) {
     fireTimer += deltaTime;
     updateProjectiles(deltaTime);
 }
 
 void Tower::draw(sf::RenderWindow& window) const {
     window.draw(sprite);
     drawProjectiles(window);
     
     // Draw range indicator 
     if (isSelected) {
         sf::CircleShape rangeIndicator(range);
         // Convert the position to Vector2f and offset by range to center the circle
         rangeIndicator.setPosition(
             sf::Vector2f(
                 sprite.getPosition().x - range,
                 sprite.getPosition().y - range
             )
         );
         rangeIndicator.setFillColor(sf::Color(255, 255, 255, 32));
         rangeIndicator.setOutlineColor(sf::Color(255, 255, 255, 128));
         rangeIndicator.setOutlineThickness(1.0f);
         window.draw(rangeIndicator);
     }
 }
 

StrategyType Tower::getCurrentStrategyType() const {
    return targetingManager->getCurrentStrategyType();
}


 bool Tower::upgrade() {
     if (currentLevel >= MAX_LEVEL) {
         return false;
     }
 
     std::string nextLevelTexturePath = getUpgradedTexturePath();
     
     // Store the old position before upgrading
     sf::Vector2f oldPosition = sprite.getPosition();
     
     currentLevel++;
     
     // Improve tower stats with each upgrade
     range *= 1.2f;    // 20% range increase
     power *= 1.5f;    // 50% power increase
     fireRate *= 1.2f; // 20% fire rate increase
     
     // Update AoE radius for area towers
     if (towerType == Type::AREA) {
         aoeRadius = UNIT_DISTANCE * currentLevel;  // Increase AoE radius with level
     }
     
     texturePath = nextLevelTexturePath;
     loadTexture();
     
     // Restore the position after texture reload
     sprite.setPosition(oldPosition);
     
     return true;
 }
 
 std::string Tower::getUpgradedTexturePath() const {
     std::string basePath;
     switch(towerType) {
         case Type::BASIC:
             basePath = "textures/stage_";
             break;
         case Type::AREA:
             basePath = "textures/area";
             break;
         case Type::SPECIAL:
             basePath = "textures/special";
             break;
         default:
             throw std::runtime_error("Unknown tower type");
     }
     return basePath + std::to_string(currentLevel + 1) + ".png";
 }
 
 void Tower::updateProjectiles(float deltaTime) {
     // Update existing projectiles
     for (auto& projectile : projectiles) {
         projectile->update(deltaTime);
     }
 
     // Remove inactive projectiles
     projectiles.erase(
         std::remove_if(projectiles.begin(), projectiles.end(), 
                        [](const std::unique_ptr<Projectile>& p) { return !p->isActive(); }),
         projectiles.end()
     );
 }
 
 void Tower::drawProjectiles(sf::RenderWindow& window) const {
     for (const auto& projectile : projectiles) {
         projectile->draw(window);
     }
 }
 
 void Tower::findAndShootTarget(std::vector<std::unique_ptr<Critter>>& critters) {
     if (fireTimer - lastShotTime < (1.0f / fireRate)) {
         return;
     }
 
     Critter* target = findBestTarget(critters);
     if (target) {
         // Create a projectile with AoE properties if it's an area tower
         float projectileAoeRadius = (towerType == Type::AREA) ? aoeRadius : 0.0f;
         bool isSpecial = (towerType == Type::SPECIAL);  // Check if this is a special tower
         
         projectiles.push_back(std::make_unique<Projectile>(
             sprite.getPosition(),
             target,
             power,
             projectileAoeRadius,
             &critters,  // Pass reference to all critters for AoE damage
             isSpecial   // Pass the special tower flag
         ));
         
         lastShotTime = fireTimer;
     }
 }
 
 Critter* Tower::findBestTarget(std::vector<std::unique_ptr<Critter>>& critters) {
    // Use the targeting manager to select appropriate strategy
    std::shared_ptr<TargetingStrategy> strategy = targetingManager->selectStrategy(critters);
    
    // Use the selected strategy to find a target
    return strategy->findTarget(sprite.getPosition(), range, critters);
}
 
 int Tower::getRefundValue() const {
     // Return 75% of total investment
     int totalInvestment = cost;
     if (currentLevel >= 2) totalInvestment += LEVEL_2_COST;
     if (currentLevel >= 3) totalInvestment += LEVEL_3_COST;
     return static_cast<int>(totalInvestment * 0.75f);
 }
 
 int Tower::getUpgradeCost() const {
     if (currentLevel == 1) return LEVEL_2_COST;
     if (currentLevel == 2) return LEVEL_3_COST;
     return 0;  // Can't upgrade level 3
 }
 
 void Tower::setSelected(bool selected) {
     isSelected = selected;
 }