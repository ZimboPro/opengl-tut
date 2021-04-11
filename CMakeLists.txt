cmake_minimum_required (VERSION 3.11 FATAL_ERROR)

project(glTut)

# set(default_build_type "Release")
set(FETCHCONTENT_QUIET OFF)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# Glew
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.2.0
)

FetchContent_GetProperties(glew)

if(NOT glew_POPULATED)
  FetchContent_Populate(glew)
  set(glew-cmake_BUILD_STATIC ON)
  set(glew-cmake_BUILD_SHARED OFF)
  add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR})
endif()

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.4
)

FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()


# STB_Image
FetchContent_Declare(
    std_image
    GIT_REPOSITORY https://github.com/jschmidt42/stb-cmake.git
)

FetchContent_GetProperties(std_image)

if(NOT std_image_POPULATED)
  FetchContent_Populate(std_image)
endif()

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
  FetchContent_Populate(glm)
endif()

# Freetype2
FetchContent_Declare(
    freetype2
    URL https://download.savannah.gnu.org/releases/freetype/freetype-2.9.1.tar.gz
)

FetchContent_GetProperties(freetype2)

if(NOT freetype2_POPULATED)
  FetchContent_Populate(freetype2)
  add_subdirectory(${freetype2_SOURCE_DIR} ${freetype2_BINARY_DIR})
endif()



if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif(APPLE)

# Assimp
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.0.1
)

FetchContent_GetProperties(assimp)

if(NOT assimp_POPULATED)
  FetchContent_Populate(assimp)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

  add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
endif()

file(GLOB SRC src/*.cpp src/**/*.cpp)

# library
add_executable(
    glTut

    ${SRC}
)


find_package(OpenGL REQUIRED)

if (OPENGL_GLU_FOUND)
    target_link_libraries(glTut PUBLIC ${OPENGL_glu_LIBRARY})
endif()

target_include_directories(glTut PUBLIC 
    ${glew_SOURCE_DIR}/include 
    ${glfw_SOURCE_DIR}/include
    ${sfml_SOURCE_DIR}/include 
    ${std_image_SOURCE_DIR} 
    ${glm_SOURCE_DIR} 
    ${assimp_SOURCE_DIR}/include
    ${freetype2_SOURCE_DIR}/include

    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(glTut PUBLIC libglew_static glfw assimp freetype ${OPENGL_LIBRARIES})

message("Libs" ${GLEW_TARGETS})
# install
install(TARGETS glTut DESTINATION ${CMAKE_BINARY_DIR}/Graphics)
set(INSTALL_DIR)

# CPack
set(CPACK_GENERATOR "OSXX11" CACHE STRING "" FORCE)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Leslie" CACHE STRING "" FORCE)
set(CPACK_PACKAGE_NAME "glTut" CACHE STRING "" FORCE)
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_BINARY_DIR} CACHE PATH "" FORCE)

include(CPack)

# add_executable(
#     og
#     src/main.cpp
#     src/Functions.cpp
# )

# target_link_libraries(og glTut)