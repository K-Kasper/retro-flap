#include "app.h"

App initApp(void) {
  App app;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    handleSdlError("Initializing SDL");
  }

  app.window =
      SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (app.window == NULL) {
    handleSdlError("Creating window");
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
  if (app.renderer == NULL) {
    handleSdlError("Creating renderer");
  }

  if (TTF_Init() != 0) {
    handleTtfError("Initializing TTF");
  }

  app.font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
  if (app.font == NULL) {
    handleTtfError("Opening font");
  }

  if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 2048) != 0) {
    handleMixError("Initializing Mixer");
  }

  app.hurtSound = Mix_LoadWAV(HURT_SOUND_PATH);
  if (!app.hurtSound) {
    handleMixError("Loading hurt sound");
  }
  app.jumpSound = Mix_LoadWAV(JUMP_SOUND_PATH);
  if (!app.jumpSound) {
    handleMixError("Loading jump sound");
  }

  return app;
}

void exitApp(App *app) {
  Mix_FreeChunk(app->hurtSound);
  Mix_FreeChunk(app->jumpSound);
  Mix_CloseAudio();
  Mix_Quit();
  TTF_CloseFont(app->font);
  TTF_Quit();
  SDL_DestroyRenderer(app->renderer);
  SDL_DestroyWindow(app->window);
  SDL_Quit();
}
