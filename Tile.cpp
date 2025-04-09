/**
 * Tower Defense Game
 * Ryan Sefrioui - Sanjai Ratnarajah
 * 
 * Tile.cpp - Tile system implementation
 * 
 * Purpose: Implements tile behavior and properties for map
 * construction and path creation.
 * 
 * Dependencies: Tile.h, iostream, string
 */
/**
 * @file Tile.cpp
 * @brief Implements individual map tiles and their properties
 */


#include "Tile.h"

// Constructor
Tile::Tile() : x(-1), y(-1), type(TileType::Scenery) {}
Tile::Tile(int x, int y, TileType type) : x(x), y(y), type(type) {}

// Getters
int Tile::getX() const { return x; }
int Tile::getY() const { return y; }
Tile::TileType Tile::getType() const { return type; }

// Setter
void Tile::setType(TileType newType) { type = newType; }

// Print function
void Tile::print() const {
    cout << typeToString(type) << endl;
}

// Convert enum to string
string Tile::typeToString(TileType type) {
    switch (type) {
        case TileType::Path: return "Path";
        case TileType::Beginning: return "Beginning";
        case TileType::End: return "End";
        case TileType::Tower: return "Tower";
        default: return "Scenery";
    }
}

// Convert string to enum
Tile::TileType Tile::stringToType(const string& typeStr) {
    if (typeStr == "Path") return TileType::Path;
    if (typeStr == "Beginning") return TileType::Beginning;
    if (typeStr == "End") return TileType::End;
    if (typeStr == "Tower") return TileType::Tower;
    return TileType::Scenery;
}

