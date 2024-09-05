#pragma once
#include <filesystem>

namespace tg {
#define TG_EXCEPTION() std::runtime_error(std::format("runtime_error: {}:{}", __FILE__, __LINE__))
#define tg_exception() TG_EXCEPTION()

inline auto resourcePath() -> const std::filesystem::path& {
	static auto path = std::filesystem::current_path().parent_path() / "resource";
	return path;
}
} // namespace tg
