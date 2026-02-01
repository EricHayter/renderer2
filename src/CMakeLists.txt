add_executable(renderer
    main.cpp
    shader.cpp
    window_management.cpp
)

target_include_directories(renderer
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(renderer
    PUBLIC
    glad_gl_core_33
    glfw
)
