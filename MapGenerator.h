/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * MapGenerator.h - Game map creation and management
 * 
 * Purpose: Creates and manages game maps, handles path generation,
 * and provides tile-based navigation for enemies.
 * 
 * Dependencies: SFML 3.0, Tile, Observer.hpp
 */
/**
 * @file MapGenerator.h
 * @brief Implementation of Map Generator header file
 */
 #ifndef MAP_GENERATOR_H
 #define MAP_GENERATOR_H
 
 #include <SFML/Graphics.hpp>
 #include <iostream>
 #include <vector>
 #include <cstdlib>
 #include <ctime>
 #include "Tile.h"
 #include "Observer.hpp"
 
 class MapGenerator : public Observable {
 public:
     MapGenerator(int selectedLevel = 1, int mapWidth = 0, int mapHeight = 0);
 
     // Build internal 2D map array
     void builder(sf::RenderWindow* win);   // Initialize map array
 
     // Get waypoints for critter pathing
     std::vector<sf::Vector2f> getWaypoints() const; // Path points for critters
 
     // Draw the tiles to the window
     void draw(sf::RenderWindow& window);
     
     void printer(); // Debug print of map
     
     int getHeight();
     
     int getWidth();
     
     void interactiveMapCreation(sf::RenderWindow* mapWindow); // Custom map editor
     bool validatePath() const;  // Checks if path reaches end
     bool isValidMap() const;
     bool findPath(sf::Vector2i start, sf::Vector2i end) const;
     
     vector<vector<Tile>> getMap() const { return gameMap; }
 
     // Get required window size based on map dimensions
     sf::Vector2u getRequiredWindowSize() const {
         return sf::Vector2u(width * 64, height * 64 + UI_SPACE);
     }
 
 private:
     int level, width, height;
     static const unsigned int UI_SPACE = 100; // Space for UI elements
     static const int TILE_SIZE = 64;

     enum class MapCreationStep {
        PLACE_START,
        PLACE_END,
        PLACE_PATH
    };
    MapCreationStep currentCreationStep = MapCreationStep::PLACE_START;
    sf::Font font;
    void updateInstructionText(sf::Text& text);
 

    bool arePositionsAdjacent(const sf::Vector2i& pos1, const sf::Vector2i& pos2) const;
    
     // Our "map" is rows × columns of tiles
     std::vector<std::vector<Tile>> gameMap;
     const sf::Texture* getTextureForTile(Tile::TileType type) const;
     // Textures for drawing each tile type
     sf::Texture pathTexture;
     sf::Texture sceneryTexture;
     sf::Texture beginningTexture;
     sf::Texture endTexture;
     std::pair<int, int> startPos, endPos;
 
     void picker(char direction); // Select start/end positions
     void pathGenerator(); // Generate path between points
     void loadPresetLevel(); // Load predefined map
     bool isValidTile(int x, int y) const;
 };
 
 #endif