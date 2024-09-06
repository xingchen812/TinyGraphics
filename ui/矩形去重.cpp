#include <tg/base.h>

namespace tg {
namespace {
class FixedCanvas2D : public Window {
public:
	FixedCanvas2D()
		: m_pixmap(m_width, m_height) {
		m_pixmap.fill(Qt::white);
	}

	auto init() -> void override {
		setFixedSize(m_width, m_height);
	}

	auto drawLine(const Point2& a, const Point2& b) {
		QPainter painter(&m_pixmap);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.setPen(QPen(Qt::black, 2));
		painter.drawLine(QLineF(a.x, a.y, b.x, b.y));
		update();
	}

	auto drawRect(const Point2& begin, float width, float height, float radians = 0.) {
		auto top_left = begin;
		auto top_right = begin + Point2(width, 0);
		auto bottom_left = begin + Point2(0, height);
		auto bottom_right = begin + Point2(width, height);
		if (equalF(radians, 0.)) {
			drawLine(top_left, bottom_left);
			drawLine(top_left, top_right);
			drawLine(top_right, bottom_right);
			drawLine(bottom_right, bottom_left);
		} else {
			auto rotated_top_left = top_left.rotated(top_left, radians);
			auto rotated_top_right = top_right.rotated(top_left, radians);
			auto rotated_bottom_left = bottom_left.rotated(top_left, radians);
			auto rotated_bottom_right = bottom_right.rotated(top_left, radians);
			drawLine(rotated_top_left, rotated_bottom_left);
			drawLine(rotated_top_left, rotated_top_right);
			drawLine(rotated_top_right, rotated_bottom_right);
			drawLine(rotated_bottom_right, rotated_bottom_left);
		}
	}

protected:
	auto paintEvent(QPaintEvent* event) -> void override {
		QPainter painter(this);
		painter.drawPixmap(0, 0, m_pixmap);
	}

	size_t m_width = 1000;
	size_t m_height = 1000;
	QPixmap m_pixmap;
};

class Impl : public FixedCanvas2D {
public:
	auto init() -> void override {
		FixedCanvas2D::init();
		drawRect({10, 10}, 100, 200);
		drawRect({80, 180}, 100, 200, degreesToRadians(30));
	}
};
TG_QUICK_WINDOW_REGISTER_2
} // namespace
} // namespace tg
