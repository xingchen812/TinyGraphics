#pragma once
#include <QString>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <numbers>
#include <spdlog/spdlog.h>

namespace tg {
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
	::tg::registerComponent<a_type>(a_name)
#define TG_WINDOW_REGISTER(a_type, ...) \
	DETAIL_TG_WINDOW_REGISTER(a_type, __VA_OPT__(__VA_ARGS__) __VA_OPT__(, ) #a_type)

#define TG_QUICK_WINDOW_REGISTER(a_type, ...) \
	TG_SINGLE_RUN() { TG_WINDOW_REGISTER(a_type, __VA_ARGS__); }

#define TG_QUICK_WINDOW_REGISTER_2 \
	TG_QUICK_WINDOW_REGISTER(Impl, ::tg::detail::filenameWithoutCpp(__FILE__))

namespace detail {
auto filenameWithoutCpp(const QString& file) -> QString;
} // namespace detail

using Json = nlohmann::json;

inline auto resourcePath() -> const std::filesystem::path& {
	static auto path = std::filesystem::current_path().parent_path() / "resource";
	return path;
}

template <typename ValueType>
inline constexpr auto equalF(ValueType a, auto b) {
	return std::abs(a - b) < std::numeric_limits<ValueType>::epsilon();
}

inline constexpr double degreesToRadians(double degrees) {
	return std::numbers::pi * (degrees / 180);
}

inline auto printReadableDuration(std::chrono::nanoseconds duration) {
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
} // namespace tg
