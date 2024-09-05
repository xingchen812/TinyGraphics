#pragma once
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace tg::ui {
class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
		auto* centralWidget = new QWidget(this);

		auto* layout = new QVBoxLayout(centralWidget);
		auto* line = new QHBoxLayout();
		line->addWidget(new QPushButton("hello"));

		layout->addLayout(line);

		centralWidget->setLayout(layout);

		setCentralWidget(centralWidget);

		setWindowTitle("TinyGraphics");
		resize(800, 600);
	}
};
} // namespace tg::ui
