#pragma once
#include <tg/Point.h>
#include <tg/utils.h>

#include <QApplication>
#include <QCursor>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QProcess>
#include <QPushButton>
#include <QScreen >
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <any>
#include <spdlog/sinks/stdout_color_sinks.h>

template <>
struct fmt::formatter<tg::Point3> : fmt::formatter<std::string> {
	auto format(const tg::Point3& v, format_context& ctx) const -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", v.x, v.y, v.z);
	}
};

template <>
struct fmt::formatter<tg::Point2> : fmt::formatter<std::string> {
	auto format(const tg::Point2& v, format_context& ctx) const -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{{{}, {}}}", v.x, v.y);
	}
};

template <typename T>
struct fmt::formatter<std::vector<T>> : fmt::formatter<std::string> {
	auto format(const std::vector<T>& v, format_context& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "[{}]", fmt::join(v, ", "));
	}
};

template <>
struct fmt::formatter<QString> : fmt::formatter<std::string> {
	auto format(const QString& v, format_context& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", v.toStdString());
	}
};

namespace tg {
class Event {
public:
	Event() = default;
	virtual ~Event() = default;

	Event(const Event&) = delete;
	Event(Event&&) = delete;
	auto operator=(const Event&) = delete;
	auto operator=(Event&&) = delete;

	QString m_window_name;
	QString m_event_name;
};

namespace detail {
namespace windowConfig {
inline auto getConfigFilePath(const QString& window_name) {
	QDir dir(QDir::currentPath());
	dir.cdUp();
	dir.cd("config");
	return dir.filePath(window_name + ".json");
}

inline auto readConfig(const QString& window_name) -> Json {
	auto filepath = getConfigFilePath(window_name);
	if (!QFileInfo(filepath).exists()) {
		throw tg_exception("Window::readConfig file not exists: {}", filepath);
	}
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw tg_exception("Window::writeConfig failed to open file: {}", filepath);
	}
	QTextStream in(&file);
	QString fileContent = in.readAll();
	return nlohmann::json::parse(fileContent.toStdString());
}

inline auto writeConfig(const QString& window_name, const Json& j) {
	auto filepath = getConfigFilePath(window_name);
	QFile file(filepath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		throw tg_exception("Window::writeConfig failed to open file: {}", filepath);
	}
	QTextStream out(&file);
	out << QString::fromStdString(j.dump(4));
}

inline auto readConfig(const QString& window_name, const QString& name) -> Json {
	try {
		auto json = readConfig(window_name);
		return json.at(name.toStdString());
	} catch (std::exception& e) {
		throw tg_exception("Window::readConfig config error: {}", e.what());
	}
}

template <typename ValueType>
inline auto readConfig(const QString& window_name, const QString& name, ValueType& v) {
	try {
		auto json = readConfig(window_name);
		json = json.at(name.toStdString());
		from_json(json, v);
	} catch (std::exception& e) {
		throw tg_exception("Window::readConfig config error: {}", e.what());
	}
}

inline auto writeConfig(const QString& window_name, const QString& name, const Json& j) {
	Json json;
	if (QFileInfo(getConfigFilePath(window_name)).exists()) {
		json = readConfig(window_name);
	} else {
		json = Json::object();
	}
	json[name.toStdString()] = j;
	writeConfig(window_name, json);
}
} // namespace windowConfig
} // namespace detail

class Window : public QWidget {
public:
	Window() = default;
	virtual ~Window() = default;

	Window(const Window&) = delete;
	Window(Window&&) = delete;
	auto operator=(const Window&) = delete;
	auto operator=(Window&&) = delete;

	virtual auto init() -> void {}

	auto registerEvent(const QString& event_name, std::function<void()> callback);

	auto moveCurWindow(int x, int y) {
		QPoint cursorPos = QCursor::pos();
		QList<QScreen*> screens = QGuiApplication::screens();
		QScreen* screen = nullptr;
		for (QScreen* s : screens) {
			if (s->geometry().contains(cursorPos)) {
				screen = s;
				break;
			}
		}
		if (screen) {
			QRect screenGeometry = screen->geometry();
			x += screenGeometry.x();
			y += screenGeometry.y();
			this->move(x, y);
		} else {
			throw tg_exception();
		}
	}

	auto messageBoxInfo(const QString& msg) {
		QMessageBox* msgBox = new QMessageBox(QMessageBox::Information, "信息", msg, QMessageBox::Ok, this);
		msgBox->setAttribute(Qt::WA_DeleteOnClose);
		msgBox->setModal(false);
		msgBox->show();
	}

	auto sendThisEvent(const QString& event_name) -> void;

	template <typename ValueType>
	auto readConfig(const QString& name, ValueType& v) {
		detail::windowConfig::readConfig<ValueType>(m_window_name, name, v);
	}

	auto writeConfig(const QString& name, const Json& j) {
		detail::windowConfig::writeConfig(m_window_name, name, j);
	}

	QString m_window_name;
};

