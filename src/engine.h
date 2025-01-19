#ifndef ENGINE_H
#define ENGINE_H

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MAIN

typedef struct Game Game;
typedef struct Engine Engine;

struct Game {
	void* gameData;
	void(*init)(Engine* engine);
	void(*update)(Engine* engine);
	void(*render)(Engine* engine);
};

struct Engine {
	int windowWidth;
	int windowHeight;
	char* windowTitle;
	int targetFPS;
	Game* game;
	bool debug;
};

Engine createEngine(int windowWidth, int windowHeight, char* windowTitle);
void runEngine(Engine* engine, Game* game);

void system_log();
void debug_log();

#endif // ENGINE_H
