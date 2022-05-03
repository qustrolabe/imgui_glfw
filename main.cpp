#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// #include <GL/gl.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#if 1

ImVec4 clear_color = ImVec4(0.45f, 0.4f, 0.60f, 1.00f);
ImVec4 main_color = ImVec4(0.45f, 0.9f, 0.60f, 1.00f);

GLuint VBO, VAO;

class FramebufferBinding {
  GLuint id;

 public:
  FramebufferBinding(GLuint id_) : id{id_} {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
  }

  ~FramebufferBinding() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  // Example usage:
  // Framebuffer fbo;
  // {
  //    auto binding = fbo.bind();
  //    ...
  //    ...
  // //Auto unbinding at the end of scope
  // }
};

class Framebuffer {
  GLuint id;

  int width = 512;
  int height = 512;

  GLuint framebuffer;
  GLuint depthRenderbuffer;
  GLuint colorRenderbuffer;

 public:
  Framebuffer() {
    glGenFramebuffers(1, &id);

    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, colorRenderbuffer);

    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthRenderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    {
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    {
      // glViewport(0, 0, width, height);

      glBindFramebuffer(GL_FRAMEBUFFER, id);

      GLfloat vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                            0.0f,  0.0f,  0.5f, 0.0f};

      int size = 3;

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                            (void*)0);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  }
  ~Framebuffer() { glDeleteFramebuffers(1, &id); }

  auto bind() { return FramebufferBinding(id); }
};

class Scene {
  // - unique name ("Scene - 001")
  // - Scene

  Framebuffer fbo;

  GLuint texture;

 public:
  Scene(int width = 512, int height = 512) {
    auto binding = fbo.bind();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);
  }
  ~Scene() {
    // Delete texture
  }

  void render() {
    ImGui::Begin("Scene");
    if (ImGui::BeginChild("GameRender")) {
      ImGui::Image(reinterpret_cast<ImTextureID>(texture),
                   ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::EndChild();
    ImGui::End();

    {
      glViewport(0, 0, 512, 512);
      glRotatef(1, 0.1, 0.4, 0.3);

      int size = 3;

      {
        auto binding = fbo.bind();

        glBindVertexArray(VAO);

        glClearColor(clear_color.x, clear_color.y,
                     clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
      }
    }
  }
};
#endif

class Window {
  GLFWwindow* window = nullptr;

 public:
  Window();
  ~Window();

  void loop() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    int width = 512;
    int height = 512;

    Scene scn;

    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();

      ImGui::NewFrame();

      {
        {
          ImGuiWindowFlags window_flags = 0;

          ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

          const ImGuiViewport* viewport = ImGui::GetMainViewport();
          ImGui::SetNextWindowPos(viewport->WorkPos);
          ImGui::SetNextWindowSize(viewport->WorkSize);
          ImGui::SetNextWindowViewport(viewport->ID);

          ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
          ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
          ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

          window_flags |= ImGuiWindowFlags_NoTitleBar |
                          ImGuiWindowFlags_NoCollapse |
                          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

          window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                          ImGuiWindowFlags_NoNavFocus;

          ImGui::Begin("DockSpace Demo", nullptr, window_flags);
          ImGuiID dockspace_id = ImGui::GetID("s1");
          ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

          ImGui::End();

          ImGui::PopStyleVar(3);
        }

        // ImGui::SetNextWindowPos(ImVec2(0, 0));
        // ImGui::SetNextWindowSize(ImVec2(400, 800));

        ImGuiWindowFlags window_flags = 0;
        // window_flags |= ImGuiWindowFlags_NoTitleBar;
        // window_flags |= ImGuiWindowFlags_NoMove;
        // window_flags |= ImGuiWindowFlags_NoResize;
        // window_flags |= ImGuiWindowFlags_NoCollapse;

        // window_flags |= ImGuiWindowFlags_NoScrollbar;
        // window_flags |= ImGuiWindowFlags_MenuBar;
        // window_flags |= ImGuiWindowFlags_NoNav;
        // window_flags |= ImGuiWindowFlags_NoBackground;
        // window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        // window_flags |= ImGuiWindowFlags_UnsavedDocument;

        ImGui::Begin("Menu", nullptr, window_flags);

        ImGui::SliderInt("slider width", &width, 1, 800);
        ImGui::SliderInt("slider height", &height, 1, 800);

        if (ImGui::CollapsingHeader("Test 1")) {
          ImGui::Text("ImGui::Text(\" ... \");");
          ImGui::BulletText("ImGui::BulletText(\" ... \");");
        }

        if (ImGui::CollapsingHeader("Test 2")) {
          ImGui::ColorEdit3("clear color", (float*)&clear_color);
          ImGui::ColorEdit3("main color", (float*)&main_color);
        }

        ImGui::End();
      }

      ImGui::ShowDemoWindow();

      // Rendering

      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      // glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);

      glColor4f(main_color.x, main_color.y, main_color.z, main_color.w);

      scn.render();

      // glViewport(400, 0, 800, 800);

      // ImGui::Begin("Scene");
      // if (ImGui::BeginChild("GameRender")) {
      //   // Because I use the texture from OpenGL, I need to invert the V from
      //   // the UV.
      //   ImGui::Image(reinterpret_cast<ImTextureID>(texture),
      //                ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
      // }
      // ImGui::EndChild();
      // ImGui::End();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
    }
  }
};

class App {
  Window mainWindow;

 public:
  App();
  ~App();
};

Window::Window() {
  // glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) throw;

  const char* glsl_version = "#version 300 es";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window with graphics context
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(1200, 800, "imgui-test", NULL, NULL);
  if (window == NULL) throw;

  glfwMakeContextCurrent(window);
  GLenum err = glewInit();

  glfwSwapInterval(1);  // Enable vsync

  if (GLEW_OK != err) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    throw;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

Window::~Window() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

App::App() { mainWindow.loop(); }

App::~App() {}

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**) {
  // App app;

  Window w;

  w.loop();

  return 0;
}
