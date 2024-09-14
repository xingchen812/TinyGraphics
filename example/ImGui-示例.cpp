#include <tg/ui/window.h>

#include <imgui.h>

using namespace tg;

namespace {
class Impl : public ui::Window {
public:
	auto paint() -> void override {
		ImGui::ShowDemoWindow();
	}
};
TG_QUICK_WINDOW_REGISTER_2
} // namespace
