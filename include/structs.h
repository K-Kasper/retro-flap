#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "config.h"

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  TTF_Font *font;
  Mix_Chunk *jumpSound;
  Mix_Chunk *hurtSound;
} App;

typedef struct {
  float x;
  float y;
  float width;
  float height;
  float yVelocity;
} Player;

typedef struct {
  float x;
  float width;
  float gapStart;
  float gapEnd;
} Pipe;

typedef enum {
  GAME_STATE_PLAYING,
  GAME_STATE_PAUSED,
  GAME_STATE_GAMEOVER
} GameState;

typedef struct {
  bool isRunning;
  GameState state;
  Player player;
  Pipe pipes[MAX_PIPES];
  int score;
  int highScore;
  int lastPhysicsTime;
} Game;
