#include "pch.h"
#include "OpenGL.h"

namespace Wallpaper {
  OpenGL::OpenGL(HWND hWnd) {
    // https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_%28WGL%29
    PIXELFORMATDESCRIPTOR pfd =
    {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
      PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
      32,                   // Colordepth of the framebuffer.
      0, 0, 0, 0, 0, 0,
      0,
      0,
      0,
      0, 0, 0, 0,
      24,                   // Number of bits for the depthbuffer
      8,                    // Number of bits for the stencilbuffer
      0,                    // Number of Aux buffers in the framebuffer.
      PFD_MAIN_PLANE,
      0,
      0, 0, 0
    };

    m_windowDeviceContext = GetDC(hWnd);

    int  letWindowsChooseThisPixelFormat;
    letWindowsChooseThisPixelFormat = ChoosePixelFormat(m_windowDeviceContext, &pfd);
    SetPixelFormat(m_windowDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

    m_openGLRenderingContext = wglCreateContext(m_windowDeviceContext);
    wglMakeCurrent(m_windowDeviceContext, m_openGLRenderingContext);

    m_initialized = gladLoadGL();
  }

  OpenGL::~OpenGL() {
    wglDeleteContext(m_openGLRenderingContext);
  }

  void OpenGL::SwapBuffers() {
    ::SwapBuffers(m_windowDeviceContext);
  }
}