load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "googletest",
  urls = ["https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip"],
  strip_prefix = "googletest-release-1.11.0",
)

http_archive(
  name = "spdlog",
  urls = ["https://github.com/gabime/spdlog/archive/refs/tags/v1.9.2.zip"],
  strip_prefix = "spdlog-1.9.2",
  build_file = "@//:external/BUILD.spdlog"
)
