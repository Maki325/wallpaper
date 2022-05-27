#pragma once
#include "StoppableTask.h"

namespace Wallpaper {
  class ConsoleTask: public StoppableTask {
  public:
    ConsoleTask();
    void Run() override;

    static void InitConsole();
  };
}

