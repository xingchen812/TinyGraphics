#pragma once
#include <QString>
#include <filesystem>

namespace tg {
#define TG_EXCEPTION(...) std::runtime_error(std::format("runtime_error: {}:{} {}", __FILE__, __LINE__, std::format("" __VA_OPT__(, ) __VA_ARGS__)))
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
	::tg::registerComponent<::tg::a_type>(a_name)
#define TG_WINDOW_REGISTER(a_type, ...) \
	DETAIL_TG_WINDOW_REGISTER(a_type, __VA_OPT__(__VA_ARGS__) __VA_OPT__(, ) #a_type)

#define TG_QUICK_WINDOW_REGISTER(a_type, ...) \
	TG_SINGLE_RUN() { TG_WINDOW_REGISTER(a_type, __VA_ARGS__); }

#define TG_QUICK_WINDOW_REGISTER_2 \
	TG_QUICK_WINDOW_REGISTER(Impl, ::tg::detail::filenameWithoutCpp(__FILE__))

namespace detail {
auto filenameWithoutCpp(const QString& file) -> QString;
} // namespace detail

inline auto resourcePath() -> const std::filesystem::path& {
	static auto path = std::filesystem::current_path().parent_path() / "resource";
	return path;
}
} // namespace tg

template <>
struct std::formatter<QString> : std::formatter<std::string> {
	auto format(const QString& v, format_context& ctx) const {
		return formatter<string>::format(
			std::format("{}", v.toStdString()), ctx);
	}
};
