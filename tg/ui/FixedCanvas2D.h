#pragma once
#include <tg/Color.h>
#include <tg/Point.h>
#include <tg/ui/window.h>

namespace tg::ui {
class FixedCanvas2D : public Window {
public:
    class Texture {
    public:
        Texture();
        ~Texture();

        Texture(const Texture&)        = delete;
        Texture(Texture&&)             = delete;
        auto operator=(const Texture&) = delete;
        auto operator=(Texture&&)      = delete;

        auto update(const std::vector<uint8_t>& data, int width, int height) -> void;

        Point2 m_texturePos;

    private:
        unsigned int m_textureID;
    };

    static constexpr auto k_default_width = 100;

    explicit FixedCanvas2D(int height = k_default_width, int width = k_default_width)
        : m_width(width), m_height(height), m_data(4ULL * m_width * m_height, 0) {}

    auto impl_paint() -> void override {
        m_texture.update(m_data, m_width, m_height);
    }

    auto resize(int width, int height) {
        m_width  = width;
        m_height = height;
        m_data.resize(4ULL * m_width * m_height, 0);
    }

    auto drawBackground(const Color& color = constants::white) {
        for (auto i : std::views::iota(0, m_width * m_height)) {
            m_data[i * 4 + 0] = color.get_r8();
            m_data[i * 4 + 1] = color.get_g8();
            m_data[i * 4 + 2] = color.get_b8();
            m_data[i * 4 + 3] = color.get_a8();
        }
    }

    auto drawPoint(const PointInt2& p, const Color& color) {
        if (p.x >= m_width || p.y >= m_height || p.x < 0 || p.y < 0) {
            throw tg_exception();
        }
        m_data[(p.x + p.y * m_width) * 4 + 0] = color.get_r8();
        m_data[(p.x + p.y * m_width) * 4 + 1] = color.get_g8();
        m_data[(p.x + p.y * m_width) * 4 + 2] = color.get_b8();
        m_data[(p.x + p.y * m_width) * 4 + 3] = color.get_a8();
    }

    auto drawLine(const Point2& begin, const Point2& end, const Color& color) {
        auto dx    = end.x - begin.x;
        auto dy    = end.y - begin.y;
        auto steps = std::max(std::abs(dx), std::abs(dy));
        auto x_inc = dx / steps;
        auto y_inc = dy / steps;
        auto x     = begin.x;
        auto y     = begin.y;
        drawPoint(PointInt2{static_cast<int>(std::round(x)), static_cast<int>(std::round(y))}, color);
        for (int i = 0; static_cast<float>(i) <= steps + 1; i++) {
            x += x_inc;
            y += y_inc;
            drawPoint({static_cast<int>(std::round(x)), static_cast<int>(std::round(y))}, color);
        }
    }

    auto drawPolygon(const std::vector<Point2>& points, const Color& color, float radians = 0, bool connect_first_last = true) {
        if (points.empty()) {
            return;
        }

        if (equalF(radians, 0)) {
            for (auto i : std::views::iota(0ULL, points.size() - 1)) {
                drawLine(points[i], points[i + 1], color);
            }
            if (connect_first_last) {
                drawLine(points.back(), points.front(), color);
            }
            return;
        }

        const auto& pivot = points[0];
        auto        pre   = pivot;
        for (auto i : std::views::iota(1ULL, points.size())) {
            auto next = points[i].rotated(pivot, radians);
            drawLine(pre, next, color);
            pre = next;
        }
        if (connect_first_last) {
            drawLine(pre, pivot, color);
        }
    }

    auto getTexturePos() const {
        return m_texture.m_texturePos;
    }

    auto getClickedTexturePos() -> std::tuple<bool, Point2> {
        if (auto [ok, p] = getClickedPoint(); ok) {
            p -= getTexturePos();
            if (p.x < 0 || p.y < 0 || p.x > static_cast<float>(m_width) || p.y > static_cast<float>(m_height)) {
                return {false, {}};
            }
            return {true, p};
        }
        return {false, {}};
    }

private:
    int                  m_width;
    int                  m_height;
    std::vector<uint8_t> m_data;
    Texture              m_texture;
};
}   // namespace tg::ui
