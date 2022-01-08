#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-glfw.h>
#include <imgui_freetype.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "core.h"
#include "TextEditor.h"
#include "GeneralTable.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

#ifdef __APPLE__
void AddSystemFont() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("/System/Library/Fonts/SFNS.ttf", 18);
}
#elif WIN32
void AddSystemFont() {
    ImGuiIO& io = ImGui::GetIO();
    ImWchar range[] = { 0, 0x04FF, 0 };
    ImFontConfig config;
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Segoeui.ttf", 16, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImWchar range_emj[] = {0x1 , 0x1FFFF, 0};
    static ImFontConfig cfg;
    cfg.OversampleH = cfg.OversampleV = 1;
    cfg.MergeMode = true;
    cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Seguiemj.ttf", 16, &cfg, range_emj);
    io.Fonts->Build();
}
#else
void AddSystemFont() {

}
#endif

#ifdef _MSC_VER
#include <windows.h>
#include <shellapi.h>
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPWSTR, INT)
{
    UNREFERENCED_PARAMETER(hInst);
    UNREFERENCED_PARAMETER(hPrevInstance);
    int argc;
    char** argv;
    {
        LPWSTR* lpArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
        argv = (char**)malloc(argc * sizeof(char*));
        int size, i = 0;
        for (; i < argc; ++i)
        {
            size = wcslen(lpArgv[i]) + 1;
            argv[i] = (char*)malloc(size);
            wcstombs(argv[i], lpArgv[i], size);
        }
        LocalFree(lpArgv);
    }

#else
int main(int argc, char* argv[])
{
#endif
       // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui + GLFW example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    if (!ImGui::GLFW::Init(window)) {
        return -1;
    }
    AddSystemFont();
    med_calc::GeneralWindow table;
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //ImGui::UpdatePlatformWindows();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
   
    
    bool isFirst = true;
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      ImGui::GLFW::NewFrame();
      ImGui::ShowDemoWindow();
      static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

      // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
      // because it would be confusing to have two docking targets within each others.
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiDockNodeFlags_NoDockingInCentralNode;


      // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
      if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
          window_flags |= ImGuiWindowFlags_NoBackground;

      // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
      // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
      // all active windows docked into it will lose their parent and become undocked.
      // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
      // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("DockSpace", nullptr, window_flags);
      ImGui::PopStyleVar();
      ImGui::PopStyleVar(2);

      // DockSpace
      ImGuiIO& io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
      {
          ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
          ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags | ImGuiDockNodeFlags_NoDockingInCentralNode);

          static auto first_time = true;
          if (first_time)
          {
              first_time = false;

              ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
              ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
              ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

              // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
              //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
              //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
              auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
              auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);

              // we now dock our windows into the docking node we made above
              //ImGui::DockBuilderDockWindow("Down", dock_id_down);
              ImGui::DockBuilderDockWindow("GeneraWindowContainer", dock_id_left);
              auto res = ImGui::DockBuilderGetCentralNode(dock_id_down);
              ImGui::DockBuilderFinish(dockspace_id);
          }
      }

      ImGui::End();

      table.Update();

      ImGui::Begin("Down");
      ImGui::Text("Hello, down!");
      ImGui::End();
      ImGui::Render();
      ImGui::GLFW::Render(window);
      glfwSwapBuffers(window);
   }
   ImGui::GLFW::Shutdown();

   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;

}
