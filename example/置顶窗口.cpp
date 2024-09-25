#include <tg/ui/window.h>

using namespace tg;

namespace {
class Impl : public ui::Window {
public:
    auto init() -> void override {
        registerEvent("置顶", [this]() {
            setWindowTop(true);
        });
        registerEvent("不置顶", [this]() {
            setWindowTop(false);
        });
    }
};
TG_QUICK_WINDOW_REGISTER_2
}   // namespace
