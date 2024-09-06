# TinyGraphics

## develop

- `.vscode/settings.json`

  ```json
  {
  	"files.insertFinalNewline": true
  }
  ```

- `.vscode/tasks.json`

  ```json
  {
  	"version": "2.0.0",
  	"tasks": [
  		{
  			"label": "build",
  			"type": "shell",
  			"command": "${workspaceRoot}/build.bat",
  			"args": [],
  			"group": {
  				"kind": "build",
  				"isDefault": true
  			},
  			"options": {
  				"env": {
  					"CMAKE_PREFIX_PATH": "C:/home/bin/qt/6.7.2/msvc2019_64",
  					"MSVC_BAT": "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Auxiliary/Build/vcvarsall.bat",
  					"CMAKE_EXEC": "C:/home/bin/qt/Tools/CMake_64/bin/cmake.exe"
  				}
  			}
  		}
  	]
  }
  ```

- `.vscode/launch.json`

  ```json
  {
  	"version": "0.2.0",
  	"configurations": [
  		{
  			"name": "Debug",
  			"type": "cppvsdbg",
  			"request": "launch",
  			"program": "${workspaceRoot}/build/TinyGraphics.exe",
  			"stopAtEntry": false,
  			"cwd": "${workspaceRoot}/build/",
  			"environment": [],
  			"console": "integratedTerminal",
  			"preLaunchTask": "build"
  		}
  	]
  }
  ```

- vscode setting

  ```json
  {
  	"C_Cpp.autocomplete": "disabled",
  	"C_Cpp.intelliSenseEngine": "Disabled",
  	"[cpp]": {
  		"editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
  	},
  	"clangd.path": "C:\\home\\bin\\clang+llvm-18.1.8-x86_64-pc-windows-msvc\\bin\\clangd.exe",
  	"clangd.arguments": [
  		"--compile-commands-dir=${workspaceFolder}/build",
  		"--pch-storage=memory",
  		"--header-insertion=never",
  		"--background-index",
  		"--completion-style=detailed",
  		"--clang-tidy",
  		"-j=12",
  		"--all-scopes-completion",
  		"--function-arg-placeholders=false",
  		"--header-insertion-decorators",
  		"--clang-tidy-checks=-unused-includes"
  	]
  }
  ```
