# C Codebase
A codebase for my C projects

>**This is still a Work In Progress**

>**Currently, only Windows 10/11 64bit has been tested**

# Compiling

This Codebase uses a Makefile Build System to compile using GCC. You will need to download [Git](https://git-scm.com/install/windows), [MingW64](https://www.mingw-w64.org) and [Make](https://gnuwin32.sourceforge.net/packages/make.htm) and add them to your `PATH`.

Run the Makefile from the `Dev/Build` directory. It includes the following rules:
```
make debug
```
Which is the default rule and will compile `Dev/Build/Lib/core_debug.a` along with the source files present in the `Dev/App` directory in `DEBUG` mode. Build objects are cached within `Dev/Build/Obj/Debug` to avoid recompiling unchanged source files.
```
make release
```
Which will compile `Dev/Build/Lib/core_release.a` and the application files in `RELEASE` mode. Its objects are cached separately within `Dev/Build/Obj/Release`.
```
make clean
```
Which will delete cached build objects, executables, and the *.a* library binaries.


# Dependencies

The Optional Part of the Codebase deals with Graphics Contexts and depends on:
1. GLFW 3 (https://www.glfw.org/)
2. OpenGL 3.3 loaded using GLAD (https://glad.dav1d.de/)
3. STB_image (https://github.com/nothings/stb)

The Windows 64bit versions of these pre-compiled libraries are already within the `Dev/Dep` directory. If you are working on a different platform you will need to manually add the headers to the directory and link with their respective binaries.
# Documentation

## Base

This is a general purpose header-heavy library for working with C99, it includes 8 modules

### Platform Detection
### Macro Helpers
### Logging & Debug
### Generic Structs
### Math Defines
### Threads
### Directories & Files
### Text Serialization

## Optional

### Window System
### Layer System
### Event System
### Rendering Utilities

