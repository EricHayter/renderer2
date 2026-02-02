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
./src/renderer
```
