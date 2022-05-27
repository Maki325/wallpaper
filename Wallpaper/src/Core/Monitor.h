#pragma once
#include "pch.h"

namespace Wallpaper {
  struct Monitor {
    HMONITOR monitorId;
    RECT area;
    RECT workingArea;
  };
}
