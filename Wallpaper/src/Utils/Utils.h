#pragma once

namespace Wallpaper {
  namespace Utils {
    BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
    HWND GetWallpaperWindow();
    void PrintRect(RECT r);
    void PrintWindowSize(HWND hWnd);
    RECT& NormalizeRect(RECT& rect);
  };
}
