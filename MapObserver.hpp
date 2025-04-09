/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * MapObserver.hpp - Map observation system
 * 
 * Purpose: Implements the Observer pattern for the Map component,
 * updating the visual representation when map state changes.
 * 
 * Dependencies: SFML 3.0, Observer.hpp, MapGenerator.h
 */
/**
 * @file MapObserver.hpp
 * @brief Implementation of Map Observer header file
 */


#ifndef MAPOBSERVER_H
#define MAPOBSERVER_H

#include <SFML/Graphics.hpp>
#include "Observer.hpp"
#include "MapGenerator.h"
#include <iostream>

class MapObserver : public IObserver {
private:
    MapGenerator* subject;
    sf::RenderWindow* window;

public:
    MapObserver(MapGenerator* mg, sf::RenderWindow* win)
        : subject(mg), window(win)
    {
        subject->addObserver(this);
    }

    ~MapObserver() {
        subject->removeObserver(this);
    }

    void update() override {
        if (window && subject)
        {
            // Calling the Draw method
            subject->draw(*window);
        }
    }
};

#endif // MAPOBSERVER_H
