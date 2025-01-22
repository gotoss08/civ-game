#ifndef GAME_H_
#define GAME_H_

#include "engine.h"

// HELPERS

typedef enum {
	TileType_Terrain,
	TileType_MetalOre
} TileType;

typedef struct {
	TileType type;
} Tile;

typedef enum {
	UnitType_Worker
} UnitType;

typedef struct {
	UnitType type;
	Vector2 pos;
	Vector2 size;
} Unit;

typedef struct {
	int width;
	int height;
	Tile** tiles;
	Unit units[2048];
	size_t unitsCount;
} Map;

// MAIN

typedef struct {
	Map* map;
	int test;
} GameData;

Game createGame();

#endif // GAME_H_
