# DSA Visualizer

A C data structures and algorithms visualizer built with Nuklear, GLFW, OpenGL 3, GLEW, CMake, and vcpkg.

The first visualization implements a dynamic integer list with operations such as append, insert, remove, get, find, contains, clear, and seed.

## Technology

- C11
- CMake
- Visual Studio 2022 / MSVC
- Nuklear immediate-mode GUI
- GLFW for window creation and input
- OpenGL 3 for rendering
- GLEW for loading modern OpenGL functions
- vcpkg for installing GLEW

## Project structure

```text
DSA-Visualizer/
├── data_structures/
│   ├── int_list.c
│   └── int_list.h
├── external/
│   ├── nuklear.h
│   └── nuklear_glfw_gl3.h
├── .vscode/
│   ├── c_cpp_properties.json
│   └── settings.json
├── CMakeLists.txt
├── main.c
└── README.md
```

Nuklear is vendored in `external/`. GLFW is downloaded by CMake during configuration, while GLEW is supplied by vcpkg.

## Prerequisites

Install:

1. Visual Studio 2022 with the **Desktop development with C++** workload.
2. CMake 3.20 or newer.
3. Git.
4. vcpkg.

This project currently expects vcpkg at:

```text
C:/vcpkg-2026.03.18
```

If vcpkg is installed somewhere else, replace that path in the commands and VS Code configuration.

## Install GLEW

Run:

```powershell
C:\vcpkg-2026.03.18\vcpkg.exe install glew:x64-windows
```

GLFW does not need to be installed through vcpkg because CMake downloads GLFW 3.4 using `FetchContent`.

## Configure the project

From the repository root:

```powershell
cmake -S . -B build-vcpkg-vs2022 `
  -G "Visual Studio 17 2022" `
  -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg-2026.03.18/scripts/buildsystems/vcpkg.cmake"
```

Use the Visual Studio 2022 generator explicitly. On this machine, allowing CMake to select Visual Studio 2026 automatically caused an MSVC toolset mismatch.

A CMake toolchain must be supplied the first time a build directory is configured. If the toolchain or generator changes, use a new build directory.

## Build

Build the Debug configuration:

```powershell
cmake --build build-vcpkg-vs2022 --config Debug
```

Build the Release configuration:

```powershell
cmake --build build-vcpkg-vs2022 --config Release
```

## Run

Run the Debug executable:

```powershell
.\build-vcpkg-vs2022\Debug\DSA-Visualizer.exe
```

Run the Release executable:

```powershell
.\build-vcpkg-vs2022\Release\DSA-Visualizer.exe
```

## Complete rebuild

To reconfigure without deleting source files, use a new build directory:

```powershell
cmake -S . -B build-fresh `
  -G "Visual Studio 17 2022" `
  -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg-2026.03.18/scripts/buildsystems/vcpkg.cmake"

cmake --build build-fresh --config Debug
```

The generated build directories are ignored by Git.

## How the dependencies fit together

```text
Application
├── IntList
└── Nuklear
    └── Nuklear GLFW/OpenGL 3 backend
        ├── GLFW: window and input
        ├── GLEW: OpenGL function loading
        └── OpenGL: rendering
```

Nuklear creates UI draw commands but does not create a window or render those commands by itself.

GLFW creates the window and OpenGL context. GLEW loads modern OpenGL functions after that context has been created.

## Important initialization order

The relevant headers must be included in this order:

```c
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Nuklear configuration macros */
#include "external/nuklear.h"

#define NK_GLFW_GL3_IMPLEMENTATION
#include "external/nuklear_glfw_gl3.h"
```

At runtime, initialization follows this order:

1. Initialize GLFW.
2. Create the GLFW window.
3. Make its OpenGL context current.
4. Initialize GLEW.
5. Initialize the Nuklear GLFW backend.
6. Initialize the Nuklear font atlas.
7. Enter the input, UI, and rendering loop.

GLEW cannot be initialized before an OpenGL context exists.

## Nuklear implementation macros

Nuklear is a single-header library. These macros must be defined in exactly one C translation unit:

```c
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
```

Defining them in multiple C files will cause duplicate-symbol linker errors.

## Main loop

Every frame follows this pattern:

```text
Poll GLFW events
        ↓
Begin a new Nuklear frame
        ↓
Build the user interface
        ↓
Clear the OpenGL framebuffer
        ↓
Render Nuklear draw commands
        ↓
Swap GLFW buffers
```

Nuklear is an immediate-mode GUI. The interface is reconstructed every frame, so values that must persist—such as search results and selected indexes—must be stored outside the main loop.

## VS Code IntelliSense

CMake can build successfully even if VS Code displays incorrect error squiggles. The compiler and IntelliSense use separate configurations.

This project uses `.vscode/c_cpp_properties.json` to provide IntelliSense with:

- The Visual Studio 2022 compiler
- Nuklear include paths
- GLFW include paths
- The vcpkg GLEW include path
- The Windows SDK version

If IntelliSense becomes stale, run these commands from the VS Code Command Palette:

```text
C/C++: Reset IntelliSense Database
Developer: Reload Window
```

If the MSVC or vcpkg version changes, update the absolute paths in `.vscode/c_cpp_properties.json`.

## Common problems

### `CMAKE_TOOLCHAIN_FILE was not used`

The build directory was already configured without the toolchain.

Configure a new build directory:

```powershell
cmake -S . -B another-build `
  -G "Visual Studio 17 2022" `
  -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg-2026.03.18/scripts/buildsystems/vcpkg.cmake"
```

### Missing `GLchar`, `GL_VERTEX_SHADER`, or `glCreateShader`

GLEW is missing, included after GLFW, or not initialized.

Confirm that:

```c
#include <GL/glew.h>
#include <GLFW/glfw3.h>
```

appear in that order and that `glewInit()` is called after `glfwMakeContextCurrent()`.

### CMake selects Visual Studio 2026

Force the known working generator:

```powershell
-G "Visual Studio 17 2022"
```

A build directory configured with one generator cannot later be changed to another generator.

### Nuklear warnings from MSVC

MSVC may report warnings inside `nuklear.h`, including enum-type, unnamed-type, and `fopen` warnings. These warnings originate in the third-party header and do not necessarily prevent the executable from building.

## Current milestone

The integer-list visualizer supports or is planned to support:

- Append
- Insert
- Remove by index
- Remove head
- Clear
- Seed with values 1 through 5
- Get by index
- Find a value
- Contains a value
- Display count and capacity

The next major milestone is animating operations rather than applying every mutation immediately.