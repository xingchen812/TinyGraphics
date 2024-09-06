#pragma once
#include <QVector3D>
#include <array>
#include <cmath>
#include <limits>
#include <numbers>

namespace tg {

inline constexpr auto PI = std::numbers::pi;

template <typename ValueType>
inline constexpr auto equalF(ValueType a, auto b) {
	return std::abs(a - b) < std::numeric_limits<ValueType>::epsilon();
}

inline constexpr double degreesToRadians(double degrees) {
	return PI * (degrees / 180.);
}

namespace detail {
template <size_t PointSize = 3, size_t XIndex = 0, size_t YIndex = 1, size_t ZIndex = 2, typename ValueType = float>
class Point {
public:
	static_assert(PointSize == 2 || PointSize == 3);
	static_assert(XIndex >= 0 && XIndex <= 2);
	static_assert(YIndex >= 0 && YIndex <= 2);
	static_assert(ZIndex >= 0 && ZIndex <= 2);

	using value_t = ValueType;

	std::array<value_t, PointSize> m_data;
	value_t& x = m_data[XIndex];
	value_t& y = m_data[YIndex];
	value_t& z = (PointSize > 2) ? m_data[ZIndex] : m_data[0];

	constexpr Point() {
		x = 0.;
		y = 0.;
		if constexpr (PointSize > 2) {
			z = 0.;
		}
	}

	constexpr Point(value_t x, value_t y) {
		static_assert(PointSize == 2, "PointSize must be at least 2 for 2D points.");
		this->x = x;
		this->y = y;
	}

	constexpr Point(value_t x, value_t y, value_t z) {
		static_assert(PointSize == 3, "PointSize must be at least 3 for 3D points.");
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		if constexpr (PointSize == 3) {
			z = p.z;
		}
		return *this;
	}

	constexpr static auto fromQPoint(const QVector2D& p) {
		if constexpr (PointSize == 2) {
			return Point(p.x(), p.y());
		} else {
			static_assert(PointSize == 3, "Unsupported PointSize for QVector2D.");
			return Point(p.x(), p.y(), 0.);
		}
	}

	constexpr auto toQPoint() const {
		if constexpr (PointSize == 2) {
			return QVector2D(x, y);
		} else {
			return QVector3D(x, y, z);
		}
	}

	constexpr Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		if constexpr (PointSize > 2) {
			z += p.z;
		}
		return *this;
	}

	constexpr Point& operator-=(const Point& p) {
		x -= p.x;
		y -= p.y;
		if constexpr (PointSize > 2) {
			z -= p.z;
		}
		return *this;
	}

	constexpr Point& operator*=(value_t c) {
		x *= c;
		y *= c;
		if constexpr (PointSize > 2) {
			z *= c;
		}
		return *this;
	}

	constexpr Point& operator/=(value_t c) {
		x /= c;
		y /= c;
		if constexpr (PointSize > 2) {
			z /= c;
		}
		return *this;
	}

	friend constexpr bool operator==(const Point& p1, const Point& p2) {
		bool equal = equalF(p1.x, p2.x) && equalF(p1.y, p2.y);
		if constexpr (PointSize > 2) {
			equal = equal && equalF(p1.z, p2.z);
		} else {
			return equal;
		}
	}

	friend constexpr bool operator!=(const Point& p1, const Point& p2) {
		return !(p1 == p2);
	}

	friend constexpr Point operator+(const Point& p1, const Point& p2) {
		if constexpr (PointSize == 2) {
			return Point(p1.x + p2.x, p1.y + p2.y);
		} else {
			return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
		}
	}

	friend constexpr Point operator-(const Point& p1, const Point& p2) {
		if constexpr (PointSize == 2) {
			return Point(p1.x - p2.x, p1.y - p2.y);
		} else {
			return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		}
	}

	friend constexpr Point operator*(const Point& p, value_t c) {
		if constexpr (PointSize == 2) {
			return Point(p.x * c, p.y * c);
		} else {
			return Point(p.x * c, p.y * c, p.z * c);
		}
	}

