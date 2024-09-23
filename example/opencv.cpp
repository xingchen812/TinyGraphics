#include <tg/ui/window.h>

#include <imgui.h>
#include <opencv2/opencv.hpp>

using namespace tg;

namespace {
class Impl : public ui::Window {
public:
    auto init() -> void override {
        spdlog::info("示例初始化成功: {}", CV_VERSION);

        cv::Mat image = cv::Mat::zeros(100, 100, CV_8UC3);
        cv::rectangle(image, cv::Point(10, 10), cv::Point(90, 90), cv::Scalar(255, 0, 0), 2);
        cv::imwrite("output_image.jpg", image);
    }
};
TG_QUICK_WINDOW_REGISTER_2
}   // namespace
