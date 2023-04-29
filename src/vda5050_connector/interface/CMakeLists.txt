add_library(vda5050_connector_interface INTERFACE)

target_include_directories(
        vda5050_connector_interface 
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS vda5050_connector_interface  EXPORT vda5050_connector_interfaceTargets)

install(DIRECTORY include/ DESTINATION include)

ament_export_targets(vda5050_connector_interfaceTargets HAS_LIBRARY_TARGET)
