load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

def clean_dep(dep):
    return str(Label(dep))

def get_vda5050_deps():
    """Loads external dependencies required to build apps"""
    new_git_repository(
        name = "nlohmann_json",
        build_file = clean_dep("//third_party:nlohmann_json.BUILD"),
        tag = "v3.10.0",
        remote = "https://github.com/nlohmann/json.git",
        licenses = ["@nlohmann_json/LICENSE.MIT",],
    )
