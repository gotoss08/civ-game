#include "engine.h"
#include "game.h"

Engine createEngine(int windowWidth, int windowHeight, char* windowTitle) {

	Engine engine = {0};
	engine.windowWidth = windowWidth;
	engine.windowHeight = windowHeight;
	engine.windowTitle = windowTitle;
	engine.targetFPS = 60;

	return engine;

}

void runEngine(Engine* engine, Game* game) {

	InitWindow(engine->windowWidth, engine->windowHeight, engine->windowTitle);
	SetTargetFPS(engine->targetFPS);

	engine->game = game;
	engine->game->init(engine);

	printf("starting gameloop\n");

	while (!WindowShouldClose()) {

		engine->game->update(engine);

		BeginDrawing();
		ClearBackground(BLACK);

		engine->game->render(engine);

		DrawFPS(10, 10);

		EndDrawing();

	}

	CloseWindow();

}
