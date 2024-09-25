#pragma once
#include <tg/Color.h>
#include <tg/Point.h>
#include <tg/ui/window.h>

#include <cmath>
#include <opencv2/opencv.hpp>

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

        auto update(const cv::Mat& image) -> void;

        Point2 m_texturePos;

    private:
        unsigned int m_textureID;
    };

    static constexpr auto k_default_width = 100;

    explicit FixedCanvas2D(int width = k_default_width, int height = k_default_width) {
        resize(width, height);
    }

    auto impl_paint() -> void override {
        m_texture.update(m_image);
    }

    auto width() const {
        return m_image.cols;
    }

    auto height() const {
        return m_image.rows;
    }

    auto resize(int newWidth, int newHeight) -> void {
        auto oldImage = m_image.clone();
        m_image       = cv::Mat::zeros(newHeight, newWidth, CV_8UC3);
        drawBackground(constants::white);
        if (!oldImage.empty()) {
            cv::Rect roi(0, 0, std::min(m_image.cols, oldImage.cols), std::min(m_image.rows, oldImage.rows));
            oldImage(roi).copyTo(m_image(roi));
        }
    }

    auto pointInCanvas(const PointInt2& p) const {
        return p.x < width() && p.y < height() && p.x >= 0 && p.y >= 0;
    }

    auto pointInCanvas(const Point2& p) const {
        return p.x < static_cast<float>(width()) && p.y < static_cast<float>(height()) && p.x >= 0 && p.y >= 0;
    }

    auto drawBackground(const Color& color = constants::white) -> void {
        m_image.setTo(ColorToCVBGR(color));
    }

    auto drawPoint(const PointInt2& p, const Color& color) {
        if (!pointInCanvas(p)) {
            throw tg_exception();
        }
        m_image.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(color.get_b8(), color.get_g8(), color.get_r8());
    }

    auto drawLine(const Point2& begin, const Point2& end, const Color& color, int thickness = 1) {
        cv::line(m_image, {std::lround(begin.x), std::lround(begin.y)}, {std::lround(end.x), std::lround(end.y)}, ColorToCVBGR(color), thickness, cv::LINE_AA);
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
            if (!pointInCanvas(p)) {
                return {false, {}};
            }
            return {true, p};
        }
        return {false, {}};
    }

    static auto ColorToCVBGR(const Color& color) -> cv::Scalar {
        auto ret = cv::Scalar(color.get_b8(), color.get_g8(), color.get_r8());
        return ret;
    }

private:
    cv::Mat m_image;
    Texture m_texture;
};
}   // namespace tg::ui
