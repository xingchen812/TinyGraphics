#include <tg/utils.h>

namespace tg {
namespace {
auto localToUtf8(const std::string& localStr) -> std::string {
    auto wideCharSize = MultiByteToWideChar(CP_ACP, 0, localStr.c_str(), -1, nullptr, 0);
    if (wideCharSize <= 0) {
        throw tg_exception();
    }
    std::wstring wideStr(wideCharSize, 0);
    MultiByteToWideChar(CP_ACP, 0, localStr.c_str(), -1, wideStr.data(), wideCharSize);
    auto utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Size <= 0) {
        throw tg_exception();
    }
    std::string utf8Str(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, utf8Str.data(), utf8Size, nullptr, nullptr);
    return utf8Str;
}
}   // namespace

auto getSystemLastErrorAsString() -> std::string {
    auto errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return "(0) No error";
    }
    LPSTR messageBuffer = nullptr;
    auto  size          = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr
    );
    auto message = localToUtf8(std::string(messageBuffer, size));
    LocalFree(messageBuffer);
    return std::format("({}) {}", errorMessageID, message);
}
}   // namespace tg
