
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define ASTAR_IMPLEMENTATION
#include "astar.h"

#define EXPAND_VARS(engine) Game* game = engine->game;GameData* data = (GameData*) game->gameData;(void)engine;(void)game;(void)data;

// CUSTOM CODE

Vector2 mousePos = {0};
Vector2 mouseWorldPos = {0};

Rectangle player = {0};
Camera2D camera = {0};

Vector2 cameraTarget = {0};
Vector2 cameraLerpedTarget = {0};

bool mouseDragging = false;
Vector2 mouseDragStartPos = {0};
Vector2 cameraDragStartPos = {0};

Path path = {0};
Vector2 startPath = {0};
Vector2 endPath = {0};

Map* createMap(int width, int height) {

	Map* map = malloc(sizeof(Map));
	map->width = width;
	map->height = height;

	return map;

}

// MAIN CODE

GameData* createGameData() {

	GameData* gd = malloc(sizeof(GameData));

	return gd;

}

Map map;
Tile* tileUnderCursor;

#define TILE_SIZE 32

void initGame(Engine* engine) {

	Game* game = engine->game;
	game->gameData = (void*) createGameData();

	map.width = 30;
	map.height = 30;
	map.tiles = (Tile**) malloc(sizeof(Tile*) * map.height);

	for (int y = 0; y < map.height; y++) {
		map.tiles[y] = (Tile*) malloc(sizeof(Tile) * map.width);
		for (int x = 0; x < map.width; x++) {
			map.tiles[y][x].type = TILE_GROUND;
		}
	}

	int chunkSize = 3;
	for (int y = map.height / 2 - chunkSize; y < map.height / 2 + chunkSize; y++) {
		for (int x = map.width / 2 - chunkSize; x < map.width / 2 + chunkSize; x++) {
			map.tiles[y][x].type = TILE_ORE;
		}
	}

	player.width = 32;
	player.height = 32;
	player.x = (float) engine->windowWidth / 2 - player.width / 2;
	player.y = (float) engine->windowHeight / 2 - player.height / 2;

	camera.zoom = 1.0f;
	camera.offset = (Vector2) { engine->windowWidth / 2.0f, engine->windowHeight / 2.0f };

	cameraTarget = (Vector2) { engine->windowWidth / 2.0f, engine->windowHeight / 2.0f };
	cameraLerpedTarget = cameraTarget;

	//

	path = aStar(&map, 0, 0, 10, 10);

}

void updateGame(Engine* engine) {

	EXPAND_VARS(engine);

	mousePos = GetMousePosition();
	mouseWorldPos = GetScreenToWorld2D(mousePos, camera);

	bool recalculatePath = false;

	if (IsMouseButtonPressed(0)) {

		mouseDragging = true;
		mouseDragStartPos = mousePos;
		cameraDragStartPos = cameraTarget;

		startPath = mouseWorldPos;
		recalculatePath = true;

	}

	if (IsMouseButtonPressed(1)) {
		endPath = mouseWorldPos;
		recalculatePath = true;
	}

	if (recalculatePath) {
		int startx = floorf(startPath.x / TILE_SIZE);
		int starty = floorf(startPath.y / TILE_SIZE);
		int endx = endPath.x / TILE_SIZE;
		int endy = endPath.y / TILE_SIZE;
		path = aStar(&map, startx, starty, endx, endy);
	}

	if (IsMouseButtonReleased(0)) {
		mouseDragging = false;
	}

	if (mouseDragging) {
		Vector2 mouseDraggedDistance = Vector2Subtract(mouseDragStartPos, mousePos);
		cameraTarget = Vector2Add(cameraDragStartPos, mouseDraggedDistance);
	}

	const float cameraSpeed = 15;

	if (IsKeyDown(KEY_W)) {
		cameraTarget.y -= cameraSpeed;
	}
	if (IsKeyDown(KEY_S)) {
		cameraTarget.y += cameraSpeed;
	}
	if (IsKeyDown(KEY_A)) {
		cameraTarget.x -= cameraSpeed;
	}
	if (IsKeyDown(KEY_D)) {
		cameraTarget.x += cameraSpeed;
	}

	float wheelMoveScale = 5;
	Vector2 wheelMove = GetMouseWheelMoveV();
	wheelMove = Vector2Scale(wheelMove, wheelMoveScale);

	cameraTarget = Vector2Add(cameraTarget, wheelMove);

	cameraLerpedTarget = Vector2Lerp(cameraLerpedTarget, cameraTarget, 0.1f);
	camera.target = cameraLerpedTarget;

}

void renderGame(Engine* engine) {

	EXPAND_VARS(engine);

	BeginMode2D(camera);

	// draw map

	for (int y = 0; y < map.height; y++) {
		for (int x = 0; x < map.width; x++) {

			Tile* tile = &map.tiles[y][x];

			Color tileColor = (Color) { 100, 100, 100, 255 };
			Rectangle tileRect = (Rectangle) { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

			if (tile->type == TILE_ORE) {
				const int gap = 7;
				Rectangle innerRect = tileRect;
				innerRect.x += gap;
				innerRect.y += gap;
				innerRect.width -= gap * 2;
				innerRect.height -= gap * 2;
				tileColor = RED;
				DrawRectangleRec(innerRect, tileColor);
			}

			if (CheckCollisionPointRec(mouseWorldPos, tileRect)) {
				tileColor = ColorBrightness(tileColor, 1.0f);
				/* tileColor = ColorContrast(tileColor, 1.0f); */
			}

			DrawRectangleLinesEx(tileRect, 2, tileColor);

		}
	}


	for (int y = 0; y < map.height; y++) {
		for (int x = 0; x < map.width; x++) {
		}
	}

	DrawRectangle(100, 100, 100, 100, BLUE);
	DrawRectangleRec(player, RED);

	if (path.size > 0) {
		for (int i = 0; i < path.size; i++) {
			int x = path.x[i];
			int y = path.y[i];
			int gap = 5;
			Vector2 worldPos = { x * TILE_SIZE + gap, y * TILE_SIZE + gap };
			DrawRectangle(worldPos.x, worldPos.y, TILE_SIZE - gap * 2, TILE_SIZE - gap * 2, YELLOW);
			const char* text = TextFormat("%d,%d", x, y);
			DrawText(text, worldPos.x, worldPos.y, 12, BLACK);
		}
	}

	EndMode2D();

}

// EXPORT CODE

Game createGame() {

	Game game = {0};
	game.init = &initGame;
	game.update = &updateGame;
	game.render = &renderGame;

	return game;
}

const Tile* getTile(const Map* map, const int x, const int y) {
	return &map->tiles[y][x];
}

const TileAttributes tileAttributes[TILE_TYPE_COUNT] = {
    [TILE_GROUND] = {.solid = false, .movementCost = 1},
    [TILE_ORE]	  = {.solid = true, .movementCost = 2},
};

const TileAttributes *getTileAttributes(const Map *map, const int x,
                                        const int y) {
	return &tileAttributes[getTile(map, x, y)->type];
}
