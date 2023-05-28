load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def clean_dep(dep):
    return str(Label(dep))

def get_vda5050_deps():
    """Loads external dependencies required to build apps"""
    http_archive(
        name = "nlohmann_json",
        sha256 = "b5e3bfad07feba218a26a4f809fbb0d1e33450524bf5d7244cabc92cf8178c69",
        type = "zip",
        url = "https://github.com/nlohmann/json/releases/download/v3.10.0/include.zip",
        build_file = "//third_party:nlohmann_json.BUILD",
        )

    new_git_repository(
        name = "aws_iot_sdk_cpp_v2",
        remote = "https://github.com/hadisamhat/aws_sdk_precompiled.git",
        commit = "f03f9bd610beafce4d395b4e0b7946ea52e776c5",
        build_file = clean_dep("//third_party:aws_iot_sdk_cpp_v2.BUILD"))
