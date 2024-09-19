#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>
#include <numbers>
#include <source_location>
#include <spdlog/spdlog.h>

#define TG_EXCEPTION(...) std::runtime_error(fmt::format("runtime_error: {}:{} {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__ __VA_OPT__(, ) "")))
#define tg_exception(...) TG_EXCEPTION(__VA_ARGS__)

#define DETAIL_CAT_1(a, b) a##b
#define DETAIL_CAT(a, b) DETAIL_CAT_1(a, b)
#define DETAIL_TG_SINGLE_RUN(a_reg, a_reg2) \
    namespace {                             \
    struct a_reg {                          \
        a_reg();                            \
    };                                      \
    a_reg a_reg2{};                         \
    }                                       \
    a_reg::a_reg()
#define TG_SINGLE_RUN() \
    DETAIL_TG_SINGLE_RUN(DETAIL_CAT(TG_SINGLE_RUN, __COUNTER__), DETAIL_CAT(TG_SINGLE_RUN, __COUNTER__))

#define DETAIL_TG_WINDOW_REGISTER(a_type, a_name, ...) \
    ::tg::ui::registerComponent<a_type>(a_name)
#define TG_WINDOW_REGISTER(a_type, ...) \
    DETAIL_TG_WINDOW_REGISTER(a_type, __VA_OPT__(__VA_ARGS__) __VA_OPT__(, ) #a_type)

#define TG_QUICK_WINDOW_REGISTER(a_type, ...) \
    TG_SINGLE_RUN() { TG_WINDOW_REGISTER(a_type, __VA_ARGS__); }

#define TG_QUICK_WINDOW_REGISTER_2 \
    TG_QUICK_WINDOW_REGISTER(Impl, ::tg::currentFilenameWithoutCpp())

namespace tg {
namespace detail {
struct string_view_hash {
    using is_transparent = void;

    auto operator()(std::string_view sv) const noexcept -> std::size_t {
        return std::hash<std::string_view>{}(sv);
    }

    auto operator()(const std::string& s) const noexcept -> std::size_t {
        return std::hash<std::string>{}(s);
    }
};

struct string_view_equal {
    using is_transparent = void;

    auto operator()(std::string_view lhs, std::string_view rhs) const noexcept -> bool {
        return lhs == rhs;
    }

    auto operator()(const std::string& lhs, std::string_view rhs) const noexcept -> bool {
        return lhs == rhs;
    }

    auto operator()(std::string_view lhs, const std::string& rhs) const noexcept -> bool {
        return lhs == rhs;
    }

    auto operator()(const std::string& lhs, const std::string& rhs) const noexcept -> bool {
        return lhs == rhs;
    }
};

template <typename ValueType>
using unordered_map_string = std::unordered_map<std::string, ValueType, string_view_hash, string_view_equal>;
}   // namespace detail

using detail::unordered_map_string;

using Json = nlohmann::json;

inline auto resourcePath() -> std::filesystem::path {
    return std::filesystem::current_path().parent_path() / "resource";
}

template <typename ValueType>
inline constexpr auto equalF(ValueType a, auto b) {
    return std::abs(a - b) < std::numeric_limits<ValueType>::epsilon();
}

inline constexpr auto degreesToRadians(double degrees) -> double {
    constexpr auto k_degrees = 180.;
    return std::numbers::pi * (degrees / k_degrees);
}

inline constexpr auto degreesToRadians(float degrees) -> float {
    return static_cast<float>(degreesToRadians(static_cast<double>(degrees)));
}

inline constexpr auto formatReadableDuration(std::chrono::nanoseconds duration) {
    auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= s;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    duration -= ms;
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    if (s.count() > 0) {
        return std::format("{}.{:03} s", s.count(), ms.count());
    }
    return std::format("{}.{:06} ms", ms.count(), us.count());
}

inline auto currentFilenameWithoutCpp(const std::source_location& source_location = std::source_location::current()) {
    constexpr std::string_view k_suffix = ".cpp";
    auto                       name     = std::string_view{source_location.file_name()};
    auto                       pos      = name.find_last_of("/\\");
    if (!name.ends_with(k_suffix)) {
        spdlog::error("currentFilenameWithoutCpp error: {}", source_location.file_name());
        std::abort();
    }
    if (pos == std::string_view::npos) {
        pos = 0;
    }
    else {
        pos++;
    }
    name = name.substr(pos, name.size() - k_suffix.size() - pos);
    return std::string{name};
}

inline auto getNextID() {
    static auto id = 1ULL;
    return id++;
}
}   // namespace tg
