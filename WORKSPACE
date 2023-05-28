workspace(name = "vda5050_connector")

load("//third_party:dependencies.bzl", "get_vda5050_deps")

get_vda5050_deps()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
####################################################################################################
# Loads boost c++ library (https://www.boost.org/) and
# custom bazel build support (https://github.com/nelhage/rules_boost/)
# explicitly due to bazel bug: https://github.com/bazelbuild/bazel/issues/1550

