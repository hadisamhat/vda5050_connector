cmake_minimum_required(VERSION 3.13.2)
project(vda5050_connector VERSION 1.0.0 LANGUAGES CXX C)
add_compile_options(-Werror -std=c++17)

find_package(nlohmann_json REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(iw_logging REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)

ament_target_dependencies(${PROJECT_NAME} INTERFACE
  nlohmann_json
  iw_logging
)

ament_export_dependencies(nlohmann_json iw_logging)

ament_export_include_directories(include)

ament_package()