# renderer2

An OpenGL-based renderer built as an evolution of my [previous software renderer](https://github.com/EricHayter/renderer). This version uses modern OpenGL and GPU shaders instead of doing all rendering calculations by hand.

Following [LearnOpenGL](https://learnopengl.com/) and documenting everything I learn in `notes.txt`.

## Building

Clone with submodules:
```bash
git clone --recursive https://github.com/EricHayter/renderer2
cd renderer2
```

If you already cloned without `--recursive`:
```bash
git submodule update --init --recursive
```

Build:
```bash
mkdir build
cd build
cmake ..
cmake --build .
./src/renderer ../models/plant.obj
```

## Architecture

![Architecture Diagram](docs/diagrams/export/architecture.png)

The renderer is organized into distinct classes handling windowing, rendering, models, and materials. See the diagram above for the relationships between major components.

## Dependencies

This project uses the following third-party libraries:

- **OpenGL 3.3 Core** - Modern graphics API for GPU-accelerated rendering
- **GLFW** - Cross-platform windowing, input handling, and OpenGL context creation
- **glad** - OpenGL function loader generated for OpenGL 3.3 Core Profile
- **GLM** - Header-only mathematics library providing vector and matrix operations matching GLSL
- **Assimp** - Asset import library for loading 3D models (.obj, .fbx, etc.) with material and texture support
- **stb_image** - Single-header image loading library for texture data (PNG, JPG, etc.)
- **ImGui** - Immediate mode GUI library for runtime debugging and parameter tweaking
- **CMake** - Build system for managing compilation across platforms
