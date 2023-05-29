config_setting(
    name = "platform_x86_64",
    define_values = {"target_platform": "x86_64"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "platform_jetpack44",
    define_values = {"target_platform": "jetpack44"},
    visibility = ["//visibility:public"],
)

cc_library(
    name="aws_iot_sdk_cpp_v2",
    hdrs = select({
        ":platform_x86_64" : glob(["install_x86/include/**"]),
        ":platform_jetpack44":  glob(["install_aarch64/include/**"])}),
    srcs = select({
        ":platform_x86_64" : glob(["install_x86/lib/*.a"]),
        ":platform_jetpack44":  glob(["install_aarch64/lib/*.a"])}),
    strip_include_prefix= select({
        ":platform_x86_64" : "install_x86/include",
        ":platform_jetpack44": "install_aarch64/include"}),
    visibility=["//visibility:public"]
)
