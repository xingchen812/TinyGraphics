@echo off
chcp 65001
if not exist build (mkdir build)
call "%MSVC_BAT%" x64
"%CMAKE_EXEC%" -S . -B build -G Ninja -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE
"%CMAKE_EXEC%" --build build --config Debug --target all
