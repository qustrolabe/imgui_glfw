#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// #include <GL/gl.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**) {
  // glewInit();
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

  const char* glsl_version = "#version 300 es";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window with graphics context
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(1200, 800, "imgui-test", NULL, NULL);
  glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
  glfwSetWindowAttrib(window, GLFW_FLOATING, GLFW_TRUE);

  if (window == NULL) return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return false;
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

  // Our state
  ImVec4 clear_color = ImVec4(0.45f, 0.4, 0.60f, 1.00f);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    {
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(ImVec2(400, 800));

      ImGuiWindowFlags window_flags = 0;
      window_flags |= ImGuiWindowFlags_NoTitleBar;
      // window_flags |= ImGuiWindowFlags_NoScrollbar;
      // window_flags |= ImGuiWindowFlags_MenuBar;
      window_flags |= ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoResize;
      window_flags |= ImGuiWindowFlags_NoCollapse;
      // window_flags |= ImGuiWindowFlags_NoNav;
      // window_flags |= ImGuiWindowFlags_NoBackground;
      // window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
      // window_flags |= ImGuiWindowFlags_UnsavedDocument;

      ImGui::Begin("Menu", nullptr, window_flags);

      if(ImGui::CollapsingHeader("Test 1")) {
      ImGui::Text("ImGui::Text(\" ... \");");
      ImGui::BulletText("ImGui::BulletText(\" ... \");");
  }

      ImGui::CollapsingHeader("Test 2");

      ImGui::End();
    }

    ImGui::ShowDemoWindow();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    {
      glViewport(400, 0, 800, 800);

      float vertices[] = {
          -0.5f, -0.5f, 0.0f,  // левая вершина
          0.5f,  -0.5f, 0.0f,  // правая вершина
          0.0f,  0.5f,  0.0f   // верхняя вершина
      };

      unsigned int VBO, VAO;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                            (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glBindVertexArray(0);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
