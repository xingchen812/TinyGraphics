#include <tg/ui/window.h>

#include <GLFW/glfw3.h>
#include <imgui.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

using namespace tg;

namespace {
class Impl : public ui::Window {
public:
    auto setImGuiWindowsTopmost() const {
        auto& io       = ImGui::GetPlatformIO();
        auto* viewport = ImGui::FindViewportByID(id);
        if (!viewport) {
            return;
        }
        auto* w = static_cast<GLFWwindow*>(viewport->PlatformHandle);
        if (!w) {
            return;
        }
        auto* hwnd = glfwGetWin32Window(w);
        if (!hwnd) {
            return;
        }
        if (!SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER)) {
            spdlog::error("SetWindowPos error: {}", getSystemLastErrorAsString());
        }
    }

    auto init() -> void override {
    }

    auto impl_paint() -> void override {
        id = ImGui::GetWindowViewport()->ID;
    }

    auto afterAllPaint() -> void override {
        setImGuiWindowsTopmost();
    }

    unsigned int id = -1;
};
TG_QUICK_WINDOW_REGISTER_2
}   // namespace
