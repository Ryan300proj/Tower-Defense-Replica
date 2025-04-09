/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * CritterObserver.hpp - Critter observation system
 * 
 * Purpose: Implements the Observer pattern for the Critter component,
 * updating the visual representation when critter states change.
 * 
 * Dependencies: SFML 3.0, Observer.hpp, CritterSpawner.hpp
 */
/**
 * @file CritterObserver.hpp
 * @brief Implementation of Critter Observer header file
 */


#ifndef CRITTEROBSERVER_HPP
#define CRITTEROBSERVER_HPP

#include <SFML/Graphics.hpp>
#include "Observer.hpp"
#include "CritterSpawner.hpp"
#include <iostream>

class CritterObserver : public IObserver {
private:
    CritterSpawner* subject;
    sf::RenderWindow* window;

public:
    CritterObserver(CritterSpawner* c, sf::RenderWindow* win)
        : subject(c), window(win)
    {
        subject->addObserver(this);
    }

    ~CritterObserver() {
        subject->removeObserver(this);
    }

    void update() override {
        subject->draw(*window);
    }
};

#endif // CRITTEROBSERVER_HPP