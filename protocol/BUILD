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
        "//protocol/utils:mqueue",
        "@spdlog"
    ],
)

cc_library(
    name = "configuration",
    srcs = ["configuration.cc"],
    hdrs = ["configuration.h"],
    deps = [
        "//protocol/utils:ringbuffer",
    ],
)

cc_library(
    name = "protocol",
    srcs = ["protocol.cc"],
    hdrs = ["protocol.h"],
    deps = [
        "//protocol/layers",
        "@spdlog",
    ],
)

cc_binary(
    name = "yap3",
    srcs = ["yap3.cc"],
    deps = [
        ":protocol",
    ],
)
