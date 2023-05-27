load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

def clean_dep(dep):
    return str(Label(dep))

def get_vda5050_deps():
    """Loads external dependencies required to build apps"""
    new_git_repository(
        name = "nlohmann_json",
        remote = "https://github.com/hadisamhat/nlohmann_json.git",
        commit = "cffcb2a767eba4f19523b7a1bcc87f2e0b02100e",
        build_file = clean_dep("//third_party:nlohmann_json.BUILD"))

    new_git_repository(
        name = "aws_iot_sdk_cpp_v2",
        remote = "https://github.com/hadisamhat/aws_sdk_precompiled.git",
        commit = "f03f9bd610beafce4d395b4e0b7946ea52e776c5",
        build_file = clean_dep("//third_party:aws_iot_sdk_cpp_v2.BUILD"))
