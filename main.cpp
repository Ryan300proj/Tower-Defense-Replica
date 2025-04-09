/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 *
 * main.cpp - Game initialization and main loop
 *
 * Purpose: Initializes SFML window, handles core game loop, error logging,
 * coordinates all game systems and demonstrates Observer, Strategy, Factory and Decorator pattern implementation.
 *
 * Dependencies: SFML 3.0, all game components
 */
/**
 * @file main.cpp
 * @brief Implements level selection, game initialization, and main loop
 */

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include "MapGenerator.h"
#include "Critter.hpp"
#include "CritterSpawner.hpp"
#include "TowerManager.hpp"
#include "TowerShop.hpp"
#include "TowerObserver.hpp"
#include "CritterObserver.hpp"
#include "MapObserver.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

// Error Handling Includes
#include <csignal>
#include <fstream>
#include <ctime>
#include <stdexcept>

// Global error logging function
void logError(const std::string &errorMessage)
{
    // Get current time
    std::time_t now = std::time(nullptr);
    char timestamp[100];
    struct std::tm timeStruct;
    
    #ifdef _WIN32
        // Windows version
        localtime_s(&timeStruct, &now);
    #else
        // Linux/Unix version
        localtime_r(&now, &timeStruct);
    #endif
    
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeStruct);

    // Open log file in append mode
    std::ofstream logFile("game_crash_log.txt", std::ios_base::app);
    if (logFile.is_open())
    {
        logFile << "[" << timestamp << "] " << errorMessage << std::endl;
        logFile.close();
    }

    // Also print to console for immediate feedback
    std::cerr << "[" << timestamp << "] " << errorMessage << std::endl;
}

// Signal handler for catching unexpected terminations
void signalHandler(int signum)
{
    std::string errorMsg;
    switch (signum)
    {
    case SIGSEGV:
        errorMsg = "Segmentation fault (SIGSEGV)";
        break;
    case SIGABRT:
        errorMsg = "Abnormal termination (SIGABRT)";
        break;
    case SIGFPE:
        errorMsg = "Floating-point exception (SIGFPE)";
        break;
    case SIGILL:
        errorMsg = "Illegal instruction (SIGILL)";
        break;
    default:
        errorMsg = "Unknown signal: " + std::to_string(signum);
    }

    logError("Game crashed: " + errorMsg);

    // Re-raise the signal to ensure standard crash behavior
    signal(signum, SIG_DFL);
    raise(signum);
}

// Allows user to choose with a UI between preset maps or custom map creation.
int showLevelSelectionScreen()
{
    sf::RenderWindow levelWindow(sf::VideoMode({400, 300}), "Tower Defense - Level Selection");

    // Load font
    sf::Font font;
    if (!font.openFromFile("fonts/arial.ttf"))
    {
        throw std::runtime_error("Error loading font: 'fonts/arial.ttf'");
    }

    // Create title text
    sf::Text titleText(font);
    titleText.setString("Select your level:");
    titleText.setFont(font);
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(1.5f);
    // Get the window size
    sf::Vector2u winSize = levelWindow.getSize();

    // Get the bounds of the text
    sf::FloatRect textBounds = titleText.getLocalBounds();

    // Center the text
    titleText.setPosition(sf::Vector2f(
        (winSize.x - textBounds.size.x) / 2.f,
        10.f));

    // Create level selection buttons
    sf::RectangleShape easyButton(sf::Vector2f(300.f, 50.f));
    easyButton.setPosition(sf::Vector2f(50.f, 50.f));
    easyButton.setFillColor(sf::Color(100, 200, 100));
    easyButton.setOutlineColor(sf::Color::White);
    easyButton.setOutlineThickness(2.f);

    // Level texts
    // Option 1: Easy
    sf::Text easyText(font);
    easyText.setString("1: Preset Easy (18x18)");
    easyText.setFont(font);
    easyText.setCharacterSize(15);
    easyText.setFillColor(sf::Color::Black);
    easyText.setOutlineColor(sf::Color::White);
    easyText.setOutlineThickness(1.0f);
    easyText.setPosition(sf::Vector2f(80.f, 65.f));

    sf::RectangleShape normalButton(sf::Vector2f(300.f, 50.f));
    normalButton.setPosition(sf::Vector2f(50.f, 110.f));
    normalButton.setFillColor(sf::Color(200, 200, 100));
    normalButton.setOutlineColor(sf::Color::White);
    normalButton.setOutlineThickness(2.f);

    // Option 2: Normal
    sf::Text normalText(font);
    normalText.setString("2: Preset Normal (12x12)");
    normalText.setCharacterSize(15);
    normalText.setFillColor(sf::Color::Black);
    normalText.setOutlineColor(sf::Color::White);
    normalText.setOutlineThickness(1.0f);
    normalText.setPosition(sf::Vector2f(80.f, 125.f));

    sf::RectangleShape hardButton(sf::Vector2f(300.f, 50.f));
    hardButton.setPosition(sf::Vector2f(50.f, 170.f));
    hardButton.setFillColor(sf::Color(200, 100, 100));
    hardButton.setOutlineColor(sf::Color::White);
    hardButton.setOutlineThickness(2.f);

    // Option 3: Hard
    sf::Text hardText(font);
    hardText.setString("3: Preset Hard (8x8)");
    hardText.setCharacterSize(15);
    hardText.setFillColor(sf::Color::Black);
    hardText.setOutlineColor(sf::Color::White);
    hardText.setOutlineThickness(1.0f);
    hardText.setPosition(sf::Vector2f(80.f, 185.f));

    sf::RectangleShape customButton(sf::Vector2f(300.f, 50.f));
    customButton.setPosition(sf::Vector2f(50.f, 230.f));
    customButton.setFillColor(sf::Color(100, 100, 200));
    customButton.setOutlineColor(sf::Color::White);
    customButton.setOutlineThickness(2.f);

    // Option 4: Custom
    sf::Text customText(font);
    customText.setString("4: Custom Map");
    customText.setCharacterSize(20);
    customText.setFillColor(sf::Color::Black);
    customText.setOutlineColor(sf::Color::White);
    customText.setOutlineThickness(1.5f);
    customText.setPosition(sf::Vector2f(80.f, 245.f));

    int selectedLevel = 0;

    while (levelWindow.isOpen() && selectedLevel == 0)
    {
        if (const auto event = levelWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                levelWindow.close();
                return 1; // Default to easy if window is closed
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouseEvent->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2i pixelPos = mouseEvent->position;
                        sf::Vector2f worldPos = levelWindow.mapPixelToCoords(pixelPos);

                        if (easyButton.getGlobalBounds().contains(worldPos))
                        {
                            selectedLevel = 1;
                            levelWindow.close();
                        }
                        else if (normalButton.getGlobalBounds().contains(worldPos))
                        {
                            selectedLevel = 2;
                            levelWindow.close();
                        }
                        else if (hardButton.getGlobalBounds().contains(worldPos))
                        {
                            selectedLevel = 3;
                            levelWindow.close();
                        }
                        else if (customButton.getGlobalBounds().contains(worldPos))
                        {
                            selectedLevel = 4;
                            levelWindow.close();
                        }
                    }
                }
            }
        }

        levelWindow.clear(sf::Color(30, 30, 30));

        levelWindow.draw(titleText);
        levelWindow.draw(easyButton);
        levelWindow.draw(easyText);
        levelWindow.draw(normalButton);
        levelWindow.draw(normalText);
        levelWindow.draw(hardButton);
        levelWindow.draw(hardText);
        levelWindow.draw(customButton);
        levelWindow.draw(customText);

        levelWindow.display();
    }

    return selectedLevel;
}

