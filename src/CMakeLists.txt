add_executable(renderer
    main.cpp
    shader.cpp
    texture.cpp
    window_management.cpp
    mesh.cpp
    model.cpp
)

target_include_directories(renderer
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/thirdparty
)

target_link_libraries(renderer
    PUBLIC
    glad_gl_core_33
    glfw
    glm::glm
    assimp
)
