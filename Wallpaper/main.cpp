#ifndef UNICODE
#define UNICODE
#endif 

#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  HWND p = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
  HWND* ret = (HWND*)lParam;

  if (p)
  {
    // Gets the WorkerW Window after the current one.
    *ret = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
  }
  return true;
}

HWND get_wallpaper_window() {
  // Fetch the Progman window
  HWND progman = FindWindow(L"ProgMan", NULL);
  // Send 0x052C to Progman. This message directs Progman to spawn a 
  // WorkerW behind the desktop icons. If it is already there, nothing 
  // happens.
  SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
  // We enumerate all Windows, until we find one, that has the SHELLDLL_DefView 
  // as a child. 
  // If we found that window, we take its next sibling and assign it to workerw.
  HWND wallpaper_hwnd = nullptr;
  EnumWindows(EnumWindowsProc, (LPARAM)&wallpaper_hwnd);
  // Return the handle you're looking for.
  return wallpaper_hwnd;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void resetWallpaper() {
  WCHAR path[MAX_PATH];
  SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, &path[0], 0);
  SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, &path[0], SPIF_UPDATEINIFILE);

  std::wcout << "Path(" << wcsnlen(path, 128) << "): ";
  for (int i = 0; i < wcsnlen(path, 128); i++) {
    std::wcout << path[i];
  }
  std::wcout << std::endl;
}

void printRect(RECT r) {
  std::wcout << "RECT: {" << r.left << ", " << r.top << ", " << r.right << ", " << r.bottom << "}" << std::endl;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  std::wofstream file;
  file.open("cout.txt");
  std::wstreambuf* sbuf = std::wcout.rdbuf();
  std::wcout.rdbuf(file.rdbuf());

  std::wcout << "get_wallpaper_window(): " << get_wallpaper_window() << std::endl;
  std::wcout << "GetDesktopWindow(): " << GetDesktopWindow() << std::endl;
  std::wcout << "get_wallpaper_window(): " << get_wallpaper_window() << std::endl;
  std::wcout << "GetDesktopWindow(): " << GetDesktopWindow() << std::endl;
  std::wcout << "GetShellWindow(): " << GetShellWindow() << std::endl;
  auto hwnd = get_wallpaper_window();
  HDC screenDC = GetDC(hwnd); //NULL gets whole screen
  for (int i = 0; i < 1; i++) {
    // resetWallpaper();

    std::wcout << "screenDC(): " << screenDC << std::endl;
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0)); //create brush
    SelectObject(screenDC, brush); //select brush into DC
    Rectangle(screenDC, 1920, 0, 1920 + 1000, 1000); //draw rectangle over whole screen
    //clean up stuff here

    const WCHAR* text = L"Abc";
    /*// TextOut(screenDC, 100, 1000, text, wcslen(text));
    // Obtain the window's client rectangle 
    RECT r{ 100, 100, 1000, 1000 };
    printRect(r);
    // GetClientRect(hwnd, &r);
    printRect(r);
    // THE FIX: by setting the background mode 
    // to transparent, the region is the text itself 
    SetBkMode(screenDC, TRANSPARENT);
    // Bracket begin a path 
    BeginPath(screenDC);
    // Send some text out into the world 
    std::wcout << "A: " << wcslen(text) << std::endl;
    TextOut(screenDC, 100, 100, text, wcslen(text));
    // Bracket end a path 
    EndPath(screenDC);
    // Derive a region from that path 
    SelectClipPath(screenDC, RGN_AND);
    // This generates the same result as SelectClipPath() 
    SelectClipRgn(screenDC, PathToRegion(screenDC));
    // Fill the region with grayness 
    FillRect(screenDC, &r, (HBRUSH) GetStockObject(GRAY_BRUSH));*/

    {
      // SetBkMode(screenDC, TRANSPARENT);
      SetTextColor(screenDC, RGB(0, 255, 0));
      RECT r{ 100, 100, 1000, 1000 };
      DrawTextW(screenDC, text, wcslen(text), &r, DT_CENTER);
    }
  }

  resetWallpaper();

  return 0;
}
