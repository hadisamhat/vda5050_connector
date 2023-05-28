load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def clean_dep(dep):
    return str(Label(dep))

def get_vda5050_deps():
    """Loads external dependencies required to build apps"""
    # http_archive(
    #     name = "github_nlohmann_json",
    #     sha256 = "230f3a03cefd586661ebab577a347c973d97a770afb89e22c52abc3c2a19d0a7",
    #     url = "https://github.com/nlohmann/json/releases/download/v3.10.0/json.hpp",
    #     build_file = clean_dep("//third_party:nlohmann_json.BUILD"),
    #     )

    new_git_repository(
        name = "aws_iot_sdk_cpp_v2",
        remote = "https://github.com/hadisamhat/aws_sdk_precompiled.git",
        commit = "f03f9bd610beafce4d395b4e0b7946ea52e776c5",
        build_file = clean_dep("//third_party:aws_iot_sdk_cpp_v2.BUILD"))
