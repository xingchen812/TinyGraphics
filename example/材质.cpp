#include <tg/ui/window.h>

#include <glfw/glfw3.h>
#include <imgui.h>

using namespace tg;

namespace {
class Impl : public ui::Window {   // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
public:
    Impl() {
        textureWidth  = 1000;      // NOLINT
        textureHeight = 1000;      // NOLINT
        // RGBA
        data = std::vector<uint8_t>(4ULL * textureWidth * textureHeight, 0);
        // Set green color (R=0, G=255, B=0, A=255)
        for (int i = 0; i < textureWidth * textureHeight; ++i) {
            data[i * 4 + 0] = 0;     // Red    // NOLINT
            data[i * 4 + 1] = 255;   // Green  // NOLINT
            data[i * 4 + 2] = 0;     // Blue   // NOLINT
            data[i * 4 + 3] = 255;   // Alpha  // NOLINT
        }
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    ~Impl() override {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &textureID);
    }

    auto impl_paint() -> void override {
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

        ImGui::Text("Here is a custom bitmap texture:");
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), ImVec2(static_cast<float>(textureWidth), static_cast<float>(textureHeight)));
    }

    int                  textureWidth;
    int                  textureHeight;
    std::vector<uint8_t> data;
    GLuint               textureID;
};
TG_QUICK_WINDOW_REGISTER_2
}   // namespace
