#include "game.h"

void runGameLoop(App *app) {
  Game game = initGame();

  while (game.isRunning) {
    update(app, &game);
    render(app, &game);
  }
}

Game initGame(void) {
  Player player = {.x = PLAYER_START_X,
                   .y = PLAYER_START_Y,
                   .width = PLAYER_WIDTH,
                   .height = PLAYER_HEIGHT,
                   .yVelocity = 0};

  Pipe pipes[MAX_PIPES];
  srand(time(NULL));
  for (int i = 0; i < MAX_PIPES; i++) {
    pipes[i].x = (i + 1) * PIPE_GAP;
    pipes[i].width = PIPE_WIDTH;
    pipes[i].gapStart = rand() % (SCREEN_HEIGHT - 300);
    pipes[i].gapEnd = pipes[i].gapStart + 300;
  }

  int highScore = 0;
  FILE *file = fopen(HIGHSCORE_PATH, "r");
  if (file) {
    if (fscanf(file, "%d", &highScore) != 1) {
      handleWarning("Couldn't read high score from file");
    }
    fclose(file);
  }

  Game game = {
      .player = player,
      .score = 0,
      .isRunning = true,
      .state = GAME_STATE_PLAYING,
      .lastPhysicsTime = SDL_GetTicks(),
      .highScore = highScore,
  };
  memcpy(game.pipes, pipes, sizeof(pipes));

  return game;
}

void update(App *app, Game *game) {
  float deltaTime = (SDL_GetTicks() - game->lastPhysicsTime) / 1000.0f;
  game->lastPhysicsTime = SDL_GetTicks();

  handleInput(app, game);

  if (game->state == GAME_STATE_PLAYING) {
    handlePhysics(game, deltaTime);
    if (isPlayerColliding(game)) {
      Mix_PlayChannel(-1, app->hurtSound, 0);
      handleGameOver(game);
    }
  }
}

void handlePhysics(Game *game, float deltaTime) {
  if (abs(game->player.yVelocity) > MAX_VELOCITY) {
    game->player.yVelocity =
        game->player.yVelocity > 0 ? MAX_VELOCITY : -MAX_VELOCITY;
  }
  game->player.y -= game->player.yVelocity * deltaTime;
  game->player.yVelocity -= GRAVITY * deltaTime;
  if (game->player.y < 0) {
    game->player.y = 0;
  }
  if (game->player.y + PLAYER_HEIGHT > SCREEN_HEIGHT) {
    game->player.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
  }

  for (int i = 0; i < MAX_PIPES; i++) {
    game->pipes[i].x -= PIPE_SPEED * deltaTime;
    if (game->pipes[i].x < -PIPE_WIDTH) {
      game->score++;
      game->pipes[i].x = SCREEN_WIDTH + (PIPE_GAP - PIPE_WIDTH);
      game->pipes[i].gapStart = rand() % (SCREEN_HEIGHT - 300);
      game->pipes[i].gapEnd = game->pipes[i].gapStart + 300;
    }
  }
}

void handleInput(App *app, Game *game) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      game->isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        game->isRunning = false;
      }

      if (event.key.keysym.sym == SDLK_p) {
        if (game->state == GAME_STATE_PLAYING) {
          game->state = GAME_STATE_PAUSED;
        } else if (game->state == GAME_STATE_PAUSED) {
          game->state = GAME_STATE_PLAYING;
        }
      };

      if (event.key.keysym.sym == SDLK_SPACE &&
          game->state == GAME_STATE_PLAYING) {
        Mix_PlayChannel(-1, app->jumpSound, 0);
        game->player.yVelocity = JUMP_VELOCITY;
      }

      if (event.key.keysym.sym == SDLK_r) {
        *game = initGame();
      }
      break;
  }
}

bool isPlayerColliding(Game *game) {
  SDL_Rect playerRect = {(int)game->player.x, (int)game->player.y,
                         (int)game->player.width, (int)game->player.height};
  for (int i = 0; i < MAX_PIPES; i++) {
    SDL_Rect topPipeRect = {(int)game->pipes[i].x, (int)0,
                            (int)game->pipes[i].width,
                            (int)game->pipes[i].gapStart};
    SDL_Rect bottomPipeRect = {(int)game->pipes[i].x,
                               (int)game->pipes[i].gapEnd,
                               (int)game->pipes[i].width,
                               (int)(SCREEN_HEIGHT - game->pipes[i].gapEnd)};

    if (SDL_HasIntersection(&playerRect, &topPipeRect) ||
        SDL_HasIntersection(&playerRect, &bottomPipeRect)) {
      return true;
    }
  }
  return false;
}

void handleGameOver(Game *game) {
  if (game->score > game->highScore) {
    FILE *file = fopen(HIGHSCORE_PATH, "w");
    if (file) {
      fprintf(file, "%d", game->score);
      fclose(file);
    } else {
      handleWarning("Couldn't save high score to file");
    }
  }
  game->state = GAME_STATE_GAMEOVER;
}

void render(App *app, Game *game) {
  SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
  SDL_RenderClear(app->renderer);

  if (game->state != GAME_STATE_GAMEOVER) {
    SDL_SetRenderDrawColor(app->renderer, 0, 255, 0, 255);
    SDL_Rect playerRect = {(int)game->player.x, (int)game->player.y,
                           (int)game->player.width, (int)game->player.height};
    SDL_RenderFillRect(app->renderer, &playerRect);

    SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
    for (int i = 0; i < MAX_PIPES; i++) {
      SDL_Rect topPipeRect = {(int)game->pipes[i].x, (int)0,
                              (int)game->pipes[i].width,
                              (int)game->pipes[i].gapStart};
      SDL_RenderFillRect(app->renderer, &topPipeRect);

      SDL_Rect bottomPipeRect = {(int)game->pipes[i].x, (int)SCREEN_HEIGHT,
                                 (int)game->pipes[i].width,
                                 (int)game->pipes[i].gapEnd - SCREEN_HEIGHT};
      SDL_RenderFillRect(app->renderer, &bottomPipeRect);
    }
  } else {
    renderText(app, "Game Over", SCREEN_WIDTH / 2 - 100,
               SCREEN_HEIGHT / 2 - 50);
    renderText(app, "Press R to Restart", SCREEN_WIDTH / 2 - 200,
               SCREEN_HEIGHT / 2 + 50);
  }

  char buffer[20];
  snprintf(buffer, sizeof buffer, "High Score: %d", game->highScore);
  renderText(app, buffer, 10, 10);
  snprintf(buffer, sizeof buffer, "Score: %d", game->score);
  renderText(app, buffer, 10, 40);
  renderText(app, "P to Pause", 10, SCREEN_HEIGHT - 30);

  SDL_RenderPresent(app->renderer);
}

void renderText(App *app, const char *text, int x, int y) {
  SDL_Color textColor = {255, 255, 255};

  SDL_Surface *surface = TTF_RenderText_Solid(app->font, text, textColor);
  if (surface == NULL) {
    fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
  if (texture == NULL) {
    fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
  }

  SDL_Rect textRect = {x, y, surface->w, surface->h};
  SDL_RenderCopy(app->renderer, texture, NULL, &textRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}
