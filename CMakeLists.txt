cmake_minimum_required(VERSION 3.25)

project(Renderer2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure Assimp to only build needed importers
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)     # .obj and .mtl
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)    # .gltf and .glb

add_subdirectory(thirdparty)
add_subdirectory(src)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
  message(STATUS "Found clang-format at ${CLANG_FORMAT}")
endif()

get_target_property(target_sources renderer SOURCES)
if(CLANG_FORMAT)
  add_custom_target(format
    COMMENT "Formatting files with clang-format..."
    COMMAND ${CLANG_FORMAT} -i ${target_sources}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
  )
  message(STATUS "Added 'format' target")
else()
  message(WARNING
    "clang-format not found - format target unavailable")
endif()
