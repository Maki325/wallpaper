#include "pch.h"
#include "Core/Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  Wallpaper::Application app;
  app.Run();

  return 0;
}
