/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * MapGenerator.cpp - Map system implementation
 * 
 * Purpose: Implements map creation, path generation, and handles
 * tile-based game environment setup and rendering.
 * 
 * Dependencies: SFML 3.0, MapGenerator.h, Tile.h, iostream, queue
 */
/**
 * @file MapGenerator.cpp
 * @brief Implements map creation and path generation systems
 */

 #include "MapGenerator.h"
 #include "Tile.h"
 #include <iostream>
 #include <queue>
 #include <SFML/Graphics.hpp>
 #include <SFML/Window.hpp>
 
 MapGenerator::MapGenerator(int selectedLevel, int mapWidth, int mapHeight) {
     srand(time(0)); // Seed the random number generator
     
     // Set level directly from parameter
     level = selectedLevel;
     
     // For custom maps, use provided dimensions
     if (level == 4) {
         width = mapWidth;
         height = mapHeight;
     } else {
         // For preset levels, use default dimensions
         switch(level) {
             case 1: // Easy
                 width = 18;
                 height = 18;
                 break;
             case 2: // Normal
                 width = 12;
                 height = 12;
                 break;
             case 3: // Hard
                 width = 8;
                 height = 8;
                 break;
             default:
                 width = 10;
                 height = 10;
                 break;
         }
     }
 
 
     if (!pathTexture.loadFromFile("textures/dirt_path_top.png")) {
         throw std::runtime_error("ERROR: Could not load textures/dirt_path_top.png");
     }
     pathTexture.setSmooth(true);
     if (!sceneryTexture.loadFromFile("textures/grass_block_top.png")) {
         throw std::runtime_error("ERROR: Could not load textures/grass_block_top.png");
     }
     sceneryTexture.setSmooth(true);
     if (!beginningTexture.loadFromFile("textures/emerald_block.png")) {
         throw std::runtime_error("ERROR: Could not load textures/emerald_block.png");
     }
    // beginningTexture.setSmooth(true); 
     if (!endTexture.loadFromFile("textures/bricks.png")) {
         throw std::runtime_error("ERROR: Could not load textures/bricks.png");
     }
     //endTexture.setSmooth(true);
 }
 
 int MapGenerator::getWidth() {
     return width;
 }
 
 int MapGenerator::getHeight() {
     return height;
 }
 
 
 void MapGenerator::loadPresetLevel() {
     switch(level) {
         case 1: // Easy level (18x18)
             gameMap[1][0] = Tile(1,0,Tile::TileType::Beginning);
             gameMap[1][1] = Tile(1,1,Tile::TileType::Path);
             gameMap[1][2] = Tile(1,2,Tile::TileType::Path);
             for (int i = 2; i < 17; i++) {
                 gameMap[i][2] = Tile(i,2,Tile::TileType::Path);
             }
             gameMap[16][3] = Tile(16,3,Tile::TileType::Path);
             gameMap[16][4] = Tile(16,4,Tile::TileType::Path);
             gameMap[16][5] = Tile(16,5,Tile::TileType::Path);
             for (int i = 16; i > 0; i--) {
                 gameMap[i][5] = Tile(i,5,Tile::TileType::Path);
             }
             gameMap[1][6] = Tile(1,6,Tile::TileType::Path);
             gameMap[1][7] = Tile(1,7,Tile::TileType::Path);
             gameMap[1][8] = Tile(1,8,Tile::TileType::Path);
             for (int i = 2; i < 17; i++) {
                 gameMap[i][8] = Tile(i,8,Tile::TileType::Path);
             }
             gameMap[16][9] = Tile(16,9,Tile::TileType::Path);
             gameMap[16][10] = Tile(16,10,Tile::TileType::Path);
             gameMap[16][11] = Tile(16,11,Tile::TileType::Path);
             for (int i = 16; i > 0; i--) {
                 gameMap[i][11] = Tile(i,11,Tile::TileType::Path);
             }
             gameMap[1][12] = Tile(1,12,Tile::TileType::Path);
             gameMap[1][13] = Tile(1,13,Tile::TileType::Path);
             gameMap[1][14] = Tile(1,14,Tile::TileType::Path);
             for (int i = 2; i < 17; i++) {
                 gameMap[i][14] = Tile(i,14,Tile::TileType::Path);
             }
             gameMap[16][15] = Tile(16,15,Tile::TileType::Path);
             gameMap[16][16] = Tile(16,16,Tile::TileType::Path);
             gameMap[width-2][height-1] = Tile(width-2,height-1,Tile::TileType::End);
             break;
             
         case 2: // Normal level (12x12)
             gameMap[1][0] = Tile(1,0,Tile::TileType::Beginning);
             gameMap[1][1] = Tile(1,1,Tile::TileType::Path);
             gameMap[1][2] = Tile(1,2,Tile::TileType::Path);
             for (int i = 2; i < 11; i++) {
                 gameMap[i][2] = Tile(i,2,Tile::TileType::Path);
             }
             gameMap[10][3] = Tile(10,3,Tile::TileType::Path);
             gameMap[10][4] = Tile(10,4,Tile::TileType::Path);
             gameMap[10][5] = Tile(10,5,Tile::TileType::Path);
             for (int i = 10; i > 0; i--) {
                 gameMap[i][5] = Tile(i,5,Tile::TileType::Path);
             }
             gameMap[1][6] = Tile(1,6,Tile::TileType::Path);
             gameMap[1][7] = Tile(1,7,Tile::TileType::Path);
             gameMap[1][8] = Tile(1,8,Tile::TileType::Path);
             for (int i = 2; i < 11; i++) {
                 gameMap[i][8] = Tile(i,8,Tile::TileType::Path);
             }
             gameMap[10][9] = Tile(10,9,Tile::TileType::Path);
             gameMap[10][10] = Tile(10,10,Tile::TileType::Path);
             gameMap[width-2][height-1] = Tile(width-2,height-1,Tile::TileType::End);
             break;
             
         case 3: // Hard level (8x8)
         gameMap[1][0] = Tile(1,0,Tile::TileType::Beginning);
         gameMap[1][1] = Tile(1,1,Tile::TileType::Path);
         gameMap[1][2] = Tile(1,2,Tile::TileType::Path);
         gameMap[1][3] = Tile(1,3,Tile::TileType::Path);
         gameMap[1][4] = Tile(1,4,Tile::TileType::Path);
         gameMap[1][5] = Tile(1,5,Tile::TileType::Path);
         gameMap[1][6] = Tile(1,6,Tile::TileType::Path);
         
         // Right turn to form the L shape
         gameMap[2][6] = Tile(2,6,Tile::TileType::Path);
         gameMap[3][6] = Tile(3,6,Tile::TileType::Path);
         gameMap[4][6] = Tile(4,6,Tile::TileType::Path);
         gameMap[5][6] = Tile(5,6,Tile::TileType::Path);
         gameMap[6][6] = Tile(6,6,Tile::TileType::Path);
         
         // End tile
         gameMap[width-2][height-1] = Tile(width-2,height-1,Tile::TileType::End);
         
         // Debug print to verify end tile position
         std::cout << "Hard level end tile position: (" << width-2 << "," << height-1 << ")" << std::endl;
         break;

         default:
             throw std::runtime_error("Invalid level selection");
     }
 }
 
 bool MapGenerator::isValidTile(int x, int y) const {
     return x >= 0 && x < width && y >= 0 && y < height;
 }
 
 std::vector<sf::Vector2f> MapGenerator::getWaypoints() const {
     std::vector<sf::Vector2f> waypoints;
     
     // First, find the beginning tile
     sf::Vector2i currentPos(-1, -1);
     
     // Find the starting position
     for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
             if (gameMap[y][x].getType() == Tile::TileType::Beginning) {
                 currentPos = sf::Vector2i(x, y);
                 waypoints.push_back(sf::Vector2f(
                     x * TILE_SIZE + TILE_SIZE / 2.f,
                     y * TILE_SIZE + TILE_SIZE / 2.f
                 ));
                 break;
             }
         }
         if (currentPos.x != -1) break;
     }
     
     if (currentPos.x == -1) {
         throw std::runtime_error("No beginning tile found in map!");
     }
 
     // Directions to check: right, down, left, up
     const std::vector<sf::Vector2i> directions = {
         {1, 0}, {0, 1}, {-1, 0}, {0, -1}
     };
     
     // Keep track of visited tiles to prevent loops
     std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
     visited[currentPos.y][currentPos.x] = true;
     
     // Follow the path
     bool foundEnd = false;
     while (!foundEnd) {
         bool foundNext = false;
         
         // Check all directions
         for (const auto& dir : directions) {
             sf::Vector2i nextPos = sf::Vector2i(currentPos.x + dir.x, currentPos.y + dir.y);
             
             // Check if position is within bounds
             if (isValidTile(nextPos.x, nextPos.y) && !visited[nextPos.y][nextPos.x]) {
                 Tile::TileType tileType = gameMap[nextPos.y][nextPos.x].getType();
                 
                 if (tileType == Tile::TileType::Path || tileType == Tile::TileType::End) {
                     // Add waypoint
                     waypoints.push_back(sf::Vector2f(
                         nextPos.x * TILE_SIZE + TILE_SIZE / 2.f,
                         nextPos.y * TILE_SIZE + TILE_SIZE / 2.f
                     ));
                     
                     visited[nextPos.y][nextPos.x] = true;
                     currentPos = nextPos;
                     foundNext = true;
                     
                     if (tileType == Tile::TileType::End) {
                         foundEnd = true;
                     }
                     break;
                 }
             }
         }
         
         if (!foundNext && !foundEnd) {
             throw std::runtime_error("Path is broken or does not reach the end!");
         }
     }
     
     return waypoints;
 }
 
 
 
 void MapGenerator::builder(sf::RenderWindow* win) {
     if (level == 4) {  
         // Interactive map creation: pass the pointer, not the object.
         interactiveMapCreation(win);
     } else {
         // Existing preset level logic
         switch(level) {
             case 1:
                 width = 18;
                 height = 18;
                 break;
             case 2:
                 width = 12;
                 height = 12;
                 break;
             case 3:
                 width = 8;
                 height = 8;
                 break;
             default:
                 throw std::runtime_error("Invalid level selection");
         }
         
         gameMap.resize(height, std::vector<Tile>(width));
 
         // Set x, y coordinates for each Tile
         for (int i = 0; i < height; i++) {
             for (int j = 0; j < width; j++) {
                 gameMap[i][j] = Tile(j, i, Tile::TileType::Scenery);
             }
         }
         
         loadPresetLevel();
     }
     notify();
 }
 
 void MapGenerator::interactiveMapCreation(sf::RenderWindow* mapWindow) {
    // Just add validation to ensure the values are in range
    if (width < 8 || width > 20) width = 10;
    if (height < 8 || height > 20) height = 10;

    // Initialize the instruction text
    if (!font.openFromFile("fonts/arial.ttf")) {
        throw std::runtime_error("Error loading font: 'fonts/arial.ttf'");
    }
    
    // Create the text object with the font
    sf::Text instructionText(font);
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(1.0f);
    currentCreationStep = MapCreationStep::PLACE_START;
    updateInstructionText(instructionText);

    // Resize game map with scenery tiles and initialize with default Scenery tiles.
    gameMap.resize(height, std::vector<Tile>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            gameMap[i][j] = Tile(j, i, Tile::TileType::Scenery);
        }
    }

    
    // Prepare tile shapes
    std::vector<std::vector<sf::RectangleShape>> tileShapes(height, std::vector<sf::RectangleShape>(width));
    
    // Initialize all tiles with the scenery texture
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            tileShapes[y][x].setTexture(&sceneryTexture);
            tileShapes[y][x].setFillColor(sf::Color(200, 200, 200, 128));
        }
    }

    

    // Initialize text with font reference
    sf::Text errorText(font);
    errorText.setCharacterSize(24);
    errorText.setFillColor(sf::Color::Red);
    errorText.setOutlineColor(sf::Color::Black);
    errorText.setOutlineThickness(1.0f);
    errorText.setString("");

    // Create restart button
    sf::RectangleShape restartButton(sf::Vector2f(100.f, 30.f));
    restartButton.setFillColor(sf::Color(200, 100, 100));
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setOutlineThickness(2.f);

    sf::Text restartText(font);
    restartText.setString("Restart");
    restartText.setCharacterSize(15);
    restartText.setFillColor(sf::Color::White);

    // Position the restart button in the top-right corner
    restartButton.setPosition(sf::Vector2f(
        mapWindow->getSize().x - restartButton.getSize().x - 10.f,
        10.f
    ));

    // Position the text on the button
    sf::FloatRect restartTextBounds = restartText.getLocalBounds();
    restartText.setPosition(sf::Vector2f(
        restartButton.getPosition().x + (restartButton.getSize().x - restartTextBounds.size.x) / 2.f,
        restartButton.getPosition().y + (restartButton.getSize().y - restartTextBounds.size.y) / 2.f
    ));
    
    // Variables to control error message display
    bool showErrorMessage = false;
    sf::Clock errorMessageClock;

    // Add variables for path validation status -- Right Click Feature to Validate
    bool isPathValid = false;
    bool showValidationMessage = false;
    sf::Clock validationMessageClock;
    sf::Text validationText(font);
    validationText.setCharacterSize(18);
    validationText.setFillColor(sf::Color::Green);
    validationText.setOutlineColor(sf::Color::Black);
    validationText.setOutlineThickness(1.5f);
    validationText.setString("Path is valid! Right-click to continue.");
    
    // Method to reposition UI elements when the window is resized
    auto repositionUI = [&](const sf::Vector2u& winSize) {
        // Recalculate tile dimensions based on the current window size
        float dynamicTileWidth = static_cast<float>(winSize.x) / static_cast<float>(width);
        float dynamicTileHeight = static_cast<float>(winSize.y) / static_cast<float>(height);

        // Update each tile shape's size and position
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                sf::RectangleShape& tile = tileShapes[y][x];
                tile.setSize(sf::Vector2f(dynamicTileWidth, dynamicTileHeight));
                tile.setPosition(sf::Vector2f(x * dynamicTileWidth, y * dynamicTileHeight));
            }
        }

        // Position the restart button in the top-right corner
        restartButton.setPosition(sf::Vector2f(
            winSize.x - restartButton.getSize().x - 10.f,
            10.f
        ));

        // Position the text on the button
        restartTextBounds = restartText.getLocalBounds();
        restartText.setPosition(sf::Vector2f(
            restartButton.getPosition().x + (restartButton.getSize().x - restartTextBounds.size.x) / 2.f,
            restartButton.getPosition().y + (restartButton.getSize().y - restartTextBounds.size.y) / 2.f
        ));

        // Position instruction text at the top of the window
        sf::FloatRect textBounds = instructionText.getLocalBounds();
        instructionText.setPosition(
            sf::Vector2f(
                (winSize.x - textBounds.size.x) / 2.0f,
                20.0f
            )
        );

        // Reposition error message if it's visible
        if (showErrorMessage) {
            textBounds = errorText.getLocalBounds();
            float xPos = (winSize.x - textBounds.size.x) / 2.0f;
            float yPos = winSize.y - textBounds.size.y - 40.0f;
            errorText.setPosition(sf::Vector2f(xPos, yPos));
        }
        // Position validation message if it's visible
        if (showValidationMessage) {
            sf::FloatRect textBounds = validationText.getLocalBounds();
            float xPos = (winSize.x - textBounds.size.x) / 2.0f;
            float yPos = winSize.y - textBounds.size.y - 80.0f;
            validationText.setPosition(sf::Vector2f(xPos, yPos));
        }
    };
    
    // Initialize UI positions
    repositionUI(mapWindow->getSize());
    
