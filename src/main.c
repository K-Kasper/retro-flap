#include "app.h"
#include "game.h"
#include "structs.h"

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  App app = initApp();

  runGameLoop(&app);

  exitApp(&app);
  return EXIT_SUCCESS;
}

#else

int main(int argc, char *argv[]) {
  App app = initApp();

  runGameLoop(&app);

  exitApp(&app);
  return EXIT_SUCCESS;
}

#endif
