# Raw Engine

The starting point for the OpenGL/GLSL bootcamps (3 onward). Lives as a subfolder of this
repo, with its own `CMakeLists.txt` pulled in via `add_subdirectory(raw-engine)` from the
root one, since it needs a different toolchain (a vcpkg-installed GLFW / GLAD / GLM stack)
than the C++-only bootcamps (1-2) do. The root CMake project's vcpkg toolchain-file setting
applies repo-wide but is harmless to the plain bootcamp-XX exercises, since they never call
`find_package`.

## What is here

- `main.cpp`, `CMakeLists.txt` - the engine as unpacked from the bootcamp 3
  hand-out (`RawEngine_v1_2526_2.zip`, kept as-is under `_original/` for
  reference, not part of the build, and excluded from git by this folder's
  own `.gitignore`). One fix from the original hand-out: the shader-copy
  steps in `CMakeLists.txt` now use `CMAKE_CURRENT_SOURCE_DIR` instead of
  `CMAKE_SOURCE_DIR`, since the latter always points at the *top-level*
  source dir (the repo root) once this folder is pulled in via
  `add_subdirectory` rather than opened as its own standalone project.
- `shaders/vertex.vert`, `shaders/fragment.frag` - the two GLSL shaders `main.cpp`
  loads at startup. `CMakeLists.txt` copies this folder next to the built
  executable automatically (`copy_shaders` target), so builds keep working
  after moving the project.

Left out of this copy on purpose: `.idea/` (personal IDE state), the old
`cmake-build-debug/` (compiled binaries), and `CMakeSettings.json` (a Visual
Studio-specific CMake file; CLion's own toolchain/profile settings do the
same job and are not committed).

## Dependencies (via vcpkg)

`CMakeLists.txt` expects three vcpkg packages, found through a CMake toolchain-file flag
set once per machine in CLion's CMake profile for this repo (Settings | Build, Execution,
Deployment | CMake | CMake options: `-DCMAKE_TOOLCHAIN_FILE=<vcpkg root>/scripts/buildsystems/vcpkg.cmake`):

- `glad` - the OpenGL function loader
- `glfw3` - window and input handling
- `glm` - vector/matrix math

## Git history note

No separate repository for this folder was ever published — Yvens's own Raw Engine repo
link never materialized for v1, and Bootcamp 4 is expected to hand out a v2 instead of a
git link. Rather than wait, this folder was folded directly into `rendering-engine` as a
subfolder (see the repo root's `CMakeLists.txt`, which pulls it in with
`add_subdirectory(raw-engine)`), so it syncs across devices the same way the bootcamp
exercises already do — clone once, open the repo root in CLion, set the vcpkg toolchain
flag once per machine, build. If Yvens's v1 repo does surface later, or when the Bootcamp 4
v2 hand-out arrives, diff it against this folder rather than replacing it outright, since
the `CMAKE_CURRENT_SOURCE_DIR` fix and this subfolder wiring would need to be reapplied to
a fresh copy.
