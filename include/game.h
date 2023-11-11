#pragma once

#include <string.h>
#include <time.h>

#include "common.h"

void runGameLoop(App *app);
Game initGame(void);
void update(App *app, Game *game);
void handlePhysics(Game *game, float deltaTime);
void handleGameOver(Game *game);
void handleInput(App *app, Game *game);
bool isPlayerColliding(Game *game);
void render(App *app, Game *game);
void renderText(App *app, const char *text, int x, int y);
