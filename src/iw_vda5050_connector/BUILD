package_group(
    name = "vda_5050_group",
    packages = [
        "//src/vda5050_connector",
        "//src/iw_vda5050_connector",
    ],
)

cc_library(
    name = "iw_vda5050_connector",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    visibility = ["//visibility:public"],
    deps = [
        "//src/vda5050_connector",
        "@boost//:asio",
        "@boost//:date_time",
        "@boost//:algorithm",
    ],
    strip_include_prefix = "include/",
    copts = ["-Wno-reorder"]
)