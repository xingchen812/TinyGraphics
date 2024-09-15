#include <tg/ui/window.h>

#include <imgui.h>

using namespace tg;

namespace {
class Impl : public ui::Window {
public:
    auto init() -> void override {
        spdlog::info("示例初始化成功");
        registerEvent("日志事件", []() {
            spdlog::info("日志事件触发");
        });
    }

    auto impl_paint() -> void override {
        ImGui::Text("Hello World!");
        ImGui::Text("你好，世界！");
    }
};
TG_QUICK_WINDOW_REGISTER_2
}   // namespace
