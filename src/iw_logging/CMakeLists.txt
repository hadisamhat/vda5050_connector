cmake_minimum_required(VERSION 3.8)
project(iw_logging)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++17)

find_package(ament_cmake REQUIRED)

set(CPP_FILES
    src/directory_logger.cpp
)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)

add_library(logger_base INTERFACE)

target_link_libraries(logger_base INTERFACE stdc++fs)

target_include_directories(logger_base INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS logger_base EXPORT logger_baseTargets)

install(DIRECTORY include/ DESTINATION include)

ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)
ament_export_targets(logger_baseTargets HAS_LIBRARY_TARGET)

ament_package()
