#include "pch.h"
#include "Utils/ConsoleHelper.h"
#include "Utils/Utils.h"
#include "Graphics/OpenGL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  ConsoleHelper::InitConsole();
  // std::wofstream file;
  // file.open("cout.txt");
  // std::wstreambuf* sbuf = std::wcout.rdbuf();
  // std::wcout.rdbuf(file.rdbuf());

  std::wcout << "get_wallpaper_window(): " << Wallpaper::Utils::GetWallpaperWindow() << std::endl;
  std::wcout << "GetDesktopWindow(): " << GetDesktopWindow() << std::endl;
  std::wcout << "get_wallpaper_window(): " << Wallpaper::Utils::GetWallpaperWindow() << std::endl;
  std::wcout << "GetDesktopWindow(): " << GetDesktopWindow() << std::endl;
  std::wcout << "GetShellWindow(): " << GetShellWindow() << std::endl;
  auto hwnd = Wallpaper::Utils::GetWallpaperWindow();
  HDC screenDC = GetDC(hwnd); //NULL gets whole screen
  for (int i = 0; i < 1; i++) {
    // resetWallpaper();

    std::wcout << "screenDC(): " << screenDC << std::endl;
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0)); //create brush
    SelectObject(screenDC, brush); //select brush into DC
    Rectangle(screenDC, 0, 0, 0 + 1000, 1000); //draw rectangle over whole screen
    //clean up stuff here

    const WCHAR* text = L"AbcdEFGhiJK@";
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

    /*{
      HBRUSH brush2 = CreateSolidBrush(RGB(255, 0, 0)); //create brush
      SelectObject(screenDC, brush2); //select brush into DC
      SetBkMode(screenDC, TRANSPARENT);
      SetTextColor(screenDC, RGB(255, 0, 0));
      RECT r{ 100, 100, 1000, 1000 };
      std::cout << DrawText(screenDC, text, wcslen(text), &r, DT_CENTER) << std::endl;
    }*/
    /*{
      ReleaseDC(hwnd, screenDC);
      PAINTSTRUCT ps;
      auto hdc = BeginPaint(hwnd, &ps);
      SetTextColor(hdc, 0x00FFFFFF);
      auto t = TEXT("TEXT");
      TextOut(hdc, 100, 100, t, lstrlen(t));
      EndPaint(hwnd, &ps);
    }*/
    if(true) {
      Wallpaper::OpenGL openGL(hwnd);
      if (openGL.IsInitialized())
      {
        std::cout << "Succeded to initialize OpenGL context" << std::endl;
      }
      else {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        while (true);
        return -1;
      }
      Wallpaper::Utils::PrintWindowSize(hwnd);

      std::cout << (char*)glGetString(GL_VERSION) << std::endl;
      // MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);

      {
        const int width = 1920, height = 1080;
        glViewport(0, 0, width, height);
      }

      const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"
        "\n"
        "out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "	gl_Position = vec4(aPos, 1.0);\n"
        "	ourColor = aColor;\n"
        "	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "}\n";
      const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "\n"
        "// texture samplers\n"
        "uniform sampler2D texture1;\n"
        "\n"
        "void main()\n"
        "{\n"
        "// linearly interpolate between both textures (80% container, 20% awesomeface)\n"
        "FragColor = texture(texture1, TexCoord);\n"
        "}\n";

      // build and compile our shader program
      // ------------------------------------
      // vertex shader
      unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
      glCompileShader(vertexShader);
      // check for shader compile errors
      int success;
      char infoLog[512];
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      }
      // fragment shader
      unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
      glCompileShader(fragmentShader);
      // check for shader compile errors
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      }
      // link shaders
      unsigned int shaderProgram = glCreateProgram();
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
      glLinkProgram(shaderProgram);
      // check for linking errors
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      }
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      // set up vertex data (and buffer(s)) and configure vertex attributes
      // ------------------------------------------------------------------
      /*float vertices[] = {
           0.5f,  0.5f, 0.0f,  // top right
           0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f,  // bottom left
          -0.5f,  0.5f, 0.0f   // top left 
      };*/
      float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
      };
      unsigned int indices[] = {  // note that we start from 0!
          0, 1, 3,  // first Triangle
          1, 2, 3   // second Triangle
      };
      unsigned int VBO, VAO, EBO;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      // color attribute
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
      // texture coord attribute
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);

      // load and create a texture 
    // -------------------------
      unsigned int texture1, texture2;
      // texture 1
      // ---------
      glGenTextures(1, &texture1);
      glBindTexture(GL_TEXTURE_2D, texture1);
      // set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      // set texture filtering parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // load image, create texture and generate mipmaps
      int width, height, nrChannels;
      stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
      // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
      unsigned char* data = stbi_load("resources/textures/background.jpg", &width, &height, &nrChannels, 0);
      if (data)
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
      else
      {
        std::cout << "Failed to load texture" << std::endl;
      }
      stbi_image_free(data);

      // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
      // -------------------------------------------------------------------------------------------
      glUseProgram(shaderProgram); // don't forget to activate/use the shader before setting uniforms!
      // either set it manually like so:
      glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
      // or set it via the texture class
      glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);




      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // bind textures on corresponding texture units
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);

      // render container
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);





      openGL.SwapBuffers();
      openGL.SwapBuffers();
      openGL.SwapBuffers();
      // SwapBuffers(ourWindowHandleToDeviceContext);

      openGL.~OpenGL();
      // wglDeleteContext(ourOpenGLRenderingContext);

      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);
      glDeleteProgram(shaderProgram);
    }
  }

  std::string input;
  do {
    std::cin >> input;
    if (input == "stop" || input == "exit" || input == "quit" || input == "q") {
      break;
    }
    else if (input == "reset") {
      resetWallpaper();
    }
    else if (input == "rq") {
      resetWallpaper();
      break;
    }
  } while (true);

  return 0;
}
