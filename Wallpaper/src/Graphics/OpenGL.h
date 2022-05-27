#pragma once

namespace Wallpaper {
  class OpenGL
  {
  public:
    OpenGL();
    OpenGL(HWND hWnd);
    ~OpenGL();

    void Init(HWND hWnd);

    void SwapBuffers();
    void SetViewport(RECT& rect);
    void SetViewport(int x, int y, int width, int height);

    HWND GetHWnd();
    bool IsInitialized() { return m_initialized; }
  private:
    HWND m_hWnd;
    HGLRC m_openGLRenderingContext;
    HDC m_windowDeviceContext;
    bool m_initialized;
  };
}

