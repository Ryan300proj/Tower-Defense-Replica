/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Tile.h - Map tile system
 * 
 * Purpose: Defines individual map tiles and their properties for
 * path creation and tower placement validation.
 * 
 * Dependencies: Basic C++ libraries
 */
/**
 * @file Tile.h
 * @brief Implementation of Tile header file
 */

#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <string>

using namespace std;

class Tile {
public:
    enum class TileType { Path, Beginning, End, Tower, Scenery, Init };
    Tile();
    Tile(int x, int y, TileType type);

    // Getters
    int getX() const;
    int getY() const;
    TileType getType() const;

    // Setters
    void setType(TileType newType); 

    // Print function
    void print() const;

    // Helper function to convert enum to string
    static string typeToString(TileType type);

    // Helper function to convert string to TileType
    static TileType stringToType(const string& typeStr);

private:
    int x, y; // Grid Position
    TileType type;  // Tile function
};

#endif
