#pragma once
#include <tg/utils.h>

#include <cmath>

namespace tg {
namespace detail {
template <size_t PointSize = 3, typename ValueType = float>
class Point {
public:
	static_assert(PointSize >= 2 && PointSize <= 4);

	using value_t = ValueType;

	value_t x = 0;
	value_t y = 0;
	value_t z = 0;
	value_t w = 0;

	constexpr Point() = default;

	constexpr Point(value_t x, value_t y)
		requires(PointSize == 2)
	{
		this->x = x;
		this->y = y;
	}

	constexpr Point(value_t x, value_t y, value_t z)
		requires(PointSize == 3)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	constexpr Point(value_t x, value_t y, value_t z, value_t w)
		requires(PointSize == 4)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	constexpr Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		if constexpr (PointSize > 2) {
			z = p.z;
		}
		if constexpr (PointSize > 3) {
			w = p.w;
		}
		return *this;
	}

	constexpr Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		if constexpr (PointSize > 2) {
			z += p.z;
		}
		if constexpr (PointSize > 3) {
			w += p.w;
		}
		return *this;
	}

	constexpr Point& operator-=(const Point& p) {
		x -= p.x;
		y -= p.y;
		if constexpr (PointSize > 2) {
			z -= p.z;
		}
		if constexpr (PointSize > 3) {
			w -= p.w;
		}
		return *this;
	}

	constexpr Point& operator*=(value_t c) {
		x *= c;
		y *= c;
		if constexpr (PointSize > 2) {
			z *= c;
		}
		if constexpr (PointSize > 3) {
			w *= c;
		}
		return *this;
	}

	constexpr Point& operator/=(value_t c) {
		x /= c;
		y /= c;
		if constexpr (PointSize > 2) {
			z /= c;
		}
		if constexpr (PointSize > 3) {
			w /= c;
		}
		return *this;
	}

	friend constexpr bool operator==(const Point& p1, const Point& p2) {
		bool equal = equalF(p1.x, p2.x) && equalF(p1.y, p2.y);
		if constexpr (PointSize > 2) {
			equal = equal && equalF(p1.z, p2.z);
		}
		if constexpr (PointSize > 3) {
			equal = equal && equalF(p1.w, p2.w);
		}
		return equal;
	}

	friend constexpr bool operator!=(const Point& p1, const Point& p2) {
		return !(p1 == p2);
	}

