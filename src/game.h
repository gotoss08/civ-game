#ifndef GAME_H
#define GAME_H

#include "engine.h"

// HELPERS

typedef enum {
	TileType_Terrain,
	TileType_MetalOre
} TileType;

typedef struct {
	TileType type;
} Tile;

typedef struct {
	int width;
	int height;
	Tile** tiles;
} Map;

// MAIN

typedef struct {
	Map* map;
	int test;
} GameData;

Game createGame();

#endif // GAME_H
