#include <tg/ui/window.h>

auto main(int argc, char** argv) -> int {
    return tg::ui::MainWindow::getInstance().main(argc, argv);
}
