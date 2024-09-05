#include <tg/utils.h>
#include <ui/MainWindow.h>

#include <QApplication>
#include <QFile>
#include <spdlog/spdlog.h>

namespace tg {
namespace {
auto setStyleSheet(QApplication& app) {
	for (const auto& entry : std::filesystem::directory_iterator(tg::resourcePath() / "ui")) {
		if (!entry.is_regular_file() || entry.path().extension() != ".css") {
			continue;
		}
		QFile file(entry.path());
		if (!file.open(QFile::ReadOnly | QFile::Text)) {
			throw tg_exception();
		}
		app.setStyleSheet(QTextStream(&file).readAll());
	}
}

auto main(int argc, char** argv) {
	QApplication app(argc, argv);

	setStyleSheet(app);
	tg::ui::MainWindow w;
	w.show();
	auto ret = app.exec();

	spdlog::shutdown();
	return ret;
}
} // namespace
} // namespace tg

int main(int argc, char** argv) {
	return tg::main(argc, argv);
}
