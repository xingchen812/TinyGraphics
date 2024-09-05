#include <tg/utils.h>
#include <ui/MainWindow.h>

#include <QApplication>
#include <QFile>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	if (QFile file(tg::resourcePath() / "main.css"); file.open(QFile::ReadOnly | QFile::Text)) {
		app.setStyleSheet(QTextStream(&file).readAll());
	} else {
		throw tg_exception();
	}

	tg::ui::MainWindow w;
	w.show();
	auto ret = app.exec();

	spdlog::shutdown();

	return ret;
}
