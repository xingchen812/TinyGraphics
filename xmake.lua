add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_languages("clatest", "c++latest")
add_cxflags("cl::/Zc:preprocessor")
add_cxflags("cl::/utf-8")
add_cxflags("cl::/wd4244") -- 隐式类型转换可能丢失数据的警告

add_includedirs(".")
add_includedirs("thirdParty")
add_includedirs("thirdParty/GLFW/include")
add_includedirs("thirdParty/imgui")
add_includedirs("thirdParty/opencv/include")

target("TinyGraphics")
    set_kind("binary")
    add_files("tg/**.cpp")
    add_files("example/**.cpp")

	add_links("opengl32")
	add_links("gdi32")
    add_defines("_GLFW_WIN32=1")
    add_files("thirdParty/glfw/src/*.c")

    add_files("thirdParty/imgui/*.cpp")

	add_linkdirs("thirdParty/opencv/x64/vc16/lib")
	add_links("opencv_world4100")
