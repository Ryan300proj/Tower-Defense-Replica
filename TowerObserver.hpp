/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * TowerObserver.hpp - Tower observation system
 * 
 * Purpose: Implements the Observer pattern for Tower entities,
 * monitoring state changes and updating the visual representation.
 * 
 * Dependencies: SFML 3.0, Observer.hpp, TowerManager.hpp
 */
/**
 * @file TowerObserver.hpp
 * @brief Implementation of Tower Observer header file
 */

#ifndef TOWEROBSERVER_H
#define TOWEROBSERVER_H

#include <SFML/Graphics.hpp>
#include "Observer.hpp"
#include "TowerManager.hpp"
#include <iostream>

/**
 * Observes a single Tower.
 * On update(), it fetches the Tower’s data and "displays" it.
 */
class TowerObserver : public IObserver {
private:
    TowerManager* subject;
    sf::RenderWindow* window;

public:
    TowerObserver(TowerManager* tm, sf::RenderWindow* win)
        : subject(tm), window(win)
    {
        subject->addObserver(this);
    }

    ~TowerObserver() {
        subject->removeObserver(this);
    }

    void update() override {
        subject->draw(*window);
    }
};

#endif // TOWEROBSERVER_H
