
#include "game.h"

#define EXPAND_VARS(engine) Game* game = engine->game;GameData* data = (GameData*) game->gameData;(void)engine;(void)game;(void)data;

// CUSTOM CODE

Map* createMap(int width, int height) {

	Map* map = malloc(sizeof(Map));
	map->width = width;
	map->height = height;

	return map;

}

// MAIN CODE

GameData* createGameData() {

	GameData* gd = malloc(sizeof(GameData));
	gd->test = 69;

	return gd;

}

void initGame(Engine* engine) {

	printf("game init\n");

	Game* game = engine->game;
	game->gameData = (void*) createGameData();

}

void updateGame(Engine* engine) {

	EXPAND_VARS(engine);

	// printf("game update\n");

}

void renderGame(Engine* engine) {

	EXPAND_VARS(engine);

	printf("test: %d\n", data->test);

}

// EXPORT CODE

Game createGame() {

	Game game = {0};
	game.init = &initGame;
	game.update = &updateGame;
	game.render = &renderGame;

	return game;

}
