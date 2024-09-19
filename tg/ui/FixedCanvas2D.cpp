#include <tg/ui/FixedCanvas2D.h>

#include <glfw/glfw3.h>
#include <imgui.h>

namespace tg::ui {
FixedCanvas2D::Texture::Texture() {
    glGenTextures(1, &m_textureID);
}

FixedCanvas2D::Texture::~Texture() {
    glDeleteTextures(1, &m_textureID);
}

auto FixedCanvas2D::Texture::update(const std::vector<uint8_t>& data, int width, int height) -> void {
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    ImVec2 imagePos = ImGui::GetCursorScreenPos();
    m_texturePos.x  = imagePos.x;
    m_texturePos.y  = imagePos.y;
    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textureID)), ImVec2(static_cast<float>(width), static_cast<float>(height)));

    glBindTexture(GL_TEXTURE_2D, 0);
}
}   // namespace tg::ui
