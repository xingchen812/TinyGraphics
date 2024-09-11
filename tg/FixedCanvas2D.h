#pragma once
#include <tg/base.h>

namespace tg {
class FixedCanvas2D : public Window {
public:
	FixedCanvas2D()
		: m_pixmap(m_width, m_height) {
		m_pixmap.fill(Qt::white);
	}

	auto init() -> void override {
		setFixedSize(m_width, m_height);
	}

	auto drawLine(const Point2& a, const Point2& b, const QColor& color) {
		QPainter painter(&m_pixmap);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.setCompositionMode(QPainter::CompositionMode_Multiply);
		painter.setPen(QPen(color, 2));
		painter.drawLine(QLineF(a.x, a.y, b.x, b.y));
		update();
	}

	auto drawPolygon(const std::vector<Point2>& points, const QColor& color, float radians = 0, bool connect_first_last = true) {
		if (points.empty()) {
			return;
		}

		if (equalF(radians, 0)) {
			for (size_t i = 0; i < points.size() - 1; ++i) {
				drawLine(points[i], points[i + 1], color);
			}
			if (connect_first_last) {
				drawLine(points.back(), points.front(), color);
			}
			return;
		}

		const auto& pivot = points[0];
		auto pre = pivot;
		for (size_t i = 1; i < points.size(); ++i) {
			auto next = points[i].rotated(pivot, radians);
			drawLine(pre, next, color);
			pre = next;
		}
		if (connect_first_last) {
			drawLine(pre, pivot, color);
		}
	}

	auto drawRect(const Point2& begin, float width, float height, float radians = 0) {
		drawPolygon({begin, begin + Point2(width, 0), begin + Point2(width, height), begin + Point2(0, height)}, radians, true);
	}

	auto drawBackground(const QColor& color = Qt::white) {
		m_pixmap.fill(color);
		update();
	}

protected:
	auto paintEvent(QPaintEvent* event) -> void override {
		QPainter painter(this);
		painter.drawPixmap(0, 0, m_pixmap);
	}

	size_t m_width = 1200;
	size_t m_height = 1000;
	QPixmap m_pixmap;
};
} // namespace tg
