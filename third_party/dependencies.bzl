load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def clean_dep(dep):
    return str(Label(dep))

def get_vda5050_deps():

    # http_archive(
    # name = "com_github_nelhage_rules_boost",
    # patches = ["//third_party:rules_boost.patch"],
    # build_file = clean_dep("//third_party:asio.BUILD"),
    # sha256 = "1479f6a46d37c415b0f803186bacb7a78f76305331c556bba20d13247622752a",
    # type = "tar.gz",
    # url = "https://developer.nvidia.com/isaac/download/third_party/rules_boost-82ae1790cef07f3fd618592ad227fe2d66fe0b31-tar-gz")

    http_archive(
        name = "asio",
        build_file = clean_dep("//third_party:asio.BUILD"),
        sha256 = "53672dcf3cf3394da10b32880de88b30e5787a04d121aa5dd92d4f3e577cf78e",
        url = "https://developer.nvidia.com/isaac/download/third_party/asio-1-16-0-tar-gz",
        type = "tar.gz",
        strip_prefix = "asio-1.16.0"
        )

    new_git_repository(
        name = "aws_iot_sdk_cpp_v2",
        remote = "https://github.com/hadisamhat/aws_sdk_precompiled.git",
        commit = "f03f9bd610beafce4d395b4e0b7946ea52e776c5",
        build_file = clean_dep("//third_party:aws_iot_sdk_cpp_v2.BUILD"))
