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

    
    new_git_repository(
        name = "aws_iot_sdk_cpp_v2",
        remote = "https://github.com/hadisamhat/aws_sdk_precompiled.git",
        commit = "d4a2e9f6a6d033e48803215705245f2b7f324500",
        build_file = clean_dep("//third_party:aws_iot_sdk_cpp_v2.BUILD"))
