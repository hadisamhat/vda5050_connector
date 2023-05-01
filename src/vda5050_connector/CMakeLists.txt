cmake_minimum_required(VERSION 3.13.2)
project(vda5050_connector VERSION 1.0.0 LANGUAGES CXX C)
add_compile_options(-Werror -std=c++17)

find_package(nlohmann_json REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(iw_logging REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_subdirectory(interface)

set(CPP_FILES
      src/PolygonPoint.cpp
      src/ProtocolHeader.cpp
      src/Position.cpp
      src/Envelope2d.cpp
      src/Envelope3d.cpp
      src/AgvGeometry.cpp
      src/WheelDefinition.cpp
      src/Action.cpp
      src/AgvAction.cpp
      src/ActionState.cpp
      src/ActionParameter.cpp
      src/AgvPosition.cpp
      src/ArrayLength.cpp
      src/BatteryState.cpp
      src/BoundingBoxReference.cpp
      src/ControlPoint.cpp
      src/Edge.cpp
      src/Trajectory.cpp
        )

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

add_executable(test_function src/test.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

ament_export_include_directories(include)

ament_target_dependencies(${PROJECT_NAME} nlohmann_json iw_logging) 

target_link_libraries(${PROJECT_NAME} vda5050_connector_interface Boost::system)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)

ament_export_targets(vda5050_connector_interfaceTargets HAS_LIBRARY_TARGET)
ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)
ament_export_dependencies(nlohmann_json iw_logging)

ament_package()