	friend constexpr Point operator+(const Point& p1, const Point& p2) {
		if constexpr (PointSize == 2) {
			return Point(p1.x + p2.x, p1.y + p2.y);
		} else if constexpr (PointSize == 3) {
			return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
		} else if constexpr (PointSize == 4) {
			return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z, p1.w + p2.w);
		}
	}

	friend constexpr Point operator-(const Point& p1, const Point& p2) {
		if constexpr (PointSize == 2) {
			return Point(p1.x - p2.x, p1.y - p2.y);
		} else if constexpr (PointSize == 3) {
			return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		} else if constexpr (PointSize == 4) {
			return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z, p1.w - p2.w);
		}
	}

	friend constexpr Point operator*(const Point& p, value_t c) {
		if constexpr (PointSize == 2) {
			return Point(p.x * c, p.y * c);
		} else if constexpr (PointSize == 3) {
			return Point(p.x * c, p.y * c, p.z * c);
		} else if constexpr (PointSize == 4) {
			return Point(p.x * c, p.y * c, p.z * c, p.w * c);
		}
	}

	friend constexpr Point operator*(value_t c, const Point& p) {
		return p * c;
	}

	friend constexpr Point operator+(const Point& p) {
		return p;
	}

	friend constexpr Point operator-(const Point& p) {
		if constexpr (PointSize == 2) {
			return Point(-p.x, -p.y);
		} else if constexpr (PointSize == 3) {
			return Point(-p.x, -p.y, -p.z);
		} else if constexpr (PointSize == 4) {
			return Point(-p.x, -p.y, -p.z, -p.w);
		}
	}

	friend constexpr Point operator/(const Point& p, value_t divisor) {
		if constexpr (PointSize == 2) {
			return Point(p.x / divisor, p.y / divisor);
		} else if constexpr (PointSize == 3) {
			return Point(p.x / divisor, p.y / divisor, p.z / divisor);
		} else if constexpr (PointSize == 4) {
			return Point(p.x / divisor, p.y / divisor, p.z / divisor, p.w / divisor);
		}
	}

	constexpr auto abs() const {
		if constexpr (PointSize == 2) {
			return Point(std::abs(x), std::abs(y));
		} else if constexpr (PointSize == 3) {
			return Point(std::abs(x), std::abs(y), std::abs(z));
		} else if constexpr (PointSize == 4) {
			return Point(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
		}
	}

	constexpr auto cross(const Point& vec) const
		requires(PointSize == 3)
	{
		return Point(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	constexpr value_t distance_to(const Point& vec) const
		requires(PointSize == 2 || PointSize == 3)
	{
		if constexpr (PointSize == 2) {
			return std::sqrt(std::pow(x - vec.x, 2) + std::pow(y - vec.y, 2));
		} else if constexpr (PointSize == 3) {
			return std::sqrt(std::pow(x - vec.x, 2) + std::pow(y - vec.y, 2) + std::pow(z - vec.z, 2));
		}
	}

	constexpr value_t dot(const Point& vec) const
		requires(PointSize == 2 || PointSize == 3)
	{
		if constexpr (PointSize == 2) {
			return x * vec.x + y * vec.y;
		} else if constexpr (PointSize == 3) {
			return x * vec.x + y * vec.y + z * vec.z;
		}
	}

	constexpr value_t length() const
		requires(PointSize == 2 || PointSize == 3)
	{
		if constexpr (PointSize == 2) {
			return std::sqrt(x * x + y * y);
		} else if constexpr (PointSize == 3) {
			return std::sqrt(x * x + y * y + z * z);
		}
	}

	constexpr auto linear_interpolate(const Point& vec, value_t t) const
		requires(PointSize == 2 || PointSize == 3)
	{
		if constexpr (PointSize == 2) {
			return Point(x + t * (vec.x - x), y + t * (vec.y - y));
		} else if constexpr (PointSize == 3) {
			return Point(x + t * (vec.x - x), y + t * (vec.y - y), z + t * (vec.z - z));
		}
	}

	constexpr auto rotate(const Point& axis, value_t angle)
		requires(PointSize == 2 || PointSize == 3)
	{
		value_t c = std::cos(angle);
		value_t s = std::sin(angle);

		Point rotated;
		rotated.x = (c + (1 - c) * axis.x * axis.x) * x + ((1 - c) * axis.x * axis.y - axis.z * s) * y + ((1 - c) * axis.x * axis.z + axis.y * s) * z;
		rotated.y = ((1 - c) * axis.x * axis.y + axis.z * s) * x + (c + (1 - c) * axis.y * axis.y) * y + ((1 - c) * axis.y * axis.z - axis.x * s) * z;
		if constexpr (PointSize > 3) {
			rotated.z = ((1 - c) * axis.x * axis.z - axis.y * s) * x + ((1 - c) * axis.y * axis.z + axis.x * s) * y + (c + (1 - c) * axis.z * axis.z) * z;
		}

		x = rotated.x;
		y = rotated.y;
		if constexpr (PointSize > 3) {
			z = rotated.z;
		}
	}
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point<2>, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point<3>, x, y, z)
} // namespace detail

using Point2 = detail::Point<2>;
using Point3 = detail::Point<3>;
using PointInt2 = detail::Point<2, int>;

inline constexpr auto Point2To3(const Point2& p) {
	return Point3(p.x, 0, p.y);
}

inline constexpr auto Point2To3(const std::vector<Point2>& p) {
	std::vector<Point3> res;
	res.resize(p.size());
	for (size_t i = 0; i < p.size(); i++) {
		res[i] = Point2To3(p[i]);
	}
	return res;
}

inline constexpr auto Point3To2(const Point3& p) {
	return Point2(p.x, p.z);
}

inline constexpr auto Point3To2(const std::vector<Point3>& p) {
	std::vector<Point2> res;
	res.resize(p.size());
	for (size_t i = 0; i < p.size(); i++) {
		res[i] = Point3To2(p[i]);
	}
	return res;
}
} // namespace tg

template <>
struct fmt::formatter<tg::Point3> : fmt::formatter<std::string> {
	auto format(const tg::Point3& v, format_context& ctx) const -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", v.x, v.y, v.z);
	}
};

template <>
struct fmt::formatter<tg::Point2> : fmt::formatter<std::string> {
	auto format(const tg::Point2& v, format_context& ctx) const -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{{{}, {}}}", v.x, v.y);
	}
};

template <typename T>
struct fmt::formatter<std::vector<T>> : fmt::formatter<std::string> {
	auto format(const std::vector<T>& v, format_context& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "[{}]", fmt::join(v, ", "));
	}
};
