licenses(["notice"])  # MIT
cc_library(
    name = "vda5050_connector_interface",
    hdrs = glob(["interface/include/**/*.hpp"]),
    deps = [
        "//src/iw_logging",
        "//src/iw_state_machine",
        "//third_party/nlohmann:json",
        "@boost//:asio",
        "@boost//:date_time",
        "@boost//:algorithm",
    ],
    strip_include_prefix = "interface/include",
    visibility = ["//visibility:private"],
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
    alwayslink = True,
    linkstatic = 1,
    strip_include_prefix = "include/"        
)