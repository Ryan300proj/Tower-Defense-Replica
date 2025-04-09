/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TowerShop.hpp - Tower purchase interface
 * 
 * Purpose: Provides UI for tower selection, purchase, and displays
 * tower information to player.
 * 
 * Dependencies: SFML 3.0, Tower, TowerManager
 */
/**
 * @file TowerShop.hpp
 * @brief Implementation of Tower Shop header file
 */
    
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "tower.hpp"
#include "TowerManager.hpp"
#include <functional>

class TowerShop {
public:
    enum class TowerType {
        BASIC = 0,    // Direct damage tower
        AREA = 1,     // Area effect tower
        SPECIAL = 2   // Special tower
    };

    struct TowerInfo {
        std::string name;
        int cost;
        float range;
        float power;
        float fireRate;
        std::string description;
        std::string texturePath;
        TowerType type;
    };

    TowerShop(TowerManager& towerManager, const sf::RenderWindow& window);
    
    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);
    void reset() {
        shopOpen = false;
        selectedTowerType = -1;
    }
    bool isShopOpen() const { return shopOpen; }
    void toggleShop() { shopOpen = !shopOpen; }
    bool hasSelectedTower() const { return selectedTowerType != -1; }
    int getSelectedTowerType() const { return selectedTowerType; }
    void clearSelection() { selectedTowerType = -1; }
    const TowerInfo* getSelectedTowerInfo() const;
    bool isMouseOverShopButton(const sf::Vector2f& mousePos) const;
    bool isMouseOverCloseButton(const sf::Vector2f& mousePos) const;
    int getTowerButtonIndex(const sf::Vector2f& mousePos) const;
    void updateUIPositions(const sf::Vector2f& viewSize);
    using RepositionCallback = std::function<void(const sf::Vector2f&)>;
    void setOnUIRepositionedCallback(RepositionCallback callback) {
    onUIRepositioned = callback;
    }


private:
    static const int SHOP_BUTTON_WIDTH = 150;
    static const int SHOP_BUTTON_HEIGHT = 40;
    static const int SHOP_MENU_WIDTH = 380;
    static const int SHOP_MENU_HEIGHT = 400;

    RepositionCallback onUIRepositioned;
    
    // Tower definitions
    std::map<TowerType, TowerInfo> towerTypes;
    
    // State
    TowerManager& towerManager;
    const sf::RenderWindow& window;
    bool shopOpen;
    int selectedTowerType;
    
    // UI Elements
    sf::RectangleShape shopButton;
    sf::RectangleShape shopMenu;
    sf::RectangleShape closeButton;
    std::vector<sf::RectangleShape> towerButtons;
    sf::Font font;
    sf::Text shopButtonText;
    sf::Text closeButtonText;
    std::vector<sf::Text> towerLabels;
    std::vector<sf::Text> towerDescriptions;

    // Helper methods
    void initializeTowerTypes();
    void setupUIElements();
    void updateUIPositions();
};