class Component {
public:
	Component(const QString& name, std::function<std::unique_ptr<Window>()> create, QTreeWidget* parentList)
		: m_name(name), m_create(create), m_window(nullptr), m_listItem(new QTreeWidgetItem(parentList)) {
		m_listItem->setText(0, m_name);
		initRegister();
	}

	~Component() = default;

	Component(const Component&) = delete;
	Component(Component&&) = delete;
	auto operator=(const Component&) = delete;
	auto operator=(Component&&) = delete;

	auto processEvent(Event* e) {
		auto it = m_event.find(e->m_event_name);
		if (it == m_event.end() || it->second == nullptr) {
			throw tg_exception();
		}
		it->second(e);
	}

	template <typename EventType>
	auto registerEvent(const QString& name, std::function<void(EventType*)> callback) -> void {
		auto [it, ok] = m_event.try_emplace(name, nullptr);
		it->second = std::move(*reinterpret_cast<std::function<void(Event*)>*>(&callback));
		if (ok) {
			QTreeWidgetItem* childItem1 = new QTreeWidgetItem(m_listItem);
			childItem1->setText(0, it->first);
		}
	}

	auto window() {
		return m_window.get();
	}

	auto windowName() {
		return m_name;
	}

private:
	QString m_name;
	std::function<std::unique_ptr<Window>()> m_create;
	std::unique_ptr<Window> m_window;
	std::unordered_map<QString, std::function<void(Event*)>> m_event;
	QTreeWidgetItem* m_listItem;

	auto initRegister() -> void {
		registerEvent<Event>("启动", [this](Event*) {
			if (m_window) {
				m_window->showNormal();
				return;
			}
			m_window = m_create();
			m_window->setWindowTitle(m_name);
			m_window->moveCurWindow(400, 200);
			m_window->m_window_name = m_name;
			m_window->setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
			m_window->init();
			m_window->show();
			detail::windowConfig::writeConfig("TinyGraphics", "最后启动的窗口", m_name.toStdString());
		});

		registerEvent<Event>("停止", [this](Event*) {
			m_window = nullptr;
			m_listItem->takeChildren();
			auto it = m_event.begin();
			while (it != m_event.end()) {
				if (it->first != "启动" && it->first != "停止") {
					it = m_event.erase(it);
				} else {
					it++;
				}
			}
			{
				QTreeWidgetItem* childItem1 = new QTreeWidgetItem(m_listItem);
				childItem1->setText(0, "启动");
			}
			{
				QTreeWidgetItem* childItem1 = new QTreeWidgetItem(m_listItem);
				childItem1->setText(0, "停止");
			}
		});
	}
};

class Manager {
public:
	Manager(const Manager&) = delete;
	Manager(Manager&&) = delete;
	auto operator=(const Manager&) = delete;
	auto operator=(Manager&&) = delete;

	static Manager& getInstance() {
		static Manager instance;
		return instance;
	}

	template <typename WindowType>
	auto registerComponent(const QString& name) {
		auto [_, ok] = m_components.try_emplace(name, std::make_unique<Component>(name, []() { return std::make_unique<WindowType>(); }, m_treeWidget));
		if (!ok) {
			throw tg_exception("register error: {}", name);
		}
	}

	auto getComponent(const QString& window_name) const -> Component* {
		auto it = m_components.find(window_name);
		if (it == m_components.end()) {
			throw tg_exception();
		}
		return it->second.get();
	}

	template <typename EventType>
	auto registerEvent(const QString& window_name, const QString& event_name, std::function<void(EventType*)> callback) {
		getComponent(window_name)->registerEvent(event_name, std::move(callback));
	}

	template <typename EventType, typename... Args>
	auto processEvent(const QString& window_name, const QString& event_name, Args... args) {
		try {
			auto e = std::make_unique<EventType>(std::forward<Args>(args)...);
			e->m_window_name = window_name;
			e->m_event_name = event_name;
			getComponent(window_name)->processEvent(e.get());
		} catch (std::exception& e) {
			spdlog::error("processEvent exception: {}", e.what());
		}
	}

	auto registerFunction(const QString& name, std::function<std::any(const std::any&)> callback) {
		m_functions.try_emplace(name, std::function<std::any(const std::any&)>{}).first->second = std::move(callback);
	}

