#include <tg/ui/FixedCanvas2D.h>

#include <imgui.h>

using namespace tg;

namespace {
class Impl : public ui::FixedCanvas2D {
public:
    static auto LinearInterp(int t, int tStart, int tEnd, Color cStart, Color cEnd) {
        return cStart * (static_cast<float>(t - tEnd) / static_cast<float>(tStart - tEnd)) + cEnd * (static_cast<float>(t - tStart) / static_cast<float>(tEnd - tStart));
    }

    auto bresenhamLine(const PointInt2& p1, const Color& c1, const PointInt2& p2, const Color& c2) {
        auto dx           = abs(p2.x - p1.x);
        auto dy           = abs(p2.y - p1.y);
        auto bInterChange = false;
        auto signX        = (p2.x > p1.x) ? 1 : ((p2.x < p1.x) ? -1 : 0);
        auto signY        = (p2.y > p1.y) ? 1 : ((p2.y < p1.y) ? -1 : 0);
        if (dy > dx) {
            std::swap(dx, dy);
            bInterChange = true;
        }
        auto e  = -dx;
        auto p  = p1;
        auto pc = c1;
        for (int i = 0; i <= dx; i++) {
            if (p1.x != p2.x) {
                pc = LinearInterp(p.x, p1.x, p2.x, c1, c2);
            }
            else {
                pc = LinearInterp(p.y, p1.y, p2.y, c1, c2);
            }
            drawPoint(p, pc);
            if (bInterChange) {
                p.y += signY;
            }
            else {
                p.x += signX;
            }
            e += 2 * dy;
            if (e >= 0) {
                if (bInterChange) {
                    p.x += signX;
                }
                else {
                    p.y += signY;
                }
                e -= 2 * dx;
            }
        }
    }

    auto doLine(const Point2& p1, const Point2& p2) {
        bresenhamLine({static_cast<int>(p1.x), static_cast<int>(p1.y)}, constants::red, {static_cast<int>(p2.x), static_cast<int>(p2.y)}, constants::green);
    }

    auto impl_paint() -> void override {
        if (auto [ok, p] = getClickedTexturePos(); ok) {
            if (m_begin_ok) {
                doLine(m_begin, p);
            }
            else {
                m_begin = p;
            }
            m_begin_ok = !m_begin_ok;
        }
        ui::FixedCanvas2D::impl_paint();
    }

    auto init() -> void override {
        FixedCanvas2D::init();
        resize(600, 600);

        registerEvent("清空", [this]() {
            drawBackground();
        });
    }

    Point2 m_begin;
    bool   m_begin_ok = false;
};
TG_QUICK_WINDOW_REGISTER_2
}   // namespace