// Main loop for the map editor UI.
bool startSet = false, endSet = false;
sf::Vector2i startPos, endPos;
sf::Clock instructionFlashClock; // Add a clock for the flashing effect
bool showInstructions = true; // Toggle for instruction visibility

while (mapWindow->isOpen()) {
    // Update error message timer if showing
    if (showErrorMessage && errorMessageClock.getElapsedTime().asSeconds() > 3.0f) {
        showErrorMessage = false;
    }
    // Continuously check for valid path if we have start and end tiles placed
    if (startSet && endSet && !isPathValid) {
        isPathValid = validatePath();
        if (isPathValid) {
            // Show validation message when path becomes valid
            showValidationMessage = true;
            validationMessageClock.restart();
            
            // Position the validation message at the bottom center of the screen
            sf::FloatRect textBounds = validationText.getLocalBounds();
            sf::Vector2u winSize = mapWindow->getSize();
            float xPos = (winSize.x - textBounds.size.x) / 2.0f;
            float yPos = winSize.y - textBounds.size.y - 80.0f;
            validationText.setPosition(sf::Vector2f(xPos, yPos));
        }
    }
    
    // Add this block to handle the case when a previously valid path becomes invalid
    if (isPathValid) {
        // Recheck in case the path was modified and is no longer valid
        isPathValid = validatePath();
        if (!isPathValid) {
            showValidationMessage = false;
        }
    }
    
    // Update validation message display time
    if (showValidationMessage && validationMessageClock.getElapsedTime().asSeconds() > 10.0f) {
        // Hide the message after 10 seconds for cleanliness
        // But we'll continue to check and show it again if needed
        showValidationMessage = false;
    }
    
    // Handle the flashing effect for instructions
    if (instructionFlashClock.getElapsedTime().asSeconds() > 0.7f) { // Flash every 0.7 seconds
        showInstructions = !showInstructions;
        instructionFlashClock.restart();
        
        // Update instruction text
        sf::Color textColor = instructionText.getFillColor();
        // In SFML 3, color components are 0-255 values 
        textColor = sf::Color(textColor.r, textColor.g, textColor.b, 
            showInstructions ? 255 : 60);
        instructionText.setFillColor(textColor);
    }
    
    // Poll events 
    while (const std::optional<sf::Event> event = mapWindow->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mapWindow->close();
        }
        
        // Handle window resize event
        if (event->is<sf::Event::Resized>()) {
            if (const auto *resizeEvent = event->getIf<sf::Event::Resized>()) {
                // Update the view to match the new window size
                sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizeEvent->size.x, resizeEvent->size.y));
                mapWindow->setView(sf::View(visibleArea));
                
                // Reposition UI elements based on the new window size
                repositionUI(sf::Vector2u(resizeEvent->size.x, resizeEvent->size.y));
            }
        }

        if (event->is<sf::Event::MouseButtonPressed>()) {
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (restartButton.getGlobalBounds().contains(sf::Vector2f(mouseEvent->position))) {
                    // Reset the map
                    for (int i = 0; i < height; i++) {
                        for (int j = 0; j < width; j++) {
                            gameMap[i][j] = Tile(j, i, Tile::TileType::Scenery);
                            tileShapes[i][j].setTexture(&sceneryTexture);
                        }
                    }
                    
                    // Reset tracking variables
                    startSet = false;
                    endSet = false;
                    currentCreationStep = MapCreationStep::PLACE_START;
                    updateInstructionText(instructionText);
                    
                    // Clear any error messages and validation messages
                    showErrorMessage = false;
                    showValidationMessage = false;
                    isPathValid = false;
                    
                    continue; // Skip the rest of the event handling
                }
                
                // Get current window size for dynamic tile calculation
                sf::Vector2u winSize = mapWindow->getSize();
                float dynamicTileWidth = static_cast<float>(winSize.x) / static_cast<float>(width);
                float dynamicTileHeight = static_cast<float>(winSize.y) / static_cast<float>(height);
                
                // Get mouse position and convert it to tile indices using dynamic tile dimensions
                sf::Vector2i mousePos = mouseEvent->position;
                int xIndex = static_cast<int>(mousePos.x / dynamicTileWidth);
                int yIndex = static_cast<int>(mousePos.y / dynamicTileHeight);

                // Ensure indices are in range
                if (xIndex < 0 || xIndex >= width || yIndex < 0 || yIndex >= height)
                    continue;

                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    // Set start tile if not already set
                    if (!startSet) {
                        startPos = sf::Vector2i(xIndex, yIndex);
                        tileShapes[yIndex][xIndex].setTexture(&beginningTexture);
                        gameMap[yIndex][xIndex] = Tile(xIndex, yIndex, Tile::TileType::Beginning);
                        startSet = true;
                        // Update to next step
                        currentCreationStep = MapCreationStep::PLACE_END;
                        updateInstructionText(instructionText);
                        repositionUI(winSize); // Update instruction text position
                    }
                    // Set end tile if not set and different from start
                    else if (!endSet && sf::Vector2i(xIndex, yIndex) != startPos) {
                        sf::Vector2i potentialEndPos(xIndex, yIndex);
                        
                        // Check if the end position is adjacent to the start
                        if (arePositionsAdjacent(startPos, potentialEndPos)) {
                            // Show error message 
                            errorText.setString("Start and end tiles cannot be adjacent!");
                            
                            // Position the text at the bottom center of the screen
                            sf::FloatRect textBounds = errorText.getLocalBounds();
                            float xPos = (winSize.x - textBounds.size.x) / 2.0f;
                            float yPos = winSize.y - textBounds.size.y - 40.0f;
                            errorText.setPosition(sf::Vector2f(xPos, yPos));
                            
                            showErrorMessage = true;
                            errorMessageClock.restart();
                        } 
                        else {
                            // Accept the end position
                            endPos = potentialEndPos;
                            tileShapes[yIndex][xIndex].setTexture(&endTexture);
                            gameMap[yIndex][xIndex] = Tile(xIndex, yIndex, Tile::TileType::End);
                            endSet = true;
                            
                            // Update to next step
                            currentCreationStep = MapCreationStep::PLACE_PATH;
                            updateInstructionText(instructionText);
                            repositionUI(winSize); // Update instruction text position
                        }
                    }
                    else if (startSet && endSet) {
                        // Code to place path tiles
                        if (gameMap[yIndex][xIndex].getType() == Tile::TileType::Scenery) {
                            tileShapes[yIndex][xIndex].setTexture(&pathTexture);
                            gameMap[yIndex][xIndex] = Tile(xIndex, yIndex, Tile::TileType::Path);
                        }
                    }
                }
                else if (mouseEvent->button == sf::Mouse::Button::Right) {
                    // On right click, validate the path
                    if (startSet && endSet) {
                        if (validatePath()) {
                            std::cout << "Path is valid! Map saved." << std::endl;
                            mapWindow->close();
                        } else {
                            // Show error in UI
                            errorText.setString("Invalid path! Path must connect start to end.");
                            
                            // Position the text at the bottom center of the screen
                            sf::FloatRect textBounds = errorText.getLocalBounds();
                            float xPos = (winSize.x - textBounds.size.x) / 2.0f;
                            float yPos = winSize.y - textBounds.size.y - 40.0f;
                            errorText.setPosition(sf::Vector2f(xPos, yPos));
                            
                            showErrorMessage = true;
                            errorMessageClock.restart();
                            
                            std::cout << "Invalid path! Try again." << std::endl;
                        }
                    }
                }
            }
        }
    }

    // Render everything
    mapWindow->clear(sf::Color::Black);
    
    // Draw tile shapes
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mapWindow->draw(tileShapes[y][x]);
        }
    }
    
    // Draw error message if active
    if (showErrorMessage) {
        mapWindow->draw(errorText);
    }

    // Draw instruction text
    mapWindow->draw(instructionText);
    
    // Draw the restart button
    mapWindow->draw(restartButton);
    mapWindow->draw(restartText);
    // Draw validation message if active
    if (showValidationMessage) {
        mapWindow->draw(validationText);
    }

    mapWindow->display();
    notify();
}

    // After the loop, ensure that a valid map was created
    if (!startSet || !endSet || !validatePath()) {
        throw std::runtime_error("Invalid map creation");
    }
}
 
 bool MapGenerator::validatePath() const {
     try {
         auto waypoints = getWaypoints();
         return !waypoints.empty();
     } catch (const std::runtime_error&) {
         return false;
     }
 }

 bool MapGenerator::arePositionsAdjacent(const sf::Vector2i& pos1, const sf::Vector2i& pos2) const {
    // Check if the positions are next to each other (horizontally, vertically, or diagonally)
    int dx = std::abs(pos1.x - pos2.x);
    int dy = std::abs(pos1.y - pos2.y);
    return dx <= 1 && dy <= 1; // This is true if positions are adjacent
}


 void MapGenerator::updateInstructionText(sf::Text& text) {
    switch (currentCreationStep) {
        case MapCreationStep::PLACE_START:
            text.setString("Please place the start tile");
            break;
        case MapCreationStep::PLACE_END:
            text.setString("Please place the exit tile");
            break;
        case MapCreationStep::PLACE_PATH:
            text.setString("Place path tiles between the start tile and end tile.\nRight click to validate");
            break;
    }
}
 
 bool MapGenerator::findPath(sf::Vector2i start, sf::Vector2i end) const {
     // Simple breadth-first search to verify path exists
     vector<vector<bool>> visited(height, vector<bool>(width, false));
     queue<sf::Vector2i> q;
     q.push(start);
     visited[start.y][start.x] = true;
 
     vector<pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
 
     while (!q.empty()) {
         sf::Vector2i current = q.front();
         q.pop();
 
         if (current == end) return true;
 
         for (auto [dx, dy] : directions) {
             int nx = current.x + dx;
             int ny = current.y + dy;
 
             // Check bounds and valid movement
             if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                 !visited[ny][nx] && 
                 (gameMap[ny][nx].getType() == Tile::TileType::Path || 
                     gameMap[ny][nx].getType() == Tile::TileType::End)) {
                 q.push(sf::Vector2i(nx, ny));
                 visited[ny][nx] = true;
             }
         }
     }
 
     return false;
 }
 
 void MapGenerator::draw(sf::RenderWindow& window) {
     for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
             Tile::TileType tileType = gameMap[y][x].getType();
             
             // Get the appropriate texture based on tile type
             const sf::Texture& texture = [&]() -> const sf::Texture& {
                 switch (tileType) {
                     case Tile::TileType::Path:
                         return pathTexture;
                     case Tile::TileType::Beginning:
                         return beginningTexture;
                     case Tile::TileType::End:
                         return endTexture;
                     default:
                         return sceneryTexture;
                 }
             }();
             
             // Create sprite with the texture
             sf::Sprite tileSprite(texture);
 
             // Scale and position the sprite
             tileSprite.setScale(sf::Vector2f(
                 TILE_SIZE / static_cast<float>(texture.getSize().x),
                 TILE_SIZE / static_cast<float>(texture.getSize().y)
             ));
             
             tileSprite.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
             window.draw(tileSprite);
         }
     }
 }
 
 void MapGenerator::printer() {
     for (const auto &row : gameMap) {
         for (Tile cell : row) {
             switch(cell.getType()) {
                 case Tile::TileType::Path: 
                     cout << "|P|";
                     break;
                 case Tile::TileType::Beginning: 
                     cout << "|B|";
                     break;
                 case Tile::TileType::End: 
                     cout << "|E|";
                     break;
                 case Tile::TileType::Tower: 
                     cout << "|T|";
                     break;
                 default: 
                     cout << "|S|";
                     break;
             }
         }
         cout << endl;
     }
 }
 
 void MapGenerator::picker(char direction) {
     if (direction == 'N') {
         startPos = {0, rand() % (width - 2) + 1};
         endPos = {height - 1, rand() % (width - 2) + 1};
     } else if (direction == 'E') {
         startPos = {rand() % (height - 2) + 1, width - 1};
         endPos = {rand() % (height - 2) + 1, 0};
     } else if (direction == 'S') {
         endPos = {0, rand() % (width - 2) + 1};
         startPos = {height - 1, rand() % (width - 2) + 1};
     } else {
         endPos = {rand() % (height - 2) + 1, width - 1};
         startPos = {rand() % (height - 2) + 1, 0};
     }
     
     gameMap[startPos.first][startPos.second] = Tile(startPos.first, startPos.second, Tile::TileType::Beginning);
     gameMap[endPos.first][endPos.second] = Tile(endPos.first, endPos.second, Tile::TileType::End);
 }
 
 void MapGenerator::pathGenerator() {
     std::pair<int, int> currentPos = startPos;
     
     while (currentPos != endPos) {
         // Move towards end position
         if (currentPos.first < endPos.first) {
             currentPos.first++;
         } else if (currentPos.first > endPos.first) {
             currentPos.first--;
         }
         
         // Set current position to path if it's not start or end
         if (gameMap[currentPos.first][currentPos.second].getType() != Tile::TileType::End && 
             gameMap[currentPos.first][currentPos.second].getType() != Tile::TileType::Beginning) {
             gameMap[currentPos.first][currentPos.second] = 
                 Tile(currentPos.first, currentPos.second, Tile::TileType::Path);
         }
         
         // Move horizontally towards end
         if (currentPos.second < endPos.second) {
             currentPos.second++;
         } else if (currentPos.second > endPos.second) {
             currentPos.second--;
         }
         
         // Set current position to path if it's not start or end
         if (gameMap[currentPos.first][currentPos.second].getType() != Tile::TileType::End && 
             gameMap[currentPos.first][currentPos.second].getType() != Tile::TileType::Beginning) {
             gameMap[currentPos.first][currentPos.second] = 
                 Tile(currentPos.first, currentPos.second, Tile::TileType::Path);
         }
     }
 }