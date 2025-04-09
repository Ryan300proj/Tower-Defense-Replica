/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * CritterFactory.hpp - Factory pattern implementation for critter creation
 * 
 * Purpose: Implements the Factory pattern to generate different types of critters
 * with varying characteristics based on the current wave number.
 * 
 * Dependencies: CritterSpawner.hpp
 */
/**
 * @file CritterFactory.hpp
 * @brief Implementation of CritterFactory header file
 */

 #ifndef CRITTERFACTORY_HPP
 #define CRITTERFACTORY_HPP
 
 #include <queue>
 #include <memory>
 #include <string>
 #include "CritterSpawner.hpp"
 
 // Abstract Creator (Factory) class
 class AbstractCritterFactory {
 public:
     virtual ~AbstractCritterFactory() = default;
     
     // Factory method that returns a queue of critter types for a wave
     virtual std::queue<CritterSpawner::CritterType> createWave(int waveNumber) const = 0;
     
 protected:
     // Utility method to create a single critter type
     CritterSpawner::CritterType createCritterType(
         const std::string& texturePath, 
         float speed, 
         float health, 
         int reward) const 
     {
         return {texturePath, speed, health, reward};
     }
 };
 
 // Concrete Factory for Easy waves (early game)
 class EasyWaveFactory : public AbstractCritterFactory {
 public:
     std::queue<CritterSpawner::CritterType> createWave(int waveNumber) const override {
         std::queue<CritterSpawner::CritterType> waveQueue;
         
         // Critter type for easy waves
         CritterSpawner::CritterType critter1 = createCritterType(
             "textures/critter.png", 
             100.f + (waveNumber * 5.f), // Small speed increase per wave
             100.f + (waveNumber * 10.f), // Small health increase per wave
             5 + waveNumber // Small reward increase per wave
         );
         
         // Add 10 basic critters
         for (int i = 0; i < 10; ++i) {
             waveQueue.push(critter1);
         }
         
         return waveQueue;
     }
 };
 
 // Concrete Factory for Medium waves (mid game)
 class MediumWaveFactory : public AbstractCritterFactory {
 public:
     std::queue<CritterSpawner::CritterType> createWave(int waveNumber) const override {
         std::queue<CritterSpawner::CritterType> waveQueue;
         
         // Two critter types for medium waves
         CritterSpawner::CritterType critter1 = createCritterType(
             "textures/critter.png", 
             100.f + (waveNumber * 5.f),
             120.f + (waveNumber * 15.f),
             7 + waveNumber
         );
         
         CritterSpawner::CritterType critter2 = createCritterType(
             "textures/critter2.png", 
             100.f + (waveNumber * 2.f),
             200.f + (waveNumber * 25.f),
             10 + (waveNumber * 2)
         );
         
         // Mix of basic and medium critters
         for (int i = 0; i < 5; ++i) {
             waveQueue.push(critter1);
             waveQueue.push(critter2);
         }
         
         return waveQueue;
     }
 };
 
 // Concrete Factory for Hard waves (late game)
 class HardWaveFactory : public AbstractCritterFactory {
 public:
     std::queue<CritterSpawner::CritterType> createWave(int waveNumber) const override {
         std::queue<CritterSpawner::CritterType> waveQueue;
         
         // Calculate adjusted wave number (relative to when hard waves start)
         int adjustedWave = waveNumber - 2;
         
         // Medium critter type with scaled stats
         float speed = 100.f + (adjustedWave * 10.f);
         float health = 200.f + (adjustedWave * 50.f);
         int reward = 10 + (adjustedWave * 5);
         
         CritterSpawner::CritterType critter2 = createCritterType(
             "textures/critter2.png",
             speed,
             health,
             reward
         );
         
         // Strong critter type with higher stats
         float strongSpeed = 80.f + (adjustedWave * 5.f);
         float strongHealth = 800.f + (adjustedWave * 200.f);
         int strongReward = 50 + (adjustedWave * 25);
         
         CritterSpawner::CritterType critter3 = createCritterType(
             "textures/critter3.png",
             strongSpeed,
             strongHealth,
             strongReward
         );
         
         // Mix of medium and strong critters
         for (int i = 0; i < 15; ++i) {
             waveQueue.push(critter2);
             waveQueue.push(critter3);
         }
         
         return waveQueue;
     }
 };
 
 // Factory Manager (uses the appropriate factory based on wave number)
 class CritterFactory {
 public:
     static std::queue<CritterSpawner::CritterType> createWave(int waveNumber) {
         if (waveNumber == 1) {
             return easyFactory.createWave(waveNumber);
         } else if (waveNumber == 2) {
             return mediumFactory.createWave(waveNumber);
         } else {
             return hardFactory.createWave(waveNumber);
         }
     }
     
 private:
     static EasyWaveFactory easyFactory;
     static MediumWaveFactory mediumFactory;
     static HardWaveFactory hardFactory;
 };
 
 // Initialize the static factory instances
 EasyWaveFactory CritterFactory::easyFactory;
 MediumWaveFactory CritterFactory::mediumFactory;
 HardWaveFactory CritterFactory::hardFactory;
 
 #endif // CRITTERFACTORY_HPP