add_rules("mode.debug", "mode.release")
set_languages("clatest", "c++latest")

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

add_includedirs("$(scriptdir)")
add_includedirs("$(scriptdir)/thirdParty")
add_includedirs("$(scriptdir)/thirdParty/GLFW/include")
add_includedirs("$(scriptdir)/thirdParty/imgui")

add_cxflags("cl::/Zc:preprocessor")
add_cxflags("cl::/utf-8")

target("TinyGraphics")
    set_kind("binary")

	add_links("opengl32")
	add_links("gdi32")
    add_defines("_GLFW_WIN32=1")
    add_files("thirdParty/glfw/src/*.c")

    add_files("thirdParty/imgui/*.cpp")

    add_files("tg/*.cpp")
    add_files("tg/ui/*.cpp")
    add_files("example/*.cpp")