// Custom Map Dimensions
std::pair<int, int> getCustomMapDimensions()
{
    sf::RenderWindow dimWindow(sf::VideoMode({400, 200}), "Enter Map Dimensions");

    sf::Font font;
    if (!font.openFromFile("fonts/arial.ttf"))
    {
        throw std::runtime_error("Error loading font: 'fonts/arial.ttf'");
    }

    // Title text
    sf::Text titleText(font);
    titleText.setString("Enter Map Dimensions (8-20)");
    titleText.setCharacterSize(15);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(80.f, 10.f));

    // Width input
    sf::RectangleShape widthBox(sf::Vector2f(100.f, 40.f));
    widthBox.setPosition(sf::Vector2f(80.f, 50.f));
    widthBox.setFillColor(sf::Color(50, 50, 50));
    widthBox.setOutlineColor(sf::Color::White);
    widthBox.setOutlineThickness(2.f);

    sf::Text widthLabel(font);
    widthLabel.setString("Width:");
    widthLabel.setCharacterSize(12);
    widthLabel.setFillColor(sf::Color::White);
    widthLabel.setPosition(sf::Vector2f(20.f, 60.f));

    sf::Text widthInput(font);
    widthInput.setString("10"); // Default value
    widthInput.setCharacterSize(12);
    widthInput.setFillColor(sf::Color::White);
    widthInput.setPosition(sf::Vector2f(widthBox.getPosition().x + 10.f, widthBox.getPosition().y + 10.f));

    // Height input
    sf::RectangleShape heightBox(sf::Vector2f(100.f, 40.f));
    heightBox.setPosition(sf::Vector2f(80.f, 100.f));
    heightBox.setFillColor(sf::Color(50, 50, 50));
    heightBox.setOutlineColor(sf::Color::White);
    heightBox.setOutlineThickness(2.f);

    sf::Text heightLabel(font);
    heightLabel.setString("Height:");
    heightLabel.setCharacterSize(12);
    heightLabel.setFillColor(sf::Color::White);
    heightLabel.setPosition(sf::Vector2f(20.f, 110.f));

    sf::Text heightInput(font);
    heightInput.setString("10"); // Default value
    heightInput.setCharacterSize(12);
    heightInput.setFillColor(sf::Color::White);
    heightInput.setPosition(sf::Vector2f(heightBox.getPosition().x + 10.f, heightBox.getPosition().y + 10.f));

    // Create confirm button
    sf::RectangleShape confirmButton(sf::Vector2f(150.f, 40.f));
    confirmButton.setPosition(sf::Vector2f(220.f, 75.f));
    confirmButton.setFillColor(sf::Color::Green);
    confirmButton.setOutlineColor(sf::Color::White);
    confirmButton.setOutlineThickness(2.f);

    sf::Text confirmText(font);
    confirmText.setString("Confirm");
    confirmText.setCharacterSize(15);
    confirmText.setFillColor(sf::Color::Black);
    confirmText.setPosition(sf::Vector2f(
        confirmButton.getPosition().x + (confirmButton.getSize().x - confirmText.getLocalBounds().size.x) / 2.f,
        confirmButton.getPosition().y + (confirmButton.getSize().y - confirmText.getLocalBounds().size.y) / 2.f));

    bool widthSelected = true; // Track which input box is selected
    bool heightSelected = false;
    int width = 10, height = 10; // Default values

    while (dimWindow.isOpen())
    {
        if (const auto event = dimWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                dimWindow.close();
                return {10, 10}; // Default values if window is closed
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouseEvent->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2i pixelPos = mouseEvent->position;
                        sf::Vector2f worldPos = dimWindow.mapPixelToCoords(pixelPos);

                        if (widthBox.getGlobalBounds().contains(worldPos))
                        {
                            widthSelected = true;
                            heightSelected = false;
                            widthBox.setOutlineColor(sf::Color::Cyan);
                            heightBox.setOutlineColor(sf::Color::White);
                        }
                        else if (heightBox.getGlobalBounds().contains(worldPos))
                        {
                            widthSelected = false;
                            heightSelected = true;
                            widthBox.setOutlineColor(sf::Color::White);
                            heightBox.setOutlineColor(sf::Color::Cyan);
                        }
                        else if (confirmButton.getGlobalBounds().contains(worldPos))
                        {
                            // Validate inputs are in range
                            if (width >= 8 && width <= 20 && height >= 8 && height <= 20)
                            {
                                dimWindow.close();
                                return {width, height};
                            }
                        }
                    }
                }
            }

            if (event->is<sf::Event::KeyPressed>())
            {
                if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
                {
                    // Handle number keys
                    int value = -1;
                    switch (keyEvent->code)
                    {
                    case sf::Keyboard::Key::Num0:
                        value = 0;
                        break;
                    case sf::Keyboard::Key::Num1:
                        value = 1;
                        break;
                    case sf::Keyboard::Key::Num2:
                        value = 2;
                        break;
                    case sf::Keyboard::Key::Num3:
                        value = 3;
                        break;
                    case sf::Keyboard::Key::Num4:
                        value = 4;
                        break;
                    case sf::Keyboard::Key::Num5:
                        value = 5;
                        break;
                    case sf::Keyboard::Key::Num6:
                        value = 6;
                        break;
                    case sf::Keyboard::Key::Num7:
                        value = 7;
                        break;
                    case sf::Keyboard::Key::Num8:
                        value = 8;
                        break;
                    case sf::Keyboard::Key::Num9:
                        value = 9;
                        break;
                    case sf::Keyboard::Key::Numpad0:
                        value = 0;
                        break;
                    case sf::Keyboard::Key::Numpad1:
                        value = 1;
                        break;
                    case sf::Keyboard::Key::Numpad2:
                        value = 2;
                        break;
                    case sf::Keyboard::Key::Numpad3:
                        value = 3;
                        break;
                    case sf::Keyboard::Key::Numpad4:
                        value = 4;
                        break;
                    case sf::Keyboard::Key::Numpad5:
                        value = 5;
                        break;
                    case sf::Keyboard::Key::Numpad6:
                        value = 6;
                        break;
                    case sf::Keyboard::Key::Numpad7:
                        value = 7;
                        break;
                    case sf::Keyboard::Key::Numpad8:
                        value = 8;
                        break;
                    case sf::Keyboard::Key::Numpad9:
                        value = 9;
                        break;
                    case sf::Keyboard::Key::Backspace:
                        value = -2;
                        break;
                    case sf::Keyboard::Key::Tab:
                        // Toggle between width and height
                        widthSelected = !widthSelected;
                        heightSelected = !heightSelected;
                        widthBox.setOutlineColor(widthSelected ? sf::Color::Cyan : sf::Color::White);
                        heightBox.setOutlineColor(heightSelected ? sf::Color::Cyan : sf::Color::White);
                        break;
                    case sf::Keyboard::Key::Enter:
                        // Validate inputs are in range
                        if (width >= 8 && width <= 20 && height >= 8 && height <= 20)
                        {
                            dimWindow.close();
                            return {width, height};
                        }
                        break;
                    default:
                        break;
                    }

                    // Update input text
                    if (value >= 0)
                    {
                        if (widthSelected)
                        {
                            std::string currentStr = widthInput.getString();
                            if (currentStr == "10" || currentStr == "0")
                                currentStr = ""; // Clear default or replace single "0" value

                            if (currentStr.length() < 2)
                            { // Limit to 2 digits
                                currentStr += std::to_string(value);
                                widthInput.setString(currentStr);
                                width = std::stoi(currentStr);
                            }
                        }
                        else if (heightSelected)
                        {
                            std::string currentStr = heightInput.getString();
                            if (currentStr == "10" || currentStr == "0")
                                currentStr = ""; // Clear default or replace single "0" value

                            if (currentStr.length() < 2)
                            { // Limit to 2 digits
                                currentStr += std::to_string(value);
                                heightInput.setString(currentStr);
                                height = std::stoi(currentStr);
                            }
                        }
                    }
                    else if (value == -2)
                    { // Backspace
                        if (widthSelected)
                        {
                            std::string currentStr = widthInput.getString();
                            if (!currentStr.empty())
                            {
                                currentStr.pop_back();
                                if (currentStr.empty())
                                    currentStr = "0";
                                widthInput.setString(currentStr);
                                width = std::stoi(currentStr);
                            }
                        }
                        else if (heightSelected)
                        {
                            std::string currentStr = heightInput.getString();
                            if (!currentStr.empty())
                            {
                                currentStr.pop_back();
                                if (currentStr.empty())
                                    currentStr = "0";
                                heightInput.setString(currentStr);
                                height = std::stoi(currentStr);
                            }
                        }
                    }
                }
            }
        }

        dimWindow.clear(sf::Color(30, 30, 30));

        dimWindow.draw(titleText);
        dimWindow.draw(widthBox);
        dimWindow.draw(widthLabel);
        dimWindow.draw(widthInput);
        dimWindow.draw(heightBox);
        dimWindow.draw(heightLabel);
        dimWindow.draw(heightInput);
        dimWindow.draw(confirmButton);
        dimWindow.draw(confirmText);

        dimWindow.display();
    }

    return {width, height};
}

