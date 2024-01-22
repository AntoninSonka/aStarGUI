#include "aStar.h"

void clearScreen(){
    if(!LINUX){
        system("cls");
    }
    else{
        system("clear");
    }
}

void setupGrid(Tile grid[SIZE_Y][SIZE_X]){
    const int sizeY = SIZE_Y;
    const int sizeX = SIZE_X;
    bool wallArr[sizeY][sizeX] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    for(int i = 0; i < SIZE_Y; i++){
        for(int j = 0; j < SIZE_X; j++){
            grid[i][j].isWall = wallArr[i][j];
            grid[i][j].position = {i, j};
            grid[i][j].rect.setPosition(j * WINDOW_SIZE_MULTIPLY, i * WINDOW_SIZE_MULTIPLY); // setPosition funkce je evidentne x, y a ne y, x. Obracene, jako to mam v kodu
            grid[i][j].rect.setSize(sf::Vector2f(WINDOW_SIZE_MULTIPLY, WINDOW_SIZE_MULTIPLY));
            if(grid[i][j].isWall){
                grid[i][j].rect.setFillColor(sf::Color::Red);
            }
            else if(i == START_Y && j == START_X){
                grid[i][j].rect.setFillColor(sf::Color::Green);
            }
            else if(i == FINISH_Y && j == FINISH_X){
                grid[i][j].rect.setFillColor(sf::Color::Yellow);
            }
            else{
                grid[i][j].rect.setFillColor(sf::Color::Black);
            }
        }
    }
}

void printGrid(Tile grid[SIZE_Y][SIZE_X], coords position){
    for(int i = 0; i < SIZE_Y; i++){
        for(int j = 0; j < SIZE_X; j++){
            if(i == FINISH_Y && j == FINISH_X){
                std::cout << "\033[33m";
            }
            else if(i == START_Y && j == START_X){
                std::cout << "\033[32m";
            }
            else if(i == position.y && j == position.x){
                std::cout << "\033[34m";
            }
            else if(grid[i][j].isWall){
                std::cout << "\033[31m";
            }
            if(grid[i][j].isWall){
                std::cout << "X ";
            }
            else{
                std::cout << ((grid[i][j].fCost == 1000) ? 0 : grid[i][j].fCost) << " ";
            }
            std::cout << "\033[0m";
        }
        std::cout << "\n";
    }
}

void drawGrid(Tile grid[SIZE_Y][SIZE_X], coords position, sf::RenderWindow& window){
    for(int i = 0; i < SIZE_Y; i++){
        for(int j = 0; j < SIZE_X; j++){
            if(i == position.y && j == position.x){
                sf::Color baseColor = grid[i][j].rect.getFillColor();

                grid[i][j].rect.setFillColor(sf::Color::Blue);

                window.draw(grid[i][j].rect);

                grid[i][j].rect.setFillColor(baseColor);
            }
            else{
                window.draw(grid[i][j].rect);
            }
        }
    }
}

float calculateHCost(coords position){
    float hCost = 0.f;
    while(position.y != FINISH_Y || position.x != FINISH_X){
        if(position.y < FINISH_Y && position.x < FINISH_X){
            position.y++;
            position.x++;
            hCost += 1.41f;
        }
        else if(position.y < FINISH_Y && position.x > FINISH_X){
            position.y++;
            position.x--;
            hCost += 1.41f;
        }
        else if(position.y > FINISH_Y && position.x < FINISH_X){
            position.y--;
            position.x++;
            hCost += 1.41f;
        }
        else if(position.y > FINISH_Y && position.x > FINISH_X){
            position.y--;
            position.x--;
            hCost += 1.41f;
        }
        else if(position.y < FINISH_Y){
            position.y++;
            hCost++;
        }
        else if(position.y > FINISH_Y){
            position.y--;
            hCost++;
        }
        else if(position.x < FINISH_X){
            position.x++;
            hCost++;
        }
        else if(position.x > FINISH_X){
            position.x--;
            hCost++;
        }
    }
    return hCost;
}

float calculateGCost(Tile grid[SIZE_Y][SIZE_X], coords position){
    float gCost = 0;
    while(position.y != START_Y || position.x != START_X){
        coords original = position;
        position = grid[original.y][original.x].prevPosition;
        if(((original.y < position.y && original.x < position.x) || (original.y > position.y && original.x > position.x)) || ((original.y < position.y && original.x > position.x) || (original.y > position.y && original.x < position.x))){
            gCost += 1.41f;
        }
        else{
            gCost++;
        }
    }
    return gCost;
}

coords minElement(std::vector<coords> vec, Tile grid[SIZE_Y][SIZE_X]){
    float lowestF = 2000.f;
    coords lowestCoords;
    for(int i = 0; i < vec.size(); i++){
        if(grid[vec[i].y][vec[i].x].fCost < lowestF){
            lowestF = grid[vec[i].y][vec[i].x].fCost;
            lowestCoords = vec[i];
        }
    }
    float lowestH = 2000.f;
    for(int i = 0; i < vec.size(); i++){
        if(grid[vec[i].y][vec[i].x].fCost == lowestF){
            if(grid[vec[i].y][vec[i].x].hCost < lowestH){
                lowestH = grid[vec[i].y][vec[i].x].hCost;
                lowestCoords = vec[i];
                //std::cout << "y: " << vec[i].y << " x: " << vec[i].x << " hCost: " << grid[vec[i].y][vec[i].x].hCost << "\n";
            }
        }
    }

    return lowestCoords;
}

int indexByCoords(std::vector<coords> vec, coords position){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i].y == position.y && vec[i].x == position.x){
            return i;
        }
    }
    return -1;
}

bool compareGCosts(coords position, Tile grid[SIZE_Y][SIZE_X], coords newParents){
    int originalG = grid[position.y][position.x].gCost;

    coords originalParents = grid[position.y][position.x].prevPosition;

    grid[position.y][position.x].prevPosition = newParents;

    int newG = calculateGCost(grid, position);

    if(newG < originalG){
        grid[position.y][position.x].prevPosition = originalParents;
        return 1;
    }

    grid[position.y][position.x].prevPosition = originalParents;
    return 0;
}

int descoverTile(coords current, coords neighbour, Tile grid[SIZE_Y][SIZE_X], std::vector<coords>& openVec, std::vector<coords> closedVec){

    bool isInClosed = (indexByCoords(closedVec, neighbour) + 1);

    if(grid[neighbour.y][neighbour.x].isWall){//tady se musi checkovat pro to, jesti tam neni zed, to bude delat robot sam
        return 1;
    }
    if(isInClosed){
        return 0;
    }

    bool isInOpen = 0;

    if(!openVec.empty()){
        isInOpen = (indexByCoords(openVec, neighbour) + 1);
    }

    bool compareGCostsSuccess = 0;

    if(!(grid[neighbour.y][neighbour.x].prevPosition.y == -1)){
        if(compareGCosts(neighbour, grid, current)){
            compareGCostsSuccess = 1;
        }
    }

    if(compareGCostsSuccess || !isInOpen){
        grid[neighbour.y][neighbour.x].prevPosition = current;
        grid[neighbour.y][neighbour.x].hCost = calculateHCost(neighbour);
        grid[neighbour.y][neighbour.x].gCost = calculateGCost(grid, neighbour);
        grid[neighbour.y][neighbour.x].fCost = grid[neighbour.y][neighbour.x].hCost + grid[neighbour.y][neighbour.x].gCost;
        if(!isInOpen){
            openVec.push_back(neighbour);
        }
    }
    return 0;
}
