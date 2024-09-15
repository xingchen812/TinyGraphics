#include <tg/ui/window.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include <glfw/glfw3.h>

namespace tg::ui {
namespace {
auto init_imgui() {
    glfwSetErrorCallback([](int error, const char* description) {
        spdlog::error("GLFW error {}: {}", error, description);
        std::abort();
    });

    if (!glfwInit()) {
        spdlog::error("glfwInit error");
        std::abort();
    }

    // GL 3.2 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto* window = glfwCreateWindow(1000, 1000, "TinyGraphics", nullptr, nullptr);   // NOLINT
    if (!window) {
        spdlog::error("glfwCreateWindow error");
        std::abort();
    }
    glfwMakeContextCurrent(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    if (!ImGui::CreateContext()) {
        spdlog::error("imgui CreateContext error");
        std::abort();
    }
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = 0;
        style.Colors[ImGuiCol_WindowBg].w = 1;
    }

    float xscale;
    float yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    auto scale = std::max(xscale, yscale);
    style.ScaleAllSizes(scale);

    constexpr auto k_font_size = 20.F;
    if (!io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", k_font_size * scale, nullptr, io.Fonts->GetGlyphRangesChineseFull())) {
        spdlog::error("imgui AddFontFromFileTTF error");
        std::abort();
    }

    // Setup Platform/Renderer backends
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        spdlog::error("imgui ImGui_ImplGlfw_InitForOpenGL error");
        std::abort();
    }
    if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
        spdlog::error("imgui ImGui_ImplOpenGL3_Init error");
        std::abort();
    }
}

auto init_spdlog() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    auto logger = std::make_shared<spdlog::logger>("console", console_sink);
    spdlog::set_default_logger(logger);
    // [时间戳] [日志级别] [线程ID] 消息
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v");

    system("chcp 65001");   // NOLINT
}
}   // namespace

MainWindow::MainWindow() = default;

MainWindow::~MainWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(glfwGetCurrentContext());
    glfwTerminate();

    spdlog::shutdown();
}

auto MainWindow::main(int /*argc*/, char** /*argv*/) -> int {
    // init
    // reinterpret_cast<Window*>(m_main_window.get())->moveCurWindow(45, 200);
    // m_main_window->setCentralWidget(m_treeWidget);
    // m_main_window->setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));

    m_component_name = "TinyGraphics";
    m_name           = m_component_name;
    m_open           = true;

    init_spdlog();
    init_imgui();
    loadWindowsConfig();

    static ImVec4 clear_color = ImVec4(0.45F, 0.55F, 0.60F, 1.00F);   // NOLINT
    ImGuiIO&      io          = ImGui::GetIO();
    auto*         window      = glfwGetCurrentContext();

    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            constexpr auto k_ms = 10ULL;
            std::this_thread::sleep_for(std::chrono::milliseconds(k_ms));
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // show all windows
        paint();
        for (auto it = m_windows.begin(); it != m_windows.end(); it++) {
            if (it->second->m_open) {
                it->second->paint();
            }
            else {
                it = m_windows.erase(it);
                saveWindowsConfig();
            }
        }

        // Rendering
        ImGui::Render();
        int display_w;
        int display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    return 0;
}

auto MainWindow::paint() -> void {
    ImGui::Begin("TinyGraphics");
    if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen)) {
        constexpr auto k_padding = 20.F;
        ImGui::Indent(k_padding);
        for (auto& [name, c] : m_components) {
            if (ImGui::Selectable(name.c_str())) {
                createWindow(name);
            }
        }
        ImGui::Unindent(k_padding);
    }
    ImGui::End();
}

auto Window::paint() -> void {
    ImGui::Begin(m_name.c_str(), &m_open);
    impl_paint();
    ImGui::End();
}
}   // namespace tg::ui
