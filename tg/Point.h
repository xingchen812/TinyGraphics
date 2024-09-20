#pragma once
#include <tg/utils.h>

#include <cmath>

namespace tg {
namespace detail {
template <size_t PointSize, typename ValueType>
class PointValue;

template <typename ValueType>
class PointValue<2, ValueType> {
public:
    ValueType x = 0;
    ValueType y = 0;
};

template <typename ValueType>
class PointValue<3, ValueType> {
public:
    ValueType x = 0;
    ValueType y = 0;
    ValueType z = 0;
};

template <typename ValueType>
class PointValue<4, ValueType> {
public:
    ValueType x = 0;
    ValueType y = 0;
    ValueType z = 0;
    ValueType w = 0;
};

template <size_t PointSize = 3, typename ValueType = float>
class Point : public PointValue<PointSize, ValueType> {   // NOLINT
public:
    static_assert(PointSize >= 2 && PointSize <= 4);

    using value_t     = ValueType;

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

    constexpr auto operator+=(const Point& p) -> Point& {
        this->x += p.x;
        this->y += p.y;
        if constexpr (PointSize > 2) {
            this->z += p.z;
        }
        if constexpr (PointSize > 3) {
            this->w += p.w;
        }
        return *this;
    }

    constexpr auto operator-=(const Point& p) -> Point& {
        this->x -= p.x;
        this->y -= p.y;
        if constexpr (PointSize > 2) {
            this->z -= p.z;
        }
        if constexpr (PointSize > 3) {
            this->w -= p.w;
        }
        return *this;
    }

    constexpr auto operator*=(value_t c) -> Point& {
        this->x *= c;
        this->y *= c;
        if constexpr (PointSize > 2) {
            this->z *= c;
        }
        if constexpr (PointSize > 3) {
            this->w *= c;
        }
        return *this;
    }

    constexpr auto operator/=(value_t c) -> Point& {
        this->x /= c;
        this->y /= c;
        if constexpr (PointSize > 2) {
            this->z /= c;
        }
        if constexpr (PointSize > 3) {
            this->w /= c;
        }
        return *this;
    }

    friend constexpr auto operator==(const Point& p1, const Point& p2) -> bool {
        if constexpr (std::is_floating_point_v<value_t>) {
            bool equal = equalF(p1.x, p2.x) && equalF(p1.y, p2.y);
            if constexpr (PointSize > 2) {
                equal = equal && equalF(p1.z, p2.z);
            }
            if constexpr (PointSize > 3) {
                equal = equal && equalF(p1.w, p2.w);
            }
            return equal;
        }
        else {
            bool equal = p1.x == p2.x && p1.y == p2.y;
            if constexpr (PointSize > 2) {
                equal = equal && p1.z == p2.z;
            }
            if constexpr (PointSize > 3) {
                equal = equal && p1.w == p2.w;
            }
            return equal;
        }
    }

    friend constexpr auto operator!=(const Point& p1, const Point& p2) -> bool {
        return !(p1 == p2);
    }

    friend constexpr auto operator+(const Point& p1, const Point& p2) -> Point {
        if constexpr (PointSize == 2) {
            return Point(p1.x + p2.x, p1.y + p2.y);
        }
        else if constexpr (PointSize == 3) {
            return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
        }
        else if constexpr (PointSize == 4) {
            return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z, p1.w + p2.w);
        }
    }

    friend constexpr auto operator-(const Point& p1, const Point& p2) -> Point {
        if constexpr (PointSize == 2) {
            return Point(p1.x - p2.x, p1.y - p2.y);
        }
        else if constexpr (PointSize == 3) {
            return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
        }
        else if constexpr (PointSize == 4) {
            return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z, p1.w - p2.w);
        }
    }

    friend constexpr auto operator*(const Point& p, value_t c) -> Point {
        if constexpr (PointSize == 2) {
            return Point(p.x * c, p.y * c);
        }
        else if constexpr (PointSize == 3) {
            return Point(p.x * c, p.y * c, p.z * c);
        }
        else if constexpr (PointSize == 4) {
            return Point(p.x * c, p.y * c, p.z * c, p.w * c);
        }
    }

    friend constexpr auto operator*(value_t c, const Point& p) -> Point {
        return p * c;
    }

    friend constexpr auto operator+(const Point& p) -> Point {
        return p;
    }

    friend constexpr auto operator-(const Point& p) -> Point {
        if constexpr (PointSize == 2) {
            return Point(-p.x, -p.y);
        }
        else if constexpr (PointSize == 3) {
            return Point(-p.x, -p.y, -p.z);
        }
        else if constexpr (PointSize == 4) {
            return Point(-p.x, -p.y, -p.z, -p.w);
        }
    }

    friend constexpr auto operator/(const Point& p, value_t divisor) -> Point {
        if constexpr (PointSize == 2) {
            return Point(p.x / divisor, p.y / divisor);
        }
        else if constexpr (PointSize == 3) {
            return Point(p.x / divisor, p.y / divisor, p.z / divisor);
        }
        else if constexpr (PointSize == 4) {
            return Point(p.x / divisor, p.y / divisor, p.z / divisor, p.w / divisor);
        }
    }

