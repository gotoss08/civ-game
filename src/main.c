#include "engine.h"
#include "game.h"

int main(void) {

	Engine engine = createEngine(1280, 720, "engine v0");
	Game game = createGame();
	runEngine(&engine, &game);
	return 0;

}
