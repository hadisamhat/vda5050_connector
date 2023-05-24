cmake_minimum_required(VERSION 3.13.2)
project(iw_vda5050_connector VERSION 1.0.0 LANGUAGES CXX C)
add_compile_options(-Werror -std=c++17)

list(APPEND CMAKE_PREFIX_PATH "/opt/iw/usr/lib")

find_package(ament_cmake REQUIRED)
find_package(vda5050_connector REQUIRED)
find_package(aws-crt-cpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

set(CPP_FILES
      src/Action.cpp
      src/InteractionZoneState.cpp
      src/InstantAction.cpp
      src/Load.cpp
      src/MapUpdate.cpp
      src/State.cpp
      src/Visualization.cpp
      src/Zone.cpp
      src/ZoneParameter.cpp
      src/ZoneUpdate.cpp
      src/Vda5050Manager.cpp
)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

add_executable(test_function src/test.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

ament_export_include_directories(include)

target_link_libraries(${PROJECT_NAME} Boost::system AWS::aws-crt-cpp)

target_link_libraries(test_function ${PROJECT_NAME} AWS::aws-crt-cpp)

ament_target_dependencies(${PROJECT_NAME} vda5050_connector nlohmann_json iw_logging) 

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS test_function RUNTIME DESTINATION bin/${PROJECT_NAME})

ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)
ament_export_dependencies(vda5050_connector)

ament_package()
