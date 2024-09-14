#pragma once
#include <tg/utils.h>

#include <fstream>
#include <functional>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <unordered_map>

namespace tg::ui {
class Component;
class Window;

class Event {
public:
	Event(Window* w, std::string_view event_name, std::any data = {})
		: m_window(w), m_event_name(event_name), m_data(std::move(data)) {}

	Window* m_window;
	std::string m_event_name;
	std::any m_data;
};

class Window {
public:
	friend class MainWindow;

	Window() = default;
	virtual ~Window() = default;

	Window(const Window&) = delete;
	Window(Window&&) = delete;
	auto operator=(const Window&) = delete;
	auto operator=(Window&&) = delete;

	virtual auto init() -> void {}
	virtual auto paint() -> void;

	auto callEvent(const Event& e) const {
		auto it = m_event.find(e.m_event_name);
		if (it == m_event.end() || it->second == nullptr) {
			throw tg_exception();
		}
		it->second(e);
	}

	auto callEvent(const std::string& event_name) {
		callEvent(Event(this, event_name));
	}

	auto registerEvent(const std::string& event_name, std::function<void(const Event&)> callback) {
		m_event.try_emplace(event_name, nullptr).first->second = std::move(callback);
	}

	auto registerEvent(const std::string& event_name, std::function<void()> callback) {
		m_event.try_emplace(event_name, nullptr).first->second = [callback = std::move(callback)](const Event&) {
			callback();
		};
	}

	template <typename ValueType>
	auto readConfig(const std::string& name, ValueType& v) {
		auto j = readConfig();
		if (!j.contains(name)) {
			return;
		}
		from_json(j.at(name), v);
	}

	auto writeConfig(std::string_view name, const Json& j) {
		auto json = readConfig();
		json[name] = j;
		writeConfig(json);
	}

protected:
	virtual auto impl_paint() -> void {}

	auto getConfigFilePath() -> std::filesystem::path {
		return std::filesystem::current_path() / "config" / std::format("{}.json", m_component_name);
	}

	auto readConfig() -> Json {
		if (!std::filesystem::exists(getConfigFilePath())) {
			return Json::object();
		}
		std::ifstream file(getConfigFilePath());
		if (!file.is_open()) {
			throw tg_exception();
		}
		Json j;
		file >> j;
		return j;
	}

	auto writeConfig(const Json& j) -> void {
		std::ofstream file(getConfigFilePath());
		if (!file.is_open()) {
			throw tg_exception();
		}
		file << j.dump(4);
	}

	std::string m_name;

private:
	std::string m_component_name;
	bool m_open;
	std::unordered_map<std::string, std::function<void(const Event&)>> m_event;
};

class Component {
public:
	Component(std::string_view name, std::function<std::unique_ptr<Window>()> create)
		: m_name(name), m_create(std::move(create)) {}

	~Component() = default;

	Component(const Component&) = delete;
	Component(Component&&) = delete;
	auto operator=(const Component&) = delete;
	auto operator=(Component&&) = delete;

	std::string m_name;
	std::function<std::unique_ptr<Window>()> m_create;
};

class MainWindow final : public Window {
public:
	static MainWindow& getInstance() {
		static MainWindow instance;
		return instance;
	}

	auto main(int argc, char** argv) -> int;
	auto paint() -> void override;

	auto registerComponent(std::string_view name, std::function<std::unique_ptr<Window>()> create) {
		auto [_, ok] = m_components.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, std::move(create)));
		if (!ok) {
			throw tg_exception("registerComponent exists: {}", name);
		}
	}

	auto getComponent(std::string_view name) const -> const Component& {
		auto it = m_components.find(name);
		if (it == m_components.end()) {
			throw tg_exception("getComponent not exists: {}", name);
		}
		return it->second;
	}

	auto createWindow(std::string_view component_name) {
		size_t n = 2;
		std::string window_name(component_name);
		while (true) {
			auto [it, ok] = m_windows.emplace(window_name, nullptr);
			if (ok) {
				it->second = makeWindow(component_name, window_name);
				saveWindowsConfig();
				return;
			}
			window_name = std::format("{}-{}", component_name, n++);
		}
	}

	auto closeWindow(std::string_view window_name) {
		m_windows.erase(window_name);
		saveWindowsConfig();
	}

	auto getWindow(std::string_view window_name) const -> const std::unique_ptr<Window>& {
		auto it = m_windows.find(window_name);
		if (it == m_windows.end()) {
			throw tg_exception();
		}
		return it->second;
	}

	auto callEvent(std::string_view window_name, std::string_view event_name, std::any data = {}) {
		auto& w = const_cast<std::unique_ptr<Window>&>(getWindow(window_name));
		w->callEvent(Event(w.get(), event_name, std::move(data)));
	}

	auto registerFunction(std::string_view name, std::function<std::any(const std::any&)> callback) {
		m_functions.try_emplace(std::string{name}, std::function<std::any(const std::any&)>{}).first->second = std::move(callback);
	}

	auto callFunction(std::string_view name, const std::any& any) {
		auto it = m_functions.find(name);
		if (it == m_functions.end()) {
			throw tg_exception("callFunction not found: {}", name);
		}
		try {
			return it->second(any);
		} catch (std::exception& e) {
			throw tg_exception("callFunction exception: {}", e.what());
		}
	}

private:
	MainWindow();

	~MainWindow();

	auto makeWindow(std::string_view component_name, std::string_view window_name) -> std::unique_ptr<Window> {
		auto& c = getComponent(component_name);
		auto w = c.m_create();
		w->m_component_name = component_name;
		w->m_name = window_name;
		w->m_open = true;
		w->init();
		return w;
	}

	auto saveWindowsConfig() -> void {
		std::vector<Json> ws;
		for (auto& i : m_windows) {
			ws.push_back(Json::object({
				{"component name", i.second->m_component_name},
				{"window name", i.second->m_name},
			}));
		}
		writeConfig("启动的窗口", ws);
	}

	auto loadWindowsConfig() -> void {
		if (!std::filesystem::exists(getConfigFilePath().parent_path())) {
			std::filesystem::create_directory(getConfigFilePath().parent_path());
		}
		std::vector<Json> ws;
		readConfig("启动的窗口", ws);
		for (auto& i : ws) {
			auto& component_name = i["component name"].get_ref<const std::string&>();
			auto& window_name = i["window name"].get_ref<const std::string&>();
			if (!m_windows.emplace(window_name, makeWindow(component_name, window_name)).second) {
				throw tg_exception();
			}
		}
		writeConfig("启动的窗口", ws);
	}

	unordered_map_string<Component> m_components;
	unordered_map_string<std::unique_ptr<Window>> m_windows;
	unordered_map_string<std::function<std::any(const std::any&)>> m_functions;
};

inline auto registerComponent(std::string_view name, std::function<std::unique_ptr<Window>()> create) {
	MainWindow::getInstance().registerComponent(name, std::move(create));
}

template <typename WindowType>
inline auto registerComponent(std::string_view name) {
	MainWindow::getInstance().registerComponent(name, []() { return std::make_unique<WindowType>(); });
}

template <bool StatisticTime = true>
inline auto callFunction(std::string_view name, const std::any& any) {
	if constexpr (!StatisticTime) {
		return MainWindow::getInstance().callFunction(name, any);
	}
	auto start = std::chrono::high_resolution_clock::now();
	auto res = MainWindow::getInstance().callFunction(name, any);
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	spdlog::info("funtion used time: {}, {}", name, formatReadableDuration(diff));
	return res;
}
} // namespace tg::ui
