#include <tg/base.h>

namespace tg {
namespace {
class Impl : public Window {
public:
	auto init() -> void override {
		spdlog::info("示例初始化成功");
	}
};
TG_QUICK_WINDOW_REGISTER_2
} // namespace
} // namespace tg
