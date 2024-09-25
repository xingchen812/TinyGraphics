#include <tg/ui/FixedCanvas2D.h>

#include <glfw/glfw3.h>
#include <imgui.h>

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_BGR 0x80E0

namespace tg::ui {
FixedCanvas2D::Texture::Texture() {
    glGenTextures(1, &m_textureID);
}

FixedCanvas2D::Texture::~Texture() {
    glDeleteTextures(1, &m_textureID);
}

auto FixedCanvas2D::Texture::update(const cv::Mat& image) -> void {
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);

    ImVec2 imagePos = ImGui::GetCursorScreenPos();
    m_texturePos.x  = imagePos.x;
    m_texturePos.y  = imagePos.y;
    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textureID)), ImVec2(static_cast<float>(image.cols), static_cast<float>(image.rows)));

    glBindTexture(GL_TEXTURE_2D, 0);
}
}   // namespace tg::ui
