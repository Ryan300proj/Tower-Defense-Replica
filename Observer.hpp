/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Observer.hpp - Observer pattern implementation
 * 
 * Purpose: Defines the core Observer pattern interfaces and base classes,
 * enabling decoupled communication between game components.
 * 
 * Dependencies: Standard C++ libraries
 */
/**
 * @file Observer.hpp
 * @brief Implementation of Observer base classes and interfaces
 */

#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <algorithm>

/**
 * Any class that wants to "observe" changes in a subject must implement update().
 */
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update() = 0; 
};

/**
 * Observable (Subject)
 */
class Observable {
private:
    std::vector<IObserver*> observers;

public:
    virtual ~Observable() = default;

    void addObserver(IObserver* obs) {
        observers.push_back(obs);
    }

    void removeObserver(IObserver* obs) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }

protected:
    // Derived classes call notify() to signal changes:
    void notify() {
        for (auto* obs : observers) {
            obs->update();
        }
    }
};

#endif // OBSERVER_H