	auto callFunction(const QString& name, const std::any& any) {
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

	auto main() {
		mainWindowInit();
		m_main_window->show();
		auto ret = m_app->exec();
		spdlog::shutdown();
		return ret;
	}

private:
	class CustomTreeWidget : public QTreeWidget {
	public:
		CustomTreeWidget(QWidget* parent = nullptr) : QTreeWidget(parent) {
			setColumnCount(1);
			setHeaderHidden(true);
		}

	protected:
		auto mousePressEvent(QMouseEvent* event) -> void override {
			QTreeWidgetItem* item = itemAt(event->pos());
			if (item) {
				if (item->parent() == nullptr) {
					item->setExpanded(!item->isExpanded());
				} else {
					auto window_name = item->parent()->text(0);
					auto event_name = item->text(0);
					getInstance().processEvent<Event>(window_name, event_name);
				}
			}
			QTreeWidget::mousePressEvent(event);
		}
	};

	std::unordered_map<QString, std::unique_ptr<Component>> m_components;
	std::unique_ptr<QApplication> m_app;
	std::unique_ptr<QMainWindow> m_main_window;
	QTreeWidget* m_treeWidget;
	std::unordered_map<QString, std::function<std::any(const std::any&)>> m_functions;

	Manager() {
		{
			auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			console_sink->set_level(spdlog::level::debug);
			auto logger = std::make_shared<spdlog::logger>("console", console_sink);
			spdlog::set_default_logger(logger);
			// [时间戳] [日志级别] [线程ID] 消息
			console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v");

			QProcess process;
			process.start("cmd.exe", QStringList() << "/c" << "chcp 65001");
			process.waitForFinished();
		}

		int argc = 0;
		char** argv = nullptr;
		m_app = std::make_unique<QApplication>(argc, argv);
		setStyleSheet(*m_app);
		m_main_window = std::make_unique<QMainWindow>();
		m_treeWidget = new CustomTreeWidget(m_main_window.get());

		m_main_window->setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));
	}

	~Manager() = default;

	auto setStyleSheet(QApplication& app) -> void {
		for (const auto& entry : std::filesystem::directory_iterator(tg::resourcePath() / "ui")) {
			if (!entry.is_regular_file() || entry.path().extension() != ".css") {
				continue;
			}
			QFile file(entry.path());
			if (!file.open(QFile::ReadOnly | QFile::Text)) {
				throw tg_exception();
			}
			app.setStyleSheet(QTextStream(&file).readAll());
		}
	}

	auto expandItemByLabel(const QString& label) {
		for (auto* i : m_treeWidget->findItems(label, Qt::MatchExactly | Qt::MatchRecursive)) {
			i->setExpanded(true);
		}
	}

	auto mainWindowInit() -> void {
		m_main_window->setWindowTitle("TinyGraphics");
		m_main_window->resize(280, 600);
		reinterpret_cast<Window*>(m_main_window.get())->moveCurWindow(45, 200);
		m_main_window->setCentralWidget(m_treeWidget);

		std::string name;
		detail::windowConfig::readConfig("TinyGraphics", "最后启动的窗口", name);
		auto window_name = QString::fromStdString(name);
		expandItemByLabel(window_name);
		processEvent<Event>(window_name, "启动");
	}
};

template <typename WindowType>
inline auto registerComponent(const QString& name) {
	Manager::getInstance().registerComponent<WindowType>(name);
}

inline auto getComponent(const QString& window_name) {
	return Manager::getInstance().getComponent(window_name);
}

template <typename EventType>
inline auto registerEvent(const QString& window_name, const QString& event_name, std::function<void(EventType*)> callback) {
	Manager::getInstance().registerEvent(window_name, event_name, std::move(callback));
}

template <typename EventType, typename... Args>
inline auto sendEvent(const QString& window_name, const QString& event_name, Args... args) {
	Manager::getInstance().processEvent<EventType>(window_name, event_name, std::forward<Args>(args)...);
}

inline auto Window::registerEvent(const QString& event_name, std::function<void()> callback) {
	::tg::registerEvent<Event>(m_window_name, event_name, [callback = std::move(callback)](Event*) {
		callback();
	});
}

inline auto Window::sendThisEvent(const QString& event_name) -> void {
	sendEvent<Event>(m_window_name, event_name);
}

inline auto registerFunction(const QString& name, std::function<std::any(const std::any&)> callback) {
	Manager::getInstance().registerFunction(name, std::move(callback));
}

template <bool StatisticTime = true>
inline auto callFunction(const QString& name, const std::any& any) {
	if constexpr (!StatisticTime) {
		return Manager::getInstance().callFunction(name, any);
	}

	auto start = std::chrono::high_resolution_clock::now();
	auto res = Manager::getInstance().callFunction(name, any);
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	spdlog::info("funtion used time: {}, {}", name, printReadableDuration(diff));
	return res;
}

inline const std::vector<QColor> k_colors = []() {
	std::vector<QColor> colors;
	colors.emplace_back(0.F, 0.F, 255.F);	// Red (BGR format)
	colors.emplace_back(0.F, 255.F, 0.F);	// Green
	colors.emplace_back(255.F, 0.F, 0.F);	// Blue
	colors.emplace_back(255.F, 255.F, 0.F); // Cyan
	colors.emplace_back(255.F, 0.F, 255.F); // Magenta
	colors.emplace_back(0.F, 255.F, 255.F); // Yellow
	// colors.emplace_back(255.F, 255.F, 255.F); // White
	colors.emplace_back(0.F, 0.F, 0.F);		  // Black
	colors.emplace_back(128.F, 128.F, 128.F); // Gray
	colors.emplace_back(0.F, 165.F, 255.F);	  // Orange
	colors.emplace_back(128.F, 0.F, 128.F);	  // Purple
	return colors;
}();
} // namespace tg
