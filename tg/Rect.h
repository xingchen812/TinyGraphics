#pragma once
#include <tg/base.h>

namespace tg {
class Rect {
public:
	Rect() = default;

	Rect(float x, float y, float width, float height, float radians = 0)
		: m_points({Point2{x, y}, Point2{x, y} + Point2(width, 0), Point2{x, y} + Point2(width, height), Point2{x, y} + Point2(0, height)}), m_width(width), m_height(height), m_radians(radians) {
		if (!equalF(m_radians, 0)) {
			for (size_t i = 1; i < m_points.size(); ++i) {
				m_points[i].rotate(m_points[0], radians);
			}
		}
	}

	auto points() const -> const std::vector<Point2>& {
		return m_points;
	}

	auto width() const {
		return m_width;
	}

	auto height() const {
		return m_height;
	}

	auto radians() const {
		return m_radians;
	}

	auto topLeft() const -> const Point2& {
		return m_points[0];
	}

	auto topRight() const -> const Point2& {
		return m_points[1];
	}

	auto bottomRight() const -> const Point2& {
		return m_points[2];
	}

	auto bottomLeft() const -> const Point2& {
		return m_points[3];
	}

	auto rotated(float radians) {
		if (equalF(radians, 0)) {
			return *this;
		}
		return Rect(topLeft().x, topLeft().y, width(), height(), m_radians + radians);
	}

	auto moved(float x, float y, float radians = 0) {
		auto rect = rotated(radians);
		for (size_t i = 0; i < rect.m_points.size(); i++) {
			rect.m_points[i] += Point2(x, y);
		}
		return rect;
	}

	auto isValid() const {
		return !equalF(width(), 0) && !equalF(height(), 0);
	}

	auto resize(float w, float h) {
		*this = Rect(topLeft().x, topLeft().y, w, h, radians());
	}

private:
	std::vector<Point2> m_points = std::vector<Point2>(4, Point2(0, 0));
	float m_width = 0;
	float m_height = 0;
	float m_radians = 0;
};
} // namespace tg
