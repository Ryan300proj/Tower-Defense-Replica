/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TowerShop.cpp - Shop interface implementation
 * 
 * Purpose: Implements tower purchase UI, selection handling,
 * and shop menu system.
 * 
 * Dependencies: SFML 3.0, TowerShop.hpp, iostream
 */

 /**
 * @file TowerShop.cpp
 * @brief Implements tower purchasing interface and menu system
 */

#include "TowerShop.hpp"
#include <iostream>

TowerShop::TowerShop(TowerManager& towerManager, const sf::RenderWindow& window)
    : towerManager(towerManager)
    , window(window)
    , shopOpen(false)
    , selectedTowerType(-1)
    , shopButtonText(font)
    , closeButtonText(font)
{
    if (!font.openFromFile("fonts/arial.ttf")) {
        throw std::runtime_error("Could not load font for shop!");
    }

    initializeTowerTypes();
    setupUIElements();
}

void TowerShop::initializeTowerTypes() {
    // Define tower types with stats and descriptions
    towerTypes[TowerType::BASIC] = {
        "Basic Tower",
        100,    // cost
        150.f,  // range
        50.f,   // power
        2.f,    // fire rate
        "Standard tower with good all-round stats",
        "textures/stage_1.png",
        TowerType::BASIC
    };

    towerTypes[TowerType::AREA] = {
        "Area Tower",
        150,    // cost
        120.f,  // range
        75.f,   // power
        1.6f,   // fire rate
        "Deals splash damage to multiple enemies",
        "textures/area1.png",
        TowerType::AREA
    };

    towerTypes[TowerType::SPECIAL] = {
        "Special Tower",
        125,    // cost
        140.f,  // range
        85.f,   // power
        2.0f,   // fire rate
        "Extra damage to level 3 Critters",
        "textures/special1.png",
        TowerType::SPECIAL
    };
}

void TowerShop::setupUIElements() {
    // Get view information for proper positioning
    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    
    // Initialize shop button (will be positioned by updateUIPositions)
    shopButton.setSize(sf::Vector2f(SHOP_BUTTON_WIDTH, SHOP_BUTTON_HEIGHT));
    shopButton.setFillColor(sf::Color(100, 100, 200));
    shopButton.setOutlineColor(sf::Color::White);
    shopButton.setOutlineThickness(2.f);

    shopButtonText.setString("Shop");
    shopButtonText.setCharacterSize(15);
    shopButtonText.setFillColor(sf::Color::White);
    shopButtonText.setOutlineColor(sf::Color::Black); 
    shopButtonText.setOutlineThickness(1.5f);         

    // Initialize shop menu (will be positioned by updateUIPositions)
    shopMenu.setSize(sf::Vector2f(SHOP_MENU_WIDTH, SHOP_MENU_HEIGHT));
    shopMenu.setFillColor(sf::Color(50, 50, 50, 230));
    shopMenu.setOutlineColor(sf::Color::White);
    shopMenu.setOutlineThickness(2.f);

    // Initialize close button
    closeButton.setSize(sf::Vector2f(30.f, 30.f));
    closeButton.setFillColor(sf::Color::Red);

    closeButtonText.setString("X");
    closeButtonText.setCharacterSize(20);
    closeButtonText.setFillColor(sf::Color::White);

    // Setup tower buttons and labels
    for (const auto& tower : towerTypes) {
        // Create button
        sf::RectangleShape towerButton(sf::Vector2f(355.f, 80.f));
        towerButton.setFillColor(sf::Color(70, 70, 70));
        towerButton.setOutlineColor(sf::Color::White);
        towerButton.setOutlineThickness(1.f);
        towerButtons.push_back(towerButton);

        // Create label
        sf::Text nameText(font);
        nameText.setString(tower.second.name + "\nCost: " + std::to_string(tower.second.cost));
        nameText.setCharacterSize(16);
        nameText.setFillColor(sf::Color::White);
        nameText.setOutlineColor(sf::Color::Black); 
        nameText.setOutlineThickness(1.0f);
        towerLabels.push_back(nameText);

        // Create description
        sf::Text descText(font);
        descText.setString(tower.second.description);
        descText.setCharacterSize(12);
        descText.setFillColor(sf::Color(200, 200, 200));
        towerDescriptions.push_back(descText);
    }
    
    // Now position all UI elements
    updateUIPositions(viewSize);
}
void TowerShop::handleClick(const sf::Vector2f& mousePos) {
    if (isMouseOverShopButton(mousePos)) {
        toggleShop();
        return;
    }

    if (shopOpen) {
        if (isMouseOverCloseButton(mousePos)) {
            shopOpen = false;
            clearSelection();
            return;
        }

        int buttonIndex = getTowerButtonIndex(mousePos);
        if (buttonIndex != -1) {
            selectedTowerType = buttonIndex;
            shopOpen = false;
            return;
        }
    }
}

