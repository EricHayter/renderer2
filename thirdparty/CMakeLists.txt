add_subdirectory(glfw)
add_subdirectory(glad/cmake)
add_subdirectory(glm)

# Configure assimp options before adding subdirectory
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(assimp)

glad_add_library(glad_gl_core_33 SHARED API gl:core=3.3)
