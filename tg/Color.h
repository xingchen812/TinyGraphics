#pragma once
#include <tg/utils.h>

#include <format>

namespace tg {
class Color {
public:
    using value_t     = float;

    value_t r         = 0;
    value_t g         = 0;
    value_t b         = 0;

    constexpr Color() = default;

    constexpr Color(value_t r, value_t g, value_t b)
        : r(r), g(g), b(b) {}

    constexpr Color(uint8_t r, uint8_t g, uint8_t b)
        : r(static_cast<value_t>(r) / static_cast<value_t>(std::numeric_limits<uint8_t>::max())),
          g(static_cast<value_t>(g) / static_cast<value_t>(std::numeric_limits<uint8_t>::max())),
          b(static_cast<value_t>(b) / static_cast<value_t>(std::numeric_limits<uint8_t>::max())) {}

    constexpr auto operator+=(const Color& right) {
        r += right.r;
        g += right.g;
        b += right.b;
    }

    constexpr auto operator-=(const Color& right) {
        r -= right.r;
        g -= right.g;
        b -= right.b;
    }

    constexpr auto operator*=(const Color& right) {
        r *= right.r;
        g *= right.g;
        b *= right.b;
    }

    constexpr auto operator*=(const value_t& right) {
        r *= right;
        g *= right;
        b *= right;
    }

    constexpr auto operator/=(const Color& right) {
        r /= right.r;
        g /= right.g;
        b /= right.b;
    }

    constexpr auto operator/=(const value_t& right) {
        r /= right;
        g /= right;
        b /= right;
    }

    friend constexpr auto operator==(const Color& left, const Color& right) {
        return equalF(left.r, right.r) && equalF(left.g, right.g) && equalF(left.b, right.b);
    }

    friend constexpr auto operator!=(const Color& left, const Color& right) {
        return !(left == right);
    }

    friend constexpr auto operator+(const Color& left, const Color& right) {
        return Color(left.r + right.r, left.g + right.g, left.b + right.b);
    }

    friend constexpr auto operator-(const Color& left, const Color& right) {
        return Color(left.r - right.r, left.g - right.g, left.b - right.b);
    }

    friend constexpr auto operator*(const Color& left, const Color& right) {
        return Color(left.r * right.r, left.g * right.g, left.b * right.b);
    }

    friend constexpr auto operator*(const Color& left, const value_t& right) {
        return Color(left.r * right, left.g * right, left.b * right);
    }

    friend constexpr auto operator/(const Color& left, const Color& right) {
        return Color(left.r / right.r, left.g / right.g, left.b / right.b);
    }

    friend constexpr auto operator/(const Color& left, const value_t& right) {
        return Color(left.r / right, left.g / right, left.b / right);
    }

    // 提取8位颜色分量
    constexpr auto get_r8() const {
        return static_cast<uint8_t>(r * std::numeric_limits<uint8_t>::max());
    }

    constexpr auto get_g8() const {
        return static_cast<uint8_t>(g * std::numeric_limits<uint8_t>::max());
    }

    constexpr auto get_b8() const {
        return static_cast<uint8_t>(b * std::numeric_limits<uint8_t>::max());
    }

    constexpr auto set_r8(uint8_t r) {
        this->r = static_cast<value_t>(r) / static_cast<value_t>(std::numeric_limits<uint8_t>::max());
    }

    constexpr auto set_g8(uint8_t g) {
        this->g = static_cast<value_t>(g) / static_cast<value_t>(std::numeric_limits<uint8_t>::max());
    }

    constexpr auto set_b8(uint8_t b) {
        this->b = static_cast<value_t>(b) / static_cast<value_t>(std::numeric_limits<uint8_t>::max());
    }

    auto to_string() const {
        return std::format("Color(r={}, g={}, b={})", r, g, b);
    }

    static constexpr auto from_uint8(uint8_t r, uint8_t g, uint8_t b) {
        return Color(r, g, b);
    }
};

namespace constants {
constexpr auto black   = Color::from_uint8(0, 0, 0);         // 黑色
constexpr auto white   = Color::from_uint8(255, 255, 255);   // 白色
constexpr auto blue    = Color::from_uint8(0, 0, 255);       // 蓝色
constexpr auto green   = Color::from_uint8(0, 255, 0);       // 绿色
constexpr auto red     = Color::from_uint8(255, 0, 0);       // 红色
constexpr auto cyan    = Color::from_uint8(0, 255, 255);     // 青色
constexpr auto magenta = Color::from_uint8(255, 0, 255);     // 洋红色
constexpr auto yellow  = Color::from_uint8(255, 255, 0);     // 黄色
constexpr auto gray    = Color::from_uint8(128, 128, 128);   // 灰色
constexpr auto orange  = Color::from_uint8(255, 162, 0);     // 橙色
constexpr auto purple  = Color::from_uint8(128, 0, 128);     // 紫色

// colors
inline static const std::vector<Color> k_colors = []() {
    return std::vector<Color>{blue, green, red, cyan, magenta, yellow, black, gray, orange, purple};
}();
}   // namespace constants

}   // namespace tg
