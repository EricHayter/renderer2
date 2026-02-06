add_subdirectory(glfw)
add_subdirectory(glad/cmake)
add_subdirectory(glm)

# Configure assimp options before adding subdirectory
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(assimp)

# imgui setup
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    imgui
    imgui/backends)
target_link_libraries(imgui
    PUBLIC
    glfw
)

glad_add_library(glad_gl_core_33 SHARED API gl:core=3.3)