    constexpr auto abs() const {
        if constexpr (PointSize == 2) {
            return Point(std::abs(this->x), std::abs(this->y));
        }
        else if constexpr (PointSize == 3) {
            return Point(std::abs(this->x), std::abs(this->y), std::abs(this->z));
        }
        else if constexpr (PointSize == 4) {
            return Point(std::abs(this->x), std::abs(this->y), std::abs(this->z), std::abs(this->w));
        }
    }

    constexpr auto cross(const Point& vec) const
        requires(PointSize == 3)
    {
        return Point(this->y * vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x * vec.y - this->y * vec.x);
    }

    constexpr auto distance_to(const Point& vec) const -> value_t
        requires(PointSize == 2 || PointSize == 3)
    {
        if constexpr (PointSize == 2) {
            return std::sqrt(std::pow(this->x - vec.x, 2) + std::pow(this->y - vec.y, 2));
        }
        else if constexpr (PointSize == 3) {
            return std::sqrt(std::pow(this->x - vec.x, 2) + std::pow(this->y - vec.y, 2) + std::pow(this->z - vec.z, 2));
        }
    }

    constexpr auto dot(const Point& vec) const -> value_t
        requires(PointSize == 2 || PointSize == 3)
    {
        if constexpr (PointSize == 2) {
            return this->x * vec.x + this->y * vec.y;
        }
        else if constexpr (PointSize == 3) {
            return this->x * vec.x + this->y * vec.y + this->z * vec.z;
        }
    }

    constexpr auto length() const -> value_t
        requires(PointSize == 2 || PointSize == 3)
    {
        if constexpr (PointSize == 2) {
            return std::sqrt(this->x * this->x + this->y * this->y);
        }
        else if constexpr (PointSize == 3) {
            return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        }
    }

    constexpr auto linear_interpolate(const Point& vec, value_t t) const
        requires(PointSize == 2 || PointSize == 3)
    {
        if constexpr (PointSize == 2) {
            return Point(this->x + t * (vec.x - this->x), this->y + t * (vec.y - this->y));
        }
        else if constexpr (PointSize == 3) {
            return Point(this->x + t * (vec.x - this->x), this->y + t * (vec.y - this->y), this->z + t * (vec.z - this->z));
        }
    }

    constexpr auto normalized() const
        requires(PointSize == 2 || PointSize == 3)
    {
        value_t len = length();
        if (!equalF(len, 0)) {
            if constexpr (PointSize == 2) {
                return Point(this->x / len, this->y / len);
            }
            else {
                return Point(this->x / len, this->y / len, this->z / len);
            }
        }
        else {
            return *this;
        }
    }

    constexpr auto normalize() {
        *this = normalized();
    }

    constexpr auto rotated(const Point& axis, value_t radians) const
        requires(PointSize == 2)
    {
        value_t cos_angle = std::cos(radians);
        value_t sin_angle = std::sin(radians);
        Point   relative  = *this - axis;
        return Point(
            relative.x * cos_angle - relative.y * sin_angle + axis.x,
            relative.x * sin_angle + relative.y * cos_angle + axis.y
        );
    }

    constexpr auto rotate(const Point& axis, value_t radians) {
        *this = rotated(axis, radians);
    }

    constexpr auto angle_to(const Point& vec) const -> value_t {
        return atan2(cross(vec), dot(vec));
    }

    constexpr auto direction_to(const Point& other) const {
        return (other - *this).normalized();
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point<2>, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point<3>, x, y, z)

using PointInt2 = Point<2, int>;
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PointInt2, x, y)
}   // namespace detail

using Point2    = detail::Point<2>;
using Point3    = detail::Point<3>;
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

inline constexpr auto Point2To3(const std::vector<std::vector<Point2>>& p) {
    std::vector<std::vector<Point3>> res;
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

inline constexpr auto Point3To2(const std::vector<std::vector<Point3>>& p) {
    std::vector<std::vector<Point2>> res;
    res.resize(p.size());
    for (size_t i = 0; i < p.size(); i++) {
        res[i] = Point3To2(p[i]);
    }
    return res;
}
}   // namespace tg

template <>
struct fmt::formatter<tg::Point3> : fmt::formatter<std::string> {                          // NOLINT
    auto format(const tg::Point3& v, format_context& ctx) const -> decltype(ctx.out()) {   // NOLINT
        return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", v.x, v.y, v.z);
    }
};

template <>
struct fmt::formatter<tg::Point2> : fmt::formatter<std::string> {                          // NOLINT
    auto format(const tg::Point2& v, format_context& ctx) const -> decltype(ctx.out()) {   // NOLINT
        return fmt::format_to(ctx.out(), "{{{}, {}}}", v.x, v.y);
    }
};

template <>
struct fmt::formatter<tg::PointInt2> : fmt::formatter<std::string> {                          // NOLINT
    auto format(const tg::PointInt2& v, format_context& ctx) const -> decltype(ctx.out()) {   // NOLINT
        return fmt::format_to(ctx.out(), "{{{}, {}}}", v.x, v.y);
    }
};

template <typename T>                                                   // NOLINT
struct fmt::formatter<std::vector<T>> : fmt::formatter<std::string> {   // NOLINT
    auto format(const std::vector<T>& v, format_context& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "[{}]", fmt::join(v, ", "));
    }
};
