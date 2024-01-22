#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>


#ifdef _WIN32
    const bool LINUX = false;
#elif __linux__
    const bool LINUX = true;
#else
    #error Unsupported operating system
#endif


enum {
    SIZE_Y = 8,
    SIZE_X = 8,
    START_Y = 0,
    START_X = 0,
    FINISH_Y = 7,
    FINISH_X = 7,
    WINDOW_SIZE_MULTIPLY = 64,
    TIME = 250
};

struct coords{
    int y;
    int x;
};

class Tile{
public:
    sf::RectangleShape rect;
    coords position;
    coords prevPosition = {-1, -1}; //previous position
    bool isWall = false;
    float hCost = 0.f; //heuristic cost - the distance from current tile to the finish
    float gCost = 0.f; //the distance from current tile to the start through visited tiles
    float fCost = 1000.f;
};

void clearScreen();

void setupGrid(Tile grid[SIZE_Y][SIZE_X]);

void printGrid(Tile grid[SIZE_Y][SIZE_X], coords position);

void drawGrid(Tile grid[SIZE_Y][SIZE_X], coords position, sf::RenderWindow& window);

float calculateHCost(coords position);

float calculateGCost(Tile grid[SIZE_Y][SIZE_X], coords position);

coords minElement(std::vector<coords> vec, Tile grid[SIZE_Y][SIZE_X]);

int indexByCoords(std::vector<coords> vec, coords position);

bool compareGCosts(coords position, Tile grid[SIZE_Y][SIZE_X], coords newParents);

int descoverTile(coords current, coords neighbour, Tile grid[SIZE_Y][SIZE_X], std::vector<coords>& openVec, std::vector<coords> closedVec);
