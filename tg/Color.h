#pragma once
#include <tg/utils.h>

#include <algorithm>
#include <format>

namespace tg {
class Color {
public:
    using value_t     = float;

    value_t r         = 0;
    value_t g         = 0;
    value_t b         = 0;
    value_t a         = 0;

    constexpr Color() = default;

    constexpr Color(value_t r, value_t g, value_t b, value_t a)
        : r(r), g(g), b(b), a(a) {}

    constexpr auto operator+=(const Color& right) {
        r += right.r;
        g += right.g;
        b += right.b;
        a += right.a;
    }

    constexpr auto operator-=(const Color& right) {
        r -= right.r;
        g -= right.g;
        b -= right.b;
        a -= right.a;
    }

    constexpr auto operator*=(const Color& right) {
        r *= right.r;
        g *= right.g;
        b *= right.b;
        a *= right.a;
    }

    constexpr auto operator*=(const value_t& right) {
        r *= right;
        g *= right;
        b *= right;
        a *= right;
    }

    constexpr auto operator/=(const Color& right) {
        r /= right.r;
        g /= right.g;
        b /= right.b;
        a /= right.a;
    }

    constexpr auto operator/=(const value_t& right) {
        r /= right;
        g /= right;
        b /= right;
        a /= right;
    }

    friend constexpr auto operator==(const Color& left, const Color& right) {
        return equalF(left.r, right.r) && equalF(left.g, right.g) && equalF(left.b, right.b) && equalF(left.a, right.a);
    }

    friend constexpr auto operator!=(const Color& left, const Color& right) {
        return !(left == right);
    }

    friend constexpr auto operator+(const Color& left, const Color& right) {
        return Color(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a);
    }

    friend constexpr auto operator-(const Color& left, const Color& right) {
        return Color(left.r - right.r, left.g - right.g, left.b - right.b, left.a - right.a);
    }

    friend constexpr auto operator*(const Color& left, const Color& right) {
        return Color(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a);
    }

    friend constexpr auto operator*(const Color& left, const value_t& right) {
        return Color(left.r * right, left.g * right, left.b * right, left.a * right);
    }

    friend constexpr auto operator/(const Color& left, const Color& right) {
        return Color(left.r / right.r, left.g / right.g, left.b / right.b, left.a / right.a);
    }

    friend constexpr auto operator/(const Color& left, const value_t& right) {
        return Color(left.r / right, left.g / right, left.b / right, left.a / right);
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

    constexpr auto get_a8() const {
        return static_cast<uint8_t>(a * std::numeric_limits<uint8_t>::max());
    }

    constexpr auto to_ARGB32() const -> uint32_t {
        return ((uint8_t(a * 255) << 24) | (uint8_t(r * 255) << 16) | (uint8_t(g * 255) << 8) | uint8_t(b * 255));   // NOLINT
    }

    constexpr auto to_ARGB64() const -> uint64_t {
        return (uint64_t(a * 65535) << 48) | (uint64_t(r * 65535) << 32) | (uint64_t(g * 65535) << 16) | uint64_t(b * 65535);   // NOLINT
    }

    constexpr auto to_RGBA32() const -> uint32_t {
        return ((uint8_t(r * 255) << 24) | (uint8_t(g * 255) << 16) | (uint8_t(b * 255) << 8) | uint8_t(a * 255));   // NOLINT
    }

    constexpr auto to_RGBA64() const -> uint64_t {
        return (uint64_t(r * 65535) << 48) | (uint64_t(g * 65535) << 32) | (uint64_t(b * 65535) << 16) | uint64_t(a * 65535);   // NOLINT
    }

                                                                                                                                // 灰度值（亮度）
    constexpr auto gray() const -> value_t {
        return (r + g + b) / 3.F;   // NOLINT
    }

                                    // 颜色加深和变浅
    constexpr auto darkened(value_t amount) const {
        return Color(std::max(0.F, r - amount), std::max(0.F, g - amount), std::max(0.F, b - amount), a);
    }

    constexpr auto lightened(value_t amount) const {
        return Color(std::min(1.F, r + amount), std::min(1.F, g + amount), std::min(1.F, b + amount), a);
    }

    // 颜色反转
    constexpr auto invert() {
        r = 1.F - r;
        g = 1.F - g;
        b = 1.F - b;
    }

    constexpr auto inverted() const {
        return Color(1.F - r, 1.F - g, 1.F - b, a);
    }

    // 对比度调整
    constexpr auto contrast() {
        constexpr value_t k_value    = 0.5F;
        value_t           gray_value = gray();
        if (gray_value < k_value) {
            r *= k_value;
            g *= k_value;
            b *= k_value;
        }
        else {
            r = (1.F - r) * k_value + r;
            g = (1.F - g) * k_value + g;
            b = (1.F - b) * k_value + b;
        }
    }

    auto to_string() const {
        return std::format("Color(r={}, g={}, b={}, a={})", r, g, b, a);
    }
};

namespace constants {
constexpr Color black{0.F, 0.F, 0.F, 1.F};       // 黑色
constexpr Color white{1.F, 1.F, 1.F, 1.F};       // 白色
constexpr Color blue{0.F, 0.F, 1.F, 1.F};        // 蓝色
constexpr Color green{0.F, 1.F, 0.F, 1.F};       // 绿色
constexpr Color red{1.F, 0.F, 0.F, 1.F};         // 红色
constexpr Color cyan{0.F, 1.F, 1.F, 1.F};        // 青色
constexpr Color magenta{1.F, 0.F, 1.F, 1.F};     // 洋红色
constexpr Color yellow{1.F, 1.F, 0.F, 1.F};      // 黄色
constexpr Color gray{0.5F, 0.5F, 0.5F, 1.F};     // 灰色
constexpr Color orange{1.F, 0.647F, 0.F, 1.F};   // 橙色
constexpr Color purple{0.5F, 0.F, 0.5F, 1.F};    // 紫色

inline static const std::vector<Color> k_colors = []() {
    return std::vector<Color>{blue, green, red, cyan, magenta, yellow, black, gray, orange, purple};
}();
}   // namespace constants

}   // namespace tg
