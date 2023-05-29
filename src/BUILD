
cc_library(
    name = "json",
    hdrs = ["third_party/nlohmann/json.hpp"],
    strip_include_prefix = "third_party/",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "iw_logging",
    hdrs = [
        "iw_logging/include/iw_logging/logger_base.hpp",
        "iw_logging/include/iw_logging/std_logger.hpp"
    ],
    strip_include_prefix = "iw_logging/include/",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "iw_state_machine",
    hdrs = ["iw_state_machine/include/iw_state_machine/state_machine.hpp"],
    strip_include_prefix = "iw_state_machine/include/",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "vda5050_connector_interface",
    hdrs = glob(["vda5050_connector/interface/include/**/*.hpp"]),
    deps = [
        ":iw_logging",
        ":iw_state_machine",
        ":json",
        "@boost//:asio",
        "@boost//:date_time",
        "@boost//:algorithm",
    ],
    strip_include_prefix = "vda5050_connector/interface/include",
    visibility = ["//visibility:private"],
)


cc_library(
    name = "vda5050_connector",
    srcs = glob(["vda5050_connector/*.cpp"]),
    hdrs = glob(["vda5050_connector/include/**/*.hpp"]),
    visibility = ["//visibility:private"],
    deps = [
        ":vda5050_connector_interface",
        "@aws_iot_sdk_cpp_v2"
    ],
    strip_include_prefix = "vda5050_connector/include/"        
)

cc_library(
    name = "iw_vda5050_connector",
    srcs = glob(["iw_vda5050_connector/*.cpp"]),
    hdrs = glob(["iw_vda5050_connector/include/**/*.hpp"]),
    visibility = ["//visibility:public"],
    deps = [
        ":vda5050_connector"
    ],
    strip_include_prefix = "iw_vda5050_connector/include/",
    copts = ["-Wno-reorder"]
)