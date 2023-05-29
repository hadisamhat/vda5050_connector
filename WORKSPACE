workspace(name = "vda5050_connector")

load("//third_party:dependencies.bzl", "get_vda5050_deps")

get_vda5050_deps()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
####################################################################################################
# Loads boost c++ library (https://www.boost.org/) and
# custom bazel build support (https://github.com/nelhage/rules_boost/)
# explicitly due to bazel bug: https://github.com/bazelbuild/bazel/issues/1550

http_archive(
    name = "com_github_nelhage_rules_boost",
    patches = ["//third_party:rules_boost.patch"],
    sha256 = "1479f6a46d37c415b0f803186bacb7a78f76305331c556bba20d13247622752a",
    type = "tar.gz",
    url = "https://developer.nvidia.com/isaac/download/third_party/rules_boost-82ae1790cef07f3fd618592ad227fe2d66fe0b31-tar-gz",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()
