#pragma once
#include <tg/utils.h>

namespace tg {
namespace {
} // namespace

namespace detail {
auto filenameWithoutCpp(const QString& file) -> QString {
	auto w = file.toStdWString();
	auto p = std::filesystem::path(w);
	auto wr = p.stem().wstring();
	auto q = QString::fromStdWString(wr);
	return q;
}
} // namespace detail
} // namespace tg
