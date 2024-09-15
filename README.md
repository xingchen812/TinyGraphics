# TinyGraphics

## develop

- download TinyGraphics
	```
	git clone https://github.com/xingchen812/TinyGraphics
	cd TinyGraphics
	mkdir bin
	mkdir thirdParty
	# download thirdParty

	# 1. build and run
	xmake config -m debug
	xmake
	xmake run

	# 2. debug with vscode
	# init file: .vscode/settings.json .vscode/tasks.json .vscode/launch.json
	# F5 to debug
	```

- download thirdParty
  - glfw
  - imgui
  - nlohmann
  - spdlog

  quick download: https://github.com/xingchen812/TinyGraphics/releases/download/thirdParty/thirdParty.7z

- `.vscode/settings.json`

  ```json
	{
		"C_Cpp.autocomplete": "disabled",
		"C_Cpp.intelliSenseEngine": "Disabled",
		"[cpp]": {
			"editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
		},
		"clangd.path": "C:\\home\\bin\\clang+llvm-18.1.8-x86_64-pc-windows-msvc\\bin\\clangd.exe",
		"clangd.arguments": [
			"--compile-commands-dir=${workspaceFolder}/.vscode",
			"--pch-storage=memory",
			"--header-insertion=never",
			"--background-index",
			"--completion-style=detailed",
			"-j=12",
			"--all-scopes-completion",
			"--function-arg-placeholders=false",
			"--pretty",
			"--cross-file-rename"
		],
		"files.insertFinalNewline": true
	}
  ```

- `.vscode/tasks.json`

  ```json
	{
		"version": "2.0.0",
		"tasks": [
			{
				"label": "build debug",
				"type": "shell",
				"command": "xmake config -m debug; xmake",
				"group": {
					"kind": "build",
					"isDefault": true
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
				"program": "${workspaceRoot}/build/windows/x64/debug/TinyGraphics.exe",
				"cwd": "${workspaceRoot}",
				"console": "internalConsole",
				"preLaunchTask": "build debug",
			}
		]
	}
  ```
