package(default_visibility = ["//visibility:public"])

cc_library(
    name = "client",
    srcs = ["client/client.cc"],
    hdrs = ["client/client.h"],
)

cc_library(
    name = "client_mqueue",
    srcs = ["client/mqueue/client_mqueue.cc"],
    hdrs = ["client/mqueue/client_mqueue.h"],
    deps = [
        ":client",
        "//source/utils:mqueue",
        "@spdlog"
    ],
)

cc_binary(
    name = "yap3",
    srcs = ["main.cc"],
    deps = [
        "//source/backend",
        "//source/utils:mqueue",
        ":client_mqueue",
        "@spdlog"
    ],
)
