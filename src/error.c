#include "error.h"

void handleError(const char* errorMessage) {
  fprintf(stderr, "Error: %s\n", errorMessage);
  exit(EXIT_FAILURE);
}

void handleSdlError(const char* errorMessage) {
  fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
  handleError(errorMessage);
}

void handleTtfError(const char* errorMessage) {
  fprintf(stderr, "TTF Error: %s\n", TTF_GetError());
  handleError(errorMessage);
}

void handleMixError(const char* errorMessage) {
  fprintf(stderr, "Mix Error: %s\n", Mix_GetError());
  handleError(errorMessage);
}

void handleWarning(const char* warningMessage) {
  fprintf(stderr, "Warning: %s\n", warningMessage);
}
