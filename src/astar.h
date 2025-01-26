#ifndef ASTAR_H_
#define ASTAR_H_

#include "game.h"

typedef struct {
    int x[256];
    int y[256];
    size_t size;
} Path;

Path aStar(Map* map, int startX, int startY, int endX, int endY);

// TODO: imeplement flow field for multiple units

#ifdef ASTAR_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INFINITY 9999

// Node structure for pathfinding
typedef struct {
    int x, y;           // Coordinates
    int gCost, hCost;   // G (cost to reach) + H (heuristic to goal)
    int fCost;          // Total cost (g + h)
    int walkable;       // 1 if walkable, 0 if obstacle
    int parentX, parentY; // To trace the path back
} Node;

const int DIRS[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1},  // 4 cardinal directions
                        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}; // 4 diagonals

// Function to calculate Manhattan distance (heuristic)
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// A* Pathfinding
Path aStar(Map* map, int startX, int startY, int endX, int endY) {

    Node** grid = malloc(sizeof(Node*) * map->height);
    int** openSet = malloc(sizeof(int*) * map->height);
    int** closedSet = malloc(sizeof(int*) * map->height);

    for (int y = 0; y < map->height; y++) {
        grid[y] = malloc(sizeof(Node) * map->width);
        openSet[y] = malloc(sizeof(int) * map->width);
        closedSet[y] = malloc(sizeof(int) * map->width);
        for (int x = 0; x < map->width; x++) {
            openSet[y][x] = 0;
            closedSet[y][x] = 0;
        }
    }

    // Initialize the grid
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            grid[y][x].x = x;
            grid[y][x].y = y;
            grid[y][x].gCost = INFINITY;
            grid[y][x].hCost = heuristic(x, y, endX, endY);
            grid[y][x].fCost = INFINITY;
            grid[y][x].walkable = !getTileAttributes(map, x, y)->solid;
            grid[y][x].parentX = -1;
            grid[y][x].parentY = -1;
        }
    }

    // Start node
    grid[startY][startX].gCost = 0;
    grid[startY][startX].fCost = grid[startY][startX].hCost;
    openSet[startY][startX] = 1;

    while (1) {
        // Find node in open set with lowest fCost
        int currentX = -1, currentY = -1;
        int lowestFCost = INFINITY;

        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                if (openSet[y][x] && grid[y][x].fCost < lowestFCost) {
                    lowestFCost = grid[y][x].fCost;
                    currentX = x;
                    currentY = y;
                }
            }
        }

        Path path = {0};

        // If no path is found, stop
        if (currentX == -1) {
            return path;
        }

        // If goal is reached, trace the path
        if (currentX == endX && currentY == endY) {
            int pathX = endX, pathY = endY;
            while (pathX != startX || pathY != startY) {
                path.x[path.size] = pathX;
                path.y[path.size] = pathY;
                path.size++;
                int tempX = grid[pathY][pathX].parentX;
                int tempY = grid[pathY][pathX].parentY;
                pathX = tempX;
                pathY = tempY;
            }
            path.x[path.size] = startX;
            path.y[path.size] = startY;
            path.size++;
            return path;
        }

        // Move current node from open to closed set
        openSet[currentY][currentX] = 0;
        closedSet[currentY][currentX] = 1;

        // Check neighbors
        for (int i = 0; i < 8; i++) {
            int nx = currentX + DIRS[i][0];
            int ny = currentY + DIRS[i][1];

            if (nx >= 0 && nx < map->width && ny >= 0 && ny < map->height) {
                if (!grid[ny][nx].walkable || closedSet[ny][nx]) {
                    continue; // Skip unwalkable or closed nodes
                }

                int newGCost = grid[currentY][currentX].gCost + 1;

                if (newGCost < grid[ny][nx].gCost || !openSet[ny][nx]) {
                    grid[ny][nx].gCost = newGCost;
                    grid[ny][nx].fCost = newGCost + grid[ny][nx].hCost;
                    grid[ny][nx].parentX = currentX;
                    grid[ny][nx].parentY = currentY;

                    if (!openSet[ny][nx]) {
                        openSet[ny][nx] = 1;
                    }
                }
            }
        }
    }
}

#endif

#endif // ASTAR_H_
