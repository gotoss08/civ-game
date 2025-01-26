#ifndef GAME_H_
#define GAME_H_

#include "engine.h"

// HELPERS

typedef enum {
	TILE_GROUND,
	TILE_ORE,
	TILE_TYPE_COUNT
} TileType;

typedef struct {
	bool solid;
	int movementCost;
} TileAttributes;

typedef enum {
    ORE_METAL
} OreType;

typedef struct {
	TileType type;
} Tile;

typedef enum {
	UnitType_Worker,
	UnitType_Hauler
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
	size_t unitsSize;
} Map;

const Tile *getTile(const Map *map, const int x, const int y);

const TileAttributes *getTileAttributes(const Map *map, const int x,
                                        const int y);

// MAIN

typedef struct {
	Map* map;
} GameData;

Game createGame();

#endif // GAME_H_
