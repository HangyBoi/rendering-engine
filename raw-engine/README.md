# Raw Engine (v2): technical notes

The starting point for the OpenGL/GLSL bootcamps. Version 2 is the **final** version of the
engine (Bootcamp 4 hand-out, `RawEngine_v2_2526_3.zip`): everything from Bootcamp 4 onward is
built on top of this folder. It lives as a subfolder of this repo, with its own `CMakeLists.txt`
pulled in via `add_subdirectory(raw-engine)` from the root one, since it needs a different
toolchain (a vcpkg-installed GLFW / GLAD / GLM / Assimp / stb stack) than the C++-only bootcamps
(1-2) do. The root CMake project's vcpkg toolchain-file setting applies repo-wide but is harmless
to the plain bootcamp-XX exercises, since they never call `find_package`.

## What is here

For the overview and the design ideas, see the [main README](../README.md) and the Wiki. This file only covers the technical details of this folder.

| Path | What it is |
| --- | --- |
| `main.cpp` | Window, OpenGL and Dear ImGui setup, the scene (textured quad + monkey), mouse/keyboard input, and the render loop, split into small functions |
| `core/mesh.h/.cpp` | `Mesh`: vertices + indices and the VAO/VBO/EBO that hold them. Owns its GPU buffers (move-only). `render()`, `generateQuad()` |
| `core/texture.h/.cpp` | `Texture`: loads an image with stb_image into an OpenGL texture. Owns it (move-only). `getId()` |
| `core/shader.h/.cpp` | `Shader`: compiles and links a vertex + fragment file into a program (move-only). `use()`, `isValid()`, `setMat4()`, `setInt()`, with a cache for uniform locations |
| `core/camera.h/.cpp` | `Camera`: position + yaw/pitch, view and projection matrices, `moveForward/Right/Up()`, `rotate()`, `setAspectRatio()` |
| `core/model.h/.cpp` | `Model`: a list of meshes plus its own `modelMatrix`; `translate`, `rotate`, `scale`, `getModelMatrix`, `render` |
| `core/vertex.h` | `Vertex`: position, normal, uv (32 bytes) |
| `core/assimpLoader.h/.cpp` | `AssimpLoader::loadModel(path)`: Assimp scene -> `Model` (Triangulate, FlipUVs, GenNormals) |
| `shaders/` | `modelVertex.vert` (applies `mvpMatrix`), `fragment.frag` (normal as colour), `texture.frag` (samples `diffuseTexture`), `vertex.vert` (the Bootcamp 3 pass-through vertex shader, unused) |
| `models/nonormalmonkey.obj` | Suzanne, exported without normals (Assimp generates them) |
| `textures/CMGaTo_crop.png` | The cat texture on the quad |
| `imgui/` | Dear ImGui 1.92.0 (WIP snapshot) source, bundled with the hand-out - **not** a vcpkg package here |
| `_original/` | The untouched hand-out zips (v1 and v2). Reference only: not part of the build and excluded from git by this folder's `.gitignore` |

## Changes compared with the hand-out

**Code.** `Shader` and `Camera` are new. `Mesh` and `Texture` were reworked (ownership rules), `Model` and `AssimpLoader` got small changes (moves instead of copies), and `main.cpp` was rewritten to use them. `core/vertex.h` is unchanged. The table in the main README lists each file.

**Build and repository setup.**

1. **`CMAKE_SOURCE_DIR` -> `CMAKE_CURRENT_SOURCE_DIR`** in all copy steps. `CMAKE_SOURCE_DIR` always points at the *top-level* project (the repo root) once this folder is pulled in via `add_subdirectory`, so the hand-out's version looks for `<repo>/shaders`, which does not exist. (Same fix v1 needed.)
2. **`copy_shaders` became `copy_assets`** and now copies `shaders/`, `models/` and `textures/` on *every* build. In the hand-out only the shaders were copied every build; models and textures were copied by `PRE_BUILD` steps that only run when the executable is relinked, so editing a `.obj` or `.png` without touching C++ code left the old file in the build folder.
3. **`.gitignore`: `!models/*.obj`.** The repo-root `.gitignore` contains the stock C++ pattern `*.obj` (compiled object files), which would silently keep `nonormalmonkey.obj` out of git - a fresh clone would then fail to find its model. Here `.obj` means 3D model, so the folder re-includes them.
4. **`CMakeSettings.json` not copied.** It is a Visual Studio-specific file (the hand-out ships ARM64/x64 presets); CLion's own CMake profile does the same job. Same decision as for v1.
5. **Shader files renamed to `.vert` / `.frag`** (`modelVertex.vs` -> `modelVertex.vert`, `fragment.fs` -> `fragment.frag`, `texture.fs` -> `texture.frag`, `vertex.vs` -> `vertex.vert`), so the GLSL plugin highlights them. The shader paths in `main.cpp` follow.
6. **`core/texture.cpp` and `core/texture.h`** are listed with the other core sources in `CMakeLists.txt` (the hand-out listed them inside the ImGui source list).

Also left out on purpose: `.idea/`, `cmake-build-*/`, macOS metadata (`__MACOSX/`, `.DS_Store`).

## Dependencies (via vcpkg)

`CMakeLists.txt` expects five vcpkg packages, found through a CMake toolchain-file flag set once
per machine in CLion's CMake profile (Settings | Build, Execution, Deployment | CMake | CMake
options: `-DCMAKE_TOOLCHAIN_FILE=<vcpkg root>/scripts/buildsystems/vcpkg.cmake`):

- `glad` - the OpenGL function loader
- `glfw3` - window and input handling
- `glm` - vector/matrix math
- `assimp` - 3D model loading (the long one: it builds from source, expect 10-20 minutes the first time)
- `stb` - header-only image loading (`stb_image.h`), found with `find_package(Stb)`

One-time install (Windows, classic mode):

```
C:\vcpkg\vcpkg.exe install glad:x64-windows glfw3:x64-windows glm:x64-windows assimp:x64-windows stb:x64-windows
```

Dear ImGui does **not** need a vcpkg package: its source is in `imgui/` and compiled by
`CMakeLists.txt`. (The comment above the ImGui includes in `main.cpp` mentions
`vcpkg install imgui[...]`; that is a leftover from an earlier setup and can be ignored.)

## Build and run

Open the repo root in CLion, set the vcpkg toolchain flag once, **Reload CMake** (Reset Cache
the first time), pick the `RawEngine` run configuration, press Run. The executable is built into
`cmake-build-*/raw-engine/`, and `shaders/`, `models/`, `textures/` are copied right next to it -
that folder must be the working directory (CLion's default for the target).

Expected result: a window titled "Raw Engine", the cat texture on a big quad, the monkey (coloured
by its normals) spinning in front of it, and a small ImGui window with the camera controls.
Hold the right mouse button and use the mouse and W/A/S/D (E/Q: up/down) to fly.
The texture looks upside down: that is a known property of the hand-out.

## Git history note

No separate repository for this folder was ever published, so it was folded into `rendering-engine`
as a subfolder (see the repo root's `CMakeLists.txt`) and syncs across devices the same way the
bootcamp exercises do: clone once, open the repo root in CLion, set the vcpkg toolchain flag once per
machine, build. The v1 -> v2 swap replaced `main.cpp`, `CMakeLists.txt` and the two v1 shaders in place
(history of v1 is in git; `_original/RawEngine_v1_2526_2.zip` still has the untouched hand-out).
