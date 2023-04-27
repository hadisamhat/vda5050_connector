cmake_minimum_required(VERSION 3.8)
project(iw_state_machine)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++17)

find_package(ament_cmake REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)

install(DIRECTORY include/ DESTINATION include)

ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)

ament_package()