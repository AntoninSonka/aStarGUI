#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <string>
#include "aStar.h"

int main () {

    const int IPS = 2;
    std::chrono::milliseconds iDuration(1000 / IPS);

    sf::RenderWindow window(sf::VideoMode(SIZE_Y * WINDOW_SIZE_MULTIPLY, SIZE_X * WINDOW_SIZE_MULTIPLY), "aStar");

    std::vector<coords> openVec;
    std::vector<coords> closedVec;

    Tile grid[SIZE_Y][SIZE_X];
    setupGrid(grid);

    grid[START_Y][START_X].hCost = calculateHCost(coords{START_Y, START_X});
    grid[START_Y][START_X].gCost = calculateGCost(grid, coords{START_Y, START_X});
    grid[START_Y][START_X].fCost = grid[START_Y][START_X].hCost + grid[START_Y][START_X].gCost;

    openVec.push_back(coords{START_Y, START_X});
    
    bool foundRoute = false;

    coords current = {0, 0};

    //bool startFinding = true;
    
    int count = 0;
    
    while(window.isOpen()){
        auto startTime = std::chrono::high_resolution_clock::now();
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                window.close();
            }
        }

        //if(startFinding){

        if(!foundRoute){
            current = minElement(openVec, grid);
            count++;
            if((current.y != START_Y || current.x != START_X) || (current.y != FINISH_Y || current.x != FINISH_X)){
                grid[current.y][current.x].rect.setFillColor(sf::Color::Cyan);
            }
            //std::cout << "\nF cost: " << grid[current.y][current.x].fCost << "\n";

            openVec.erase(openVec.begin() + indexByCoords(openVec, current));
            closedVec.push_back(current);

            if(current.y == FINISH_Y && current.x == FINISH_X){
                foundRoute = true;
                goto routeFound;
            }
            bool ym = 0, yp = 0, xm = 0, xp = 0;
            if((current.y - 1) >= 0){
                ym = descoverTile(current, coords{current.y - 1, current.x}, grid, openVec, closedVec);
            }
            if((current.y + 1) < SIZE_Y){
                yp = descoverTile(current, coords{current.y + 1, current.x}, grid, openVec, closedVec);
            }
            if((current.x - 1) >= 0){
                xm = descoverTile(current, coords{current.y, current.x - 1}, grid, openVec, closedVec);
            }
            if((current.x + 1) < SIZE_X){
                xp = descoverTile(current, coords{current.y, current.x + 1}, grid, openVec, closedVec);
            }
            if((current.y - 1) >= 0 && (current.x - 1) >= 0){
                if(!ym || !xm){
                    descoverTile(current, coords{current.y - 1, current.x - 1}, grid, openVec, closedVec);
                }
            }
            if((current.y + 1) < SIZE_Y && (current.x + 1) < SIZE_X){
                if(!yp || !xp){
                    descoverTile(current, coords{current.y + 1, current.x + 1}, grid, openVec, closedVec);
                }
            }
            if((current.y + 1) < SIZE_Y && (current.x - 1) >= 0){
                if(!yp || !xm){
                    descoverTile(current, coords{current.y + 1, current.x - 1}, grid, openVec, closedVec);
                }
            }
            if((current.y - 1) >= 0 && (current.x + 1) < SIZE_X){
                if(!ym || !xp){
                    descoverTile(current, coords{current.y - 1, current.x + 1}, grid, openVec, closedVec);
                }
            }
        }
        if(foundRoute){
            current = grid[current.y][current.x].prevPosition;
            if(current.y == START_Y && current.x == START_X){
                std::cout << count << "\n";
                return 0;
            }
        }
    routeFound:
        
        drawGrid(grid, current, window);
        //}

        window.display();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        if(elapsedTime < iDuration){
            std::this_thread::sleep_for(iDuration - elapsedTime);
        }
    }
    
    return 0;
}
