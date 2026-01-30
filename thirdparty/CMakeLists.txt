add_subdirectory(glfw)
add_subdirectory(glad/cmake)

glad_add_library(glad_gl_core_33 SHARED API gl:core=3.3)