// Main function

int main()
{
    // Register signal handlers
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);

    try
    {

        std::cout << "Initializing Tower Defense Game..." << std::endl;

        // Show level selection screen first
        int selectedLevel = showLevelSelectionScreen();

        // Get map dimensions if custom level is selected
        int mapWidth = 0, mapHeight = 0;
        if (selectedLevel == 4)
        {
            auto dimensions = getCustomMapDimensions();
            mapWidth = dimensions.first;
            mapHeight = dimensions.second;
        }

        // Create a separate window for the map editor (or reuse an existing one)
        sf::RenderWindow mapEditorWindow(sf::VideoMode({800, 600}), "Map Editor");

        // Pass both level and dimensions to MapGenerator
        MapGenerator mapGen(selectedLevel, mapWidth, mapHeight);
        mapGen.builder(&mapEditorWindow);
        std::vector<sf::Vector2f> waypoints = mapGen.getWaypoints();
        // Check if waypoints are valid
        if (waypoints.empty())
        {
            throw std::runtime_error("ERROR: No waypoints generated! Exiting game.");
        }

        // Create SFML Window
        sf::Vector2u windowSize = mapGen.getRequiredWindowSize();
        sf::RenderWindow window(sf::VideoMode(windowSize), "Tower Defense");

        // Create critter spawner
        CritterSpawner spawner(waypoints);

        // Create tower manager and shop
        TowerManager towerManager(mapGen, spawner);
        TowerShop shop(towerManager, window);
    
        // Create Observers for MapGenerator, TowerManager, CritterSpawner
        MapObserver mObs(&mapGen, &window);
        CritterObserver cObs(&spawner, &window);
        TowerObserver tObs(&towerManager, &window);

        // Load font
        sf::Font font;
        if (!font.openFromFile("fonts/arial.ttf"))
        {
            throw std::runtime_error("Error loading font: 'fonts/arial.ttf'");
        }

       
        // Create and configure wave text
        sf::Text waveText(font);
        waveText.setCharacterSize(20);
        waveText.setFillColor(sf::Color::Black);
        waveText.setOutlineColor(sf::Color::White);
        waveText.setOutlineThickness(2.0f);
        waveText.setPosition(sf::Vector2f(10.f, 10.f));

        // Tooltip text for critter inspection

        bool showCritterTooltip = false;
        sf::Text critterTooltipText(font); // Pass the font reference here
        critterTooltipText.setCharacterSize(14);
        critterTooltipText.setFillColor(sf::Color::White);
        critterTooltipText.setOutlineColor(sf::Color::Black);
        critterTooltipText.setOutlineThickness(1.0f);
        sf::RectangleShape tooltipBackground;
        tooltipBackground.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent dark background
        tooltipBackground.setOutlineColor(sf::Color::White);
        tooltipBackground.setOutlineThickness(1.0f);

        // Tooltip text for tower inspection
        bool showTowerTooltip = false;
        sf::Text towerTooltipText(font);
        towerTooltipText.setCharacterSize(14);
        towerTooltipText.setFillColor(sf::Color::White);
        towerTooltipText.setOutlineColor(sf::Color::Black);
        towerTooltipText.setOutlineThickness(1.0f);
        sf::RectangleShape towerTooltipBackground;
        towerTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent dark background
        towerTooltipBackground.setOutlineColor(sf::Color::White);
        towerTooltipBackground.setOutlineThickness(1.0f);

        // Create and configure currency text
        sf::Text currencyText(font);
        currencyText.setCharacterSize(20);
        currencyText.setFillColor(sf::Color::Yellow);
        currencyText.setOutlineColor(sf::Color::Black);
        currencyText.setOutlineThickness(1.5f);
        currencyText.setPosition(sf::Vector2f(490.f, 10.f));

        // Create start round button
        sf::RectangleShape startButton(sf::Vector2f(150.f, 50.f));

        // Create help button
        sf::RectangleShape helpButton(sf::Vector2f(120.f, 40.f));
        helpButton.setFillColor(sf::Color(100, 150, 200));
        helpButton.setOutlineColor(sf::Color::White);
        helpButton.setOutlineThickness(2.f);

        // Help button text
        sf::Text helpButtonText(font);
        helpButtonText.setString("Controls");
        helpButtonText.setCharacterSize(18);
        helpButtonText.setFillColor(sf::Color::White);
        helpButtonText.setOutlineColor(sf::Color::Black);
        helpButtonText.setOutlineThickness(1.5f);

        // Help dialog
        bool showHelpDialog = false;
        sf::RectangleShape helpDialog(sf::Vector2f(400.f, 350.f));
        helpDialog.setFillColor(sf::Color(50, 50, 50, 230));
        helpDialog.setOutlineColor(sf::Color::White);
        helpDialog.setOutlineThickness(2.f);

        // Help dialog title
        sf::Text helpDialogTitle(font);
        helpDialogTitle.setString("Game Controls");
        helpDialogTitle.setCharacterSize(22);
        helpDialogTitle.setFillColor(sf::Color::White);
        helpDialogTitle.setOutlineColor(sf::Color::Black);
        helpDialogTitle.setOutlineThickness(1.0f);

        // Help dialog content
        sf::Text helpDialogContent(font);
        helpDialogContent.setString(
            "Left Click: Place selected tower\n\n"
            "Right Click: Upgrade tower\n\n"
            "Shift + Left Click: Sell tower\n\n"
            "Alt + Mouse Over: Inspect object\n\n"
            "Shop: Buy new towers\n\n"
            "Start Wave: Begin next enemy wave\n\n"
            "R: Restart (Once prompted)"
        );
        helpDialogContent.setCharacterSize(16);
        helpDialogContent.setFillColor(sf::Color::White);
        helpDialogContent.setOutlineColor(sf::Color::Black);
        helpDialogContent.setOutlineThickness(2.0f); // Thick outline

        // Help dialog close button
        sf::RectangleShape helpCloseButton(sf::Vector2f(30.f, 30.f));
        helpCloseButton.setFillColor(sf::Color::Red);

        sf::Text helpCloseButtonText(font);
        helpCloseButtonText.setString("X");
        helpCloseButtonText.setCharacterSize(20);
        helpCloseButtonText.setFillColor(sf::Color::White);




        // Position based on window size, keeping 10px padding from the right edge
        startButton.setPosition(sf::Vector2f(
            windowSize.x - startButton.getSize().x - 10.f, // 10px from right edge
            10.f                                           // 10px from top
            ));

        startButton.setFillColor(sf::Color::Green);
        startButton.setOutlineColor(sf::Color::Black);
        startButton.setOutlineThickness(2.f);

        // Position help button at bottom-left corner
        helpButton.setPosition(sf::Vector2f(
            10.f, // 10px from left edge
            windowSize.y - helpButton.getSize().y - 10.f // 10px from bottom
        ));

        // Position help button text centered on the button
        sf::FloatRect helpTextBounds = helpButtonText.getLocalBounds();
        helpButtonText.setPosition(sf::Vector2f(
            helpButton.getPosition().x + (helpButton.getSize().x - helpTextBounds.size.x) / 2.f,
            helpButton.getPosition().y + (helpButton.getSize().y - helpTextBounds.size.y) / 2.f
        ));

        // Position help dialog (will be centered when shown)
        helpDialog.setPosition(sf::Vector2f(
            (windowSize.x - helpDialog.getSize().x) / 2.f,
            (windowSize.y - helpDialog.getSize().y) / 2.f
        ));

        // Position help dialog title
        helpDialogTitle.setPosition(sf::Vector2f(
            helpDialog.getPosition().x + 20.f,
            helpDialog.getPosition().y + 20.f
        ));

        // Position help dialog content
        helpDialogContent.setPosition(sf::Vector2f(
            helpDialog.getPosition().x + 20.f,
            helpDialog.getPosition().y + 60.f
        ));

        // Position help close button
        helpCloseButton.setPosition(sf::Vector2f(
            helpDialog.getPosition().x + helpDialog.getSize().x - 40.f,
            helpDialog.getPosition().y + 10.f
        ));

        // Position help close button text
        helpCloseButtonText.setPosition(sf::Vector2f(
            helpCloseButton.getPosition().x + 8.f,
            helpCloseButton.getPosition().y + 2.f
        ));

        // Button text
        sf::Text buttonText(font);
        buttonText.setString("Start Wave");
        buttonText.setCharacterSize(18);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setOutlineColor(sf::Color::Black);
        buttonText.setOutlineThickness(2.0f);
        buttonText.setPosition(sf::Vector2f(475.f, 55.f));

        // Position the text centered within the button
        buttonText.setPosition(sf::Vector2f(
            startButton.getPosition().x + (startButton.getSize().x - buttonText.getLocalBounds().size.x) / 2.f,
            startButton.getPosition().y + (startButton.getSize().y - buttonText.getLocalBounds().size.y) / 2.f));



            shop.setOnUIRepositionedCallback([&](const sf::Vector2f& viewSize) {
                // Reposition start button
                startButton.setPosition(sf::Vector2f(
                    viewSize.x - startButton.getSize().x - 10.f,
                    10.f
                ));
                
                // Reposition button text
                sf::FloatRect buttonBounds = buttonText.getLocalBounds();
                buttonText.setPosition(sf::Vector2f(
                    startButton.getPosition().x + (startButton.getSize().x - buttonBounds.size.x) / 2.f,
                    startButton.getPosition().y + (startButton.getSize().y - buttonBounds.size.y) / 2.f
                ));
                
              
                
                // Update currency text
                currencyText.setString("Gold: $" + std::to_string(spawner.getPlayerCurrency()));
                sf::FloatRect textBounds = currencyText.getLocalBounds();
                currencyText.setPosition(sf::Vector2f(
                    (viewSize.x - textBounds.size.x) / 2.f,
                    10.f
                ));
                // Reposition help button
                helpButton.setPosition(sf::Vector2f(
                    10.f,
                    viewSize.y - helpButton.getSize().y - 10.f
                ));
                
                // Reposition help button text
                sf::FloatRect helpTextBounds = helpButtonText.getLocalBounds();
                helpButtonText.setPosition(sf::Vector2f(
                    helpButton.getPosition().x + (helpButton.getSize().x - helpTextBounds.size.x) / 2.f,
                    helpButton.getPosition().y + (helpButton.getSize().y - helpTextBounds.size.y) / 2.f
                ));
                
                // Reposition help dialog if visible
                if (showHelpDialog) {
                    helpDialog.setPosition(sf::Vector2f(
                        (viewSize.x - helpDialog.getSize().x) / 2.f,
                        (viewSize.y - helpDialog.getSize().y) / 2.f
                    ));
                    
                    // Update help dialog content positions
                    helpDialogTitle.setPosition(sf::Vector2f(
                        helpDialog.getPosition().x + 20.f,
                        helpDialog.getPosition().y + 20.f
                    ));
                    
                    helpDialogContent.setPosition(sf::Vector2f(
                        helpDialog.getPosition().x + 20.f,
                        helpDialog.getPosition().y + 60.f
                    ));
                    
                    helpCloseButton.setPosition(sf::Vector2f(
                        helpDialog.getPosition().x + helpDialog.getSize().x - 40.f,
                        helpDialog.getPosition().y + 10.f
                    ));
                    
                    helpCloseButtonText.setPosition(sf::Vector2f(
                        helpCloseButton.getPosition().x + 8.f,
                        helpCloseButton.getPosition().y + 2.f
                    ));
                }
            });
            
        sf::Clock clock;

        // Create and configure lives text
        sf::Text livesText(font);
        livesText.setCharacterSize(20);
        livesText.setFillColor(sf::Color::Red);  // Red to emphasize importance
        livesText.setOutlineColor(sf::Color::White);
        livesText.setOutlineThickness(2.0f);
        livesText.setPosition(sf::Vector2f(40.f, 40.f));  // Position below wave text

        // Reposition other UI elements
        waveText.setPosition(sf::Vector2f(10.f, 10.f));


        // Add heart icon for lives 
        sf::Texture heartTexture;
        sf::Sprite heartSprite(heartTexture);

        // Load the heart texture
        if (!heartTexture.loadFromFile("textures/hpicon.png")) {
            throw std::runtime_error("Error loading heart icon: 'textures/hpicon.png'");
        }
        heartSprite = sf::Sprite(heartTexture);  // Create the sprite with the texture
        heartSprite.setScale(sf::Vector2f(0.5f, 0.5f)); // Adjust scale as needed
        heartSprite.setPosition(sf::Vector2f(10.f, 40.f));  // Position left of the lives text

        // Game over text (only displayed when game is over)
        sf::Text gameOverText(font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(72);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineColor(sf::Color::Black);
        gameOverText.setOutlineThickness(3.0f);

        // Restart text
        sf::Text restartText(font);
        restartText.setString("Press R to Restart");
        restartText.setCharacterSize(24);
        restartText.setFillColor(sf::Color::White);
        restartText.setOutlineColor(sf::Color::Black);
        restartText.setOutlineThickness(1.5f);    

        // Victory screen text (only displayed when game is complete)
        sf::Text victoryText(font);
        victoryText.setString("VICTORY!");
        victoryText.setCharacterSize(72);
        victoryText.setFillColor(sf::Color::Green);  // Use green for victory
        victoryText.setOutlineColor(sf::Color::Black);
        victoryText.setOutlineThickness(3.0f);

        // Victory restart text
        sf::Text victoryRestartText(font);
        victoryRestartText.setString("Press R to Play Again");
        victoryRestartText.setCharacterSize(24);
        victoryRestartText.setFillColor(sf::Color::White);
        victoryRestartText.setOutlineColor(sf::Color::Black);
        victoryRestartText.setOutlineThickness(1.5f);


    
       

        // Game loop
        while (window.isOpen())
        {
            try
            {
                float deltaTime = clock.restart().asSeconds();

                // Process window events
                if (const auto event = window.pollEvent())
                {
                    if (event->is<sf::Event::Closed>())
                    {
                        std::cout << "Window closing..." << std::endl;
                        window.close();
                    }
                    // Add game restart logic
                    else if (spawner.isGameOver() && event->is<sf::Event::KeyPressed>())
                    {
                        if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
                        {
                            if (keyEvent->code == sf::Keyboard::Key::R) {
                                // Reset the game
                                spawner.reset();
                                towerManager.reset();
                                shop.reset();
                            }
                        }
                    }
                    // Game restart logic for both game over and victory conditions
                    else if ((spawner.isGameOver() || spawner.isGameComplete()) && event->is<sf::Event::KeyPressed>())
                    {
                        if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
                        {
                            if (keyEvent->code == sf::Keyboard::Key::R) {
                                // Reset the game
                                spawner.reset();
                                towerManager.reset();
                                shop.reset();
                            }
                        }
                    }
                    // Inside the event handling loop
                    else if (event->is<sf::Event::MouseButtonPressed>())
                    {
                        if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
                        {
                            if (mouseEvent->button == sf::Mouse::Button::Left)
                            {
                                sf::Vector2i pixelPos = mouseEvent->position;
                                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                                // Check if Shift key is held down
                                bool isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
                                                      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);

                                // Handle help dialog
                                if (showHelpDialog) {
                                    // Check if close button is clicked
                                    if (helpCloseButton.getGlobalBounds().contains(worldPos)) {
                                        showHelpDialog = false;
                                    }
                                    continue; // Skip further processing if help dialog is open
                                }
                                
                                // Check if help button is clicked
                                if (helpButton.getGlobalBounds().contains(worldPos)) {
                                    showHelpDialog = true;
                                    continue; // Skip further processing when opening help
                                }

                                // If shop is open, handle shop interaction
                                if (shop.isShopOpen() || shop.isMouseOverShopButton(worldPos))
                                {
                                    shop.handleClick(worldPos);
                                }
                                else if (isShiftPressed)
                                {
                                    // Attempt to sell tower
                                    if (towerManager.trySellTower(worldPos))
                                    {
                                        // Optional: Add sound or visual feedback
                                    }
                                }
                                else if (shop.hasSelectedTower())
                                {
                                    // Existing tower placement logic
                                    if (towerManager.tryPlaceTower(worldPos, shop.getSelectedTowerType()))
                                    {
                                        shop.clearSelection();
                                    }
                                }
                                // Existing start wave button check
                                else if (startButton.getGlobalBounds().contains(worldPos))
                                {
                                    if (spawner.canStartNewWave())
                                    {
                                        spawner.startNextWave();
                                    }
                                }
                            }
                            else if (mouseEvent->button == sf::Mouse::Button::Right)
                            {
                                sf::Vector2f worldPos = window.mapPixelToCoords(mouseEvent->position);
                                towerManager.tryUpgradeTower(worldPos);
                            }
                        }
                    }
                    else if (event->is<sf::Event::Resized>())
                    {
                        if (const auto *resizeEvent = event->getIf<sf::Event::Resized>())
                        {
                            float windowWidth = static_cast<float>(resizeEvent->size.x);
                            float windowHeight = static_cast<float>(resizeEvent->size.y);
                    
                            // Calculate the actual game area size
                            const float UI_SPACE = 100.f;
                            float gameAreaWidth = static_cast<float>(mapGen.getWidth() * 64);
                            float gameAreaHeight = static_cast<float>(mapGen.getHeight() * 64 + UI_SPACE);
                            
                            // Create a view that shows the entire game area
                            sf::View gameView;
                            gameView.setCenter(sf::Vector2f(gameAreaWidth / 2.f, gameAreaHeight / 2.f));
                            gameView.setSize(sf::Vector2f(gameAreaWidth, gameAreaHeight));
                            
                            // Calculate how to fit this view in the window while maintaining aspect ratio
                            float gameAspectRatio = gameAreaWidth / gameAreaHeight;
                            float windowAspectRatio = windowWidth / windowHeight;
                            
                            // Adjust the viewport to maintain the correct aspect ratio
                            if (windowAspectRatio > gameAspectRatio) {
                                // Window is wider than needed - center horizontally
                                float viewportWidth = gameAspectRatio / windowAspectRatio;
                                float viewportLeft = (1.f - viewportWidth) / 2.f;
                                gameView.setViewport(sf::FloatRect(sf::Vector2f(viewportLeft, 0.f), sf::Vector2f(viewportWidth, 1.f)));
                            } else {
                                // Window is taller than needed - center vertically
                                float viewportHeight = windowAspectRatio / gameAspectRatio;
                                float viewportTop = (1.f - viewportHeight) / 2.f;
                                gameView.setViewport(sf::FloatRect(sf::Vector2f(0.f, viewportTop), sf::Vector2f(1.f, viewportHeight)));
                            }
                            
                            // Apply the view
                            window.setView(gameView);
                            
                            // Update UI positions
                            shop.updateUIPositions(sf::Vector2f(gameAreaWidth, gameAreaHeight));
                        }
                    }

                    // Check for mouse movement while Alt key is pressed (for critter inspection)
                    // Check for mouse movement while Alt key is pressed (for entity inspection)
                    else if (event->is<sf::Event::MouseMoved>())
                    {
                        if (const auto *moveEvent = event->getIf<sf::Event::MouseMoved>())
                        {
                            sf::Vector2i pixelPos = moveEvent->position;
                            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                            // Only show tooltips when Alt key is pressed
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) ||
                                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt))
                            {

                                // First check for tower under mouse
                                Tower *hoveredTower = towerManager.getTowerUnderMouse(worldPos);

                                if (hoveredTower)
                                {
                                    
                                    // Format the tower information as a string
                                    std::string tooltipString = "Tower Info:\n";
                                    
                                    // Add type info based on tower type
                                    std::string type = "Basic";
                                    if (hoveredTower->getTowerType() == Tower::Type::AREA)
                                    {
                                        type = "Area";
                                        tooltipString += "Type: " + type + "\n";
                                        tooltipString += "Level: " + std::to_string(hoveredTower->getLevel()) + "\n";
                                        tooltipString += "Power: " + std::to_string(static_cast<int>(hoveredTower->getPower())) + "\n";
                                        tooltipString += "Range: " + std::to_string(static_cast<int>(hoveredTower->getRange())) + "\n";
                                        std::stringstream fireRateStream;
                                        fireRateStream << std::fixed << std::setprecision(2) << hoveredTower->getFireRate();
                                        tooltipString += "Fire Rate: " + fireRateStream.str() + "\n";
                                        tooltipString += "AoE Radius: " + std::to_string(static_cast<int>(hoveredTower->getAoeRadius())) + "\n";
                                        float mainDamage = hoveredTower->getPower();
                                        float splashDamage = mainDamage * 0.5f;  // 50% of main damage
                                        tooltipString += "Splash Damage: " + std::to_string(static_cast<int>(splashDamage)) + " (" + 
                                                         std::to_string(50) + "% of main damage)\n";
                                        // Add upgrade cost info based on current level
                                        if (hoveredTower->canUpgrade()) {
                                            if (hoveredTower->getLevel() == 1) {
                                                tooltipString += "Upgrade Cost: $100\n";
                                            } else if (hoveredTower->getLevel() == 2) {
                                                tooltipString += "Upgrade Cost: $260\n";
                                            }
                                        }
                                    }
                                    else if (hoveredTower->getTowerType() == Tower::Type::SPECIAL)
                                    {
                                        type = "Special";
                                        tooltipString += "Type: " + type + "\n";
                                        tooltipString += "Level: " + std::to_string(hoveredTower->getLevel()) + "\n";
                                        tooltipString += "Power: " + std::to_string(static_cast<int>(hoveredTower->getPower())) + "\n";
                                        tooltipString += "Range: " + std::to_string(static_cast<int>(hoveredTower->getRange())) + "\n";
                                        std::stringstream fireRateStream;
                                        fireRateStream << std::fixed << std::setprecision(2) << hoveredTower->getFireRate();
                                        tooltipString += "Fire Rate: " + fireRateStream.str() + "\n";
                                        
                                        float baseDamage = hoveredTower->getPower();
                                        float multiplier = hoveredTower->getSpecialMultiplier();
                                        int totalDamage = static_cast<int>(baseDamage * multiplier);
                                        tooltipString += "Special: " + std::to_string(totalDamage) + " damage vs Level 3 Critters (+" + 
                                                       std::to_string(static_cast<int>(multiplier)) + "x)\n";
                                        // Add upgrade cost info based on current level
                                        if (hoveredTower->canUpgrade()) {
                                            if (hoveredTower->getLevel() == 1) {
                                                tooltipString += "Upgrade Cost: $100\n";
                                            } else if (hoveredTower->getLevel() == 2) {
                                                tooltipString += "Upgrade Cost: $325\n";
                                            }
                                        }
                                    }
                                    else
                                    {
                                        tooltipString += "Type: " + type + "\n";
                                        tooltipString += "Level: " + std::to_string(hoveredTower->getLevel()) + "\n";
                                        tooltipString += "Power: " + std::to_string(static_cast<int>(hoveredTower->getPower())) + "\n";
                                        tooltipString += "Range: " + std::to_string(static_cast<int>(hoveredTower->getRange())) + "\n";
                                        std::stringstream fireRateStream;
                                        fireRateStream << std::fixed << std::setprecision(2) << hoveredTower->getFireRate();
                                        tooltipString += "Fire Rate: " + fireRateStream.str() + "\n";
                                        // Add upgrade cost info based on current level
                                        if (hoveredTower->canUpgrade()) {
                                            if (hoveredTower->getLevel() == 1) {
                                                tooltipString += "Upgrade Cost: $100\n";
                                            } else if (hoveredTower->getLevel() == 2) {
                                                tooltipString += "Upgrade Cost: $390\n";
                                            }
                                        }
                                    }

                                    // Position tooltip near mouse but not under it
                                    towerTooltipText.setString(tooltipString);
                                    sf::FloatRect textBounds = towerTooltipText.getLocalBounds();
                                    
                                    // Get window size for boundary checking
                                    sf::Vector2u windowSize = window.getSize();
                                    
                                    // Check if tooltip would go off the right edge
                                    if (worldPos.x + textBounds.size.x + 25.f > windowSize.x) {
                                        // Not enough space to the right, position tooltip to the left of the mouse
                                        towerTooltipText.setPosition(sf::Vector2f(worldPos.x - textBounds.size.x - 15.f, worldPos.y - 15.f));
                                    } else {
                                        // Default positioning (to the right of mouse)
                                        towerTooltipText.setPosition(sf::Vector2f(worldPos.x + 15.f, worldPos.y - 15.f));
                                    }
                                    
                                    // Size the background to fit the text
                                    towerTooltipBackground.setSize(sf::Vector2f(textBounds.size.x + 20.f, textBounds.size.y + 20.f));
                                    towerTooltipBackground.setPosition(sf::Vector2f(
                                        towerTooltipText.getPosition().x - 10.f,
                                        towerTooltipText.getPosition().y - 10.f));
                                    
                                    // Check if tooltip would go off the bottom edge
                                    if (towerTooltipText.getPosition().y + textBounds.size.y + 20.f > windowSize.y) {
                                        // Not enough space below, move tooltip up
                                        float yOffset = textBounds.size.y + 30.f;
                                        towerTooltipText.setPosition(sf::Vector2f(
                                            towerTooltipText.getPosition().x,
                                            worldPos.y - yOffset));
                                        towerTooltipBackground.setPosition(sf::Vector2f(
                                            towerTooltipBackground.getPosition().x,
                                            towerTooltipText.getPosition().y - 10.f));

                                            
                                    }
                                    // Add this line to actually show the tooltip
                                    showTowerTooltip = true;
                                }
                                else
                                {
                                    // Only if no tower was found, check for critter
                                    Critter *hoveredCritter = spawner.getCritterUnderMouse(worldPos);

                                    if (hoveredCritter)
                                    {
                                        // Format the critter information as a string
                                        std::string tooltipString = "Critter Info:\n";

                                        // Add type info based on texture path
                                        std::string type = "Normal";
                                        if (hoveredCritter->getTexturePath() == "textures/critter2.png")
                                        {
                                            type = "Medium";
                                        }
                                        else if (hoveredCritter->getTexturePath() == "textures/critter3.png")
                                        {
                                            type = "Strong";
                                        }

                                        tooltipString += "Type: " + type + "\n";
                                        tooltipString += "Health: " + std::to_string(static_cast<int>(hoveredCritter->getHealth())) + "\n";
                                        tooltipString += "Speed: " + std::to_string(static_cast<int>(hoveredCritter->getSpeed())) + "\n";
                                        tooltipString += "Reward: $" + std::to_string(hoveredCritter->getReward()) + "\n";

                                        
                                        // Position tooltip near mouse but not under it
                                        critterTooltipText.setString(tooltipString);
                                        sf::FloatRect textBounds = critterTooltipText.getLocalBounds();
                                        
                                        // Get window size for boundary checking
                                        sf::Vector2u windowSize = window.getSize();
                                        
                                        // Check if tooltip would go off the right edge
                                        if (worldPos.x + textBounds.size.x + 25.f > windowSize.x) {
                                            // Not enough space to the right, position tooltip to the left of the mouse
                                            critterTooltipText.setPosition(sf::Vector2f(worldPos.x - textBounds.size.x - 15.f, worldPos.y - 15.f));
                                        } else {
                                            // Default positioning (to the right of mouse)
                                            critterTooltipText.setPosition(sf::Vector2f(worldPos.x + 15.f, worldPos.y - 15.f));
                                        }
                                        
                                        // Size the background to fit the text
                                        tooltipBackground.setSize(sf::Vector2f(textBounds.size.x + 20.f, textBounds.size.y + 20.f));
                                        tooltipBackground.setPosition(sf::Vector2f(
                                            critterTooltipText.getPosition().x - 10.f,
                                            critterTooltipText.getPosition().y - 10.f));
                                        
                                        // Check if tooltip would go off the bottom edge
                                        if (critterTooltipText.getPosition().y + textBounds.size.y + 20.f > windowSize.y) {
                                            // Not enough space below, move tooltip up
                                            float yOffset = textBounds.size.y + 30.f;
                                            critterTooltipText.setPosition(sf::Vector2f(
                                                critterTooltipText.getPosition().x,
                                                worldPos.y - yOffset));
                                            tooltipBackground.setPosition(sf::Vector2f(
                                                tooltipBackground.getPosition().x,
                                                critterTooltipText.getPosition().y - 10.f));
                                        }

                                        showTowerTooltip = false;
                                        showCritterTooltip = true;
                                    }
                                    else
                                    {
                                        showTowerTooltip = false;
                                        showCritterTooltip = false;
                                    }
                                }
                            }
                            else
                            {
                                // If Alt is not pressed, don't show any tooltips
                                showTowerTooltip = false;
                                showCritterTooltip = false;
                            }
                        }
                    }
                }

                // Update game state
                if (!spawner.isGameOver()) {
                    spawner.update(deltaTime);
                    towerManager.update(deltaTime);
                } // Close the condition here

                // Update texts
                waveText.setString("Wave: " + std::to_string(spawner.getCurrentWave()));

                // Lives text for life points
                livesText.setString("Lives: " + std::to_string(spawner.getLifePoints()));


                // Update currency text and recenter it
                currencyText.setString("Gold: $" + std::to_string(spawner.getPlayerCurrency()));
                sf::FloatRect textBounds = currencyText.getLocalBounds();
                currencyText.setPosition(sf::Vector2f(
                    (windowSize.x - textBounds.size.x) / 2.f, // Center horizontally
                    10.f                                      // 10px from top
                    ));

                // Render everything
                window.clear(sf::Color::Black);
                mObs.update();
                tObs.update();
                cObs.update();
                // mapGen.draw(window);
                // spawner.draw(window);
                // towerManager.draw(window);

                // Draw UI elements
                window.draw(waveText);
                window.draw(currencyText);
                // Draw heart icon and lives counter
                window.draw(heartSprite);
                window.draw(livesText);


                // Draw button only if we can start a new wave
                if (spawner.canStartNewWave())
                {
                    window.draw(startButton);
                    window.draw(buttonText);
                }

                // Draw shop last so it appears on top
                shop.draw(window);

                // Draw critter tooltip if active
                if (showCritterTooltip)
                {
                    window.draw(tooltipBackground);
                    window.draw(critterTooltipText);
                }

                // Draw tower tooltip if active
                if (showTowerTooltip)
                {
                    window.draw(towerTooltipBackground);
                    window.draw(towerTooltipText);
                }
                // Draw help button
                window.draw(helpButton);
                window.draw(helpButtonText);
                
                // Draw help dialog if active
                if (showHelpDialog) {
                    // Add semi-transparent overlay
                    sf::RectangleShape overlay(sf::Vector2f(windowSize.x, windowSize.y));
                    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
                    window.draw(overlay);
                    
                    // Draw help dialog and contents
                    window.draw(helpDialog);
                    window.draw(helpDialogTitle);
                    window.draw(helpDialogContent);
                    window.draw(helpCloseButton);
                    window.draw(helpCloseButtonText);
                }
                if (spawner.isGameOver()) {
                    // Add semi-transparent overlay
                    sf::RectangleShape overlay(sf::Vector2f(windowSize.x, windowSize.y));
                    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Semi-transparent black
                    window.draw(overlay);
                    
                    // Position and draw game over text
                    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
                    gameOverText.setPosition(sf::Vector2f(
                        (windowSize.x - gameOverBounds.size.x) / 2.f,
                        (windowSize.y - gameOverBounds.size.y) / 2.f - 50.f
                    ));
                    window.draw(gameOverText);
                    
                    // Position and draw restart text
                    sf::FloatRect restartBounds = restartText.getLocalBounds();
                    restartText.setPosition(sf::Vector2f(
                        (windowSize.x - restartBounds.size.x) / 2.f,
                        gameOverText.getPosition().y + gameOverBounds.size.y + 30.f
                    ));
                    window.draw(restartText);
                }
                // Check for victory condition and draw victory screen
                if (spawner.isGameComplete()) {
                    // Add semi-transparent overlay
                    sf::RectangleShape overlay(sf::Vector2f(windowSize.x, windowSize.y));
                    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Semi-transparent black
                    window.draw(overlay);
                    
                    // Position and draw victory text
                    sf::FloatRect victoryBounds = victoryText.getLocalBounds();
                    victoryText.setPosition(sf::Vector2f(
                        (windowSize.x - victoryBounds.size.x) / 2.f,
                        (windowSize.y - victoryBounds.size.y) / 2.f - 50.f
                    ));
                    window.draw(victoryText);
                    
                    // Position and draw restart text
                    sf::FloatRect victoryRestartBounds = victoryRestartText.getLocalBounds();
                    victoryRestartText.setPosition(sf::Vector2f(
                        (windowSize.x - victoryRestartBounds.size.x) / 2.f,
                        victoryText.getPosition().y + victoryBounds.size.y + 30.f
                    ));
                    window.draw(victoryRestartText);
                }

                window.display();
            }
            catch (const std::exception &e)
            {
                // Log any runtime exceptions that occur during the game loop
                logError("Runtime exception: " + std::string(e.what()));

                // Show error message to user
                std::cerr << "An error occurred: " << e.what() << std::endl;

                // Break the game loop or handle the error as appropriate
                break;
            }
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        // Log any initialization or setup exceptions
        logError("Initialization error: " + std::string(e.what()));
        std::cerr << "Fatal error during game initialization: " << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        // Catch any unknown exceptions
        logError("Unknown fatal error occurred");
        std::cerr << "An unknown fatal error occurred" << std::endl;
        return -1;
    }
}
