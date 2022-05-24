#pragma once

namespace Wallpaper {
  class OpenGL
  {
  public:
    OpenGL(HWND hWnd);
    ~OpenGL();

    void SwapBuffers();

    bool IsInitialized() { return m_initialized; }

  private:
    HGLRC m_openGLRenderingContext;
    HDC m_windowDeviceContext;
    bool m_initialized;
  };
}