void TowerShop::draw(sf::RenderWindow& window) {
    // Always draw shop button
    window.draw(shopButton);
    window.draw(shopButtonText);

    // Draw shop menu and contents if open
    if (shopOpen) {
        window.draw(shopMenu);
        window.draw(closeButton);
        window.draw(closeButtonText);

        for (size_t i = 0; i < towerButtons.size(); ++i) {
            window.draw(towerButtons[i]);
            window.draw(towerLabels[i]);
            window.draw(towerDescriptions[i]);
        }
    }
}

bool TowerShop::isMouseOverShopButton(const sf::Vector2f& mousePos) const {
    return shopButton.getGlobalBounds().contains(mousePos);
}

bool TowerShop::isMouseOverCloseButton(const sf::Vector2f& mousePos) const {
    return shopOpen && closeButton.getGlobalBounds().contains(mousePos);
}

int TowerShop::getTowerButtonIndex(const sf::Vector2f& mousePos) const {
    for (size_t i = 0; i < towerButtons.size(); ++i) {
        if (towerButtons[i].getGlobalBounds().contains(mousePos)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

const TowerShop::TowerInfo* TowerShop::getSelectedTowerInfo() const {
    if (!hasSelectedTower()) return nullptr;
    auto type = static_cast<TowerType>(selectedTowerType);
    auto it = towerTypes.find(type);
    if (it != towerTypes.end()) {
        return &(it->second);
    }
    return nullptr;
}


// Resize Feature Added
void TowerShop::updateUIPositions(const sf::Vector2f& viewSize) {
    // Update shop button position (bottom right corner)
    shopButton.setPosition(sf::Vector2f(
        viewSize.x - SHOP_BUTTON_WIDTH - 10.f,
        viewSize.y - SHOP_BUTTON_HEIGHT - 10.f
    ));
    
    // Update shop button text position
    sf::FloatRect textBounds = shopButtonText.getLocalBounds();
    shopButtonText.setPosition(sf::Vector2f(
        shopButton.getPosition().x + (SHOP_BUTTON_WIDTH - textBounds.size.x) / 2.f,
        shopButton.getPosition().y + (SHOP_BUTTON_HEIGHT - textBounds.size.y) / 2.f
    ));
    
    // Update shop menu position (centered)
    shopMenu.setPosition(sf::Vector2f(
        (viewSize.x - SHOP_MENU_WIDTH) / 2.f,
        (viewSize.y - SHOP_MENU_HEIGHT) / 2.f
    ));
    
    // Update close button position
    closeButton.setPosition(sf::Vector2f(
        shopMenu.getPosition().x + SHOP_MENU_WIDTH - 40.f,
        shopMenu.getPosition().y + 10.f
    ));
    
    closeButtonText.setPosition(sf::Vector2f(
        closeButton.getPosition().x + 8.f,
        closeButton.getPosition().y + 2.f
    ));
    
    // Update tower buttons and labels
    float buttonY = shopMenu.getPosition().y + 60.f;
    for (size_t i = 0; i < towerButtons.size(); ++i) {
        towerButtons[i].setPosition(sf::Vector2f(shopMenu.getPosition().x + 20.f, buttonY));
        
        towerLabels[i].setPosition(sf::Vector2f(
            towerButtons[i].getPosition().x + 10.f,
            towerButtons[i].getPosition().y + 10.f
        ));
        
        towerDescriptions[i].setPosition(sf::Vector2f(
            towerButtons[i].getPosition().x + 10.f,
            towerButtons[i].getPosition().y + 50.f
        ));
        
        buttonY += 100.f;
    }
    
    // Notify external components about repositioning
    if (onUIRepositioned) {
        onUIRepositioned(viewSize);
    }
}