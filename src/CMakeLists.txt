add_executable(renderer main.cpp)

target_link_libraries(renderer
    PUBLIC
    glad_gl_core_33
    glfw
)