	friend constexpr Point operator*(value_t c, const Point& p) {
		if constexpr (PointSize == 2) {
			return Point(p.x * c, p.y * c);
		} else {
			return Point(p.x * c, p.y * c, p.z * c);
		}
	}

	friend constexpr Point operator+(const Point& p) {
		return p;
	}

	friend constexpr Point operator-(const Point& p) {
		if constexpr (PointSize == 2) {
			return Point(-p.x, -p.y);
		} else {
			return Point(-p.x, -p.y, -p.z);
		}
	}

	friend constexpr Point operator/(const Point& p, value_t divisor) {
		if constexpr (PointSize == 2) {
			return Point(p.x / divisor, p.y / divisor);
		} else {
			return Point(p.x / divisor, p.y / divisor, p.z / divisor);
		}
	}

	constexpr Point abs() const {
		if constexpr (PointSize == 2) {
			return Point(std::abs(x), std::abs(y));
		} else {
			return Point(std::abs(x), std::abs(y), std::abs(z));
		}
	}

	constexpr Point cross(const Point& vec) const {
		static_assert(PointSize == 3, "Cross product is only defined for 3D points.");
		return Point(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	constexpr value_t distance_to(const Point& vec) const {
		if constexpr (PointSize == 2) {
			return std::sqrt(std::pow(x - vec.x, 2) + std::pow(y - vec.y, 2));
		} else {
			return std::sqrt(std::pow(x - vec.x, 2) + std::pow(y - vec.y, 2) + std::pow(z - vec.z, 2));
		}
	}

	constexpr value_t dot(const Point& vec) const {
		if constexpr (PointSize == 2) {
			return x * vec.x + y * vec.y;
		} else {
			return x * vec.x + y * vec.y + z * vec.z;
		}
	}

	constexpr value_t length() const {
		if constexpr (PointSize == 2) {
			return std::sqrt(x * x + y * y);
		} else {
			return std::sqrt(x * x + y * y + z * z);
		}
	}

	constexpr Point linear_interpolate(const Point& vec, value_t t) const {
		if constexpr (PointSize == 2) {
			return Point(x + t * (vec.x - x), y + t * (vec.y - y));
		} else {
			return Point(x + t * (vec.x - x), y + t * (vec.y - y), z + t * (vec.z - z));
		}
	}

	constexpr Point normalized() const {
		value_t len = length();
		if constexpr (len != 0) {
			if constexpr (PointSize == 2) {
				return Point(x / len, y / len);
			} else {
				return Point(x / len, y / len, z / len);
			}
		} else {
			return *this;
		}
	}

	constexpr void normalize() {
		value_t len = length();
		if constexpr (len != 0) {
			if constexpr (PointSize == 2) {
				x /= len;
				y /= len;
			} else {
				x /= len;
				y /= len;
				z /= len;
			}
		}
	}

	constexpr Point rotated(const Point& axis, value_t radians) const {
		static_assert(PointSize == 2, "Rotation is only defined for 2D points.");
		value_t cos_angle = std::cos(radians);
		value_t sin_angle = std::sin(radians);
		Point relative = *this - axis;
		return Point(
			relative.x * cos_angle - relative.y * sin_angle + axis.x,
			relative.x * sin_angle + relative.y * cos_angle + axis.y);
	}

	constexpr void rotate(const Point& axis, value_t radians) {
		*this = rotated(axis, radians);
	}

	constexpr value_t angle_to(const Point& vec) const {
		value_t dot_product = dot(vec);
		value_t lengths_product = length() * vec.length();
		return lengths_product != 0 ? std::acos(dot_product / lengths_product) : 0;
	}

	inline Point direction_to(const Point& other) const {
		return (other - *this).normalized();
	}
};
} // namespace detail

using Point2 = detail::Point<2>;
using Point3 = detail::Point<3>;
} // namespace tg
