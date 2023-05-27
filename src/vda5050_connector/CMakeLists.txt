cmake_minimum_required(VERSION 3.13.2)
project(vda5050_connector VERSION 1.0.0 LANGUAGES CXX C)
add_compile_options(-Werror -std=c++17)

list(APPEND CMAKE_PREFIX_PATH "/opt/iw/usr/lib")

find_package(nlohmann_json REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(iw_logging REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(aws-crt-cpp REQUIRED)
find_package(iw_state_machine REQUIRED)

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
      src/EdgeState.cpp
      src/Error.cpp
      src/ErrorReference.cpp
      src/FactSheet.cpp
      src/LoadDimension.cpp
      src/LoadSet.cpp
      src/LoadSpecification.cpp
      src/OptionalParameter.cpp
      src/PhysicalParameters.cpp
      src/ProtocolFeatures.cpp
      src/ProtocolLimits.cpp
      src/StringLength.cpp
      src/TimingInfo.cpp
      src/TypeSpecification.cpp
      src/Trajectory.cpp
      src/Info.cpp
      src/InfoReference.cpp
      src/InstantAction.cpp
      src/Node.cpp
      src/NodeState.cpp
      src/Order.cpp
      src/SafetyState.cpp
      src/State.cpp
      src/Velocity.cpp
      src/NodePosition.cpp
      src/Visualization.cpp
      src/Connection.cpp
)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})


target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

ament_export_include_directories(include)

ament_target_dependencies(${PROJECT_NAME}  iw_state_machine nlohmann_json iw_logging) 

target_link_libraries(${PROJECT_NAME} vda5050_connector_interface  Boost::system AWS::aws-crt-cpp)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)

install(DIRECTORY include/ DESTINATION include)

ament_export_targets(vda5050_connector_interfaceTargets HAS_LIBRARY_TARGET)
ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)
ament_export_dependencies(nlohmann_json iw_state_machine iw_logging aws-crt-cpp)

ament_package()
