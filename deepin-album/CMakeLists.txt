cmake_minimum_required(VERSION 3.1)

project(deepin-album LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_BINARY_DIR})
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h @ONLY)

find_package(DtkDeclarative REQUIRED)

# Application
add_subdirectory(src)

# Unit Tests
#add_subdirectory(tests)
TARGET_COMPILE_DEFINITIONS(deepin-album
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
