cc_library(
    name = "vda5050_connector_interface",
    hdrs = glob(["interface/include/**/*.hpp"]),
    deps = [
        "//third_party/nlohmann:json",
        "@boost//:asio",
        "//src/iw_logging",
        "//src/iw_state_machine"
    ],
    strip_include_prefix = "interface/include"
)


cc_library(
    name = "vda5050_connector",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    visibility = ["//visibility:public"],
    deps = [
        ":vda5050_connector_interface",
        "@aws_iot_sdk_cpp_v2" 
    ],
    strip_include_prefix = "include/"        
)