package(default_visibility = ["//visibility:public"])

cc_library(
    name = "layer",
    srcs = ["layer.cc"],
    hdrs = ["layer.h"],
)

cc_library(
    name = "datalink",
    srcs = [
        "datalink.cc",
    ],
    hdrs = [
        "datalink.h"
    ],
    deps = [
        ":layer",
        "//source/backend",
    ],
)

cc_library(
    name = "application",
    srcs = [
        "application.cc",
    ],
    hdrs = [
        "application.h"
    ],
    deps = [
        ":layer",
    ],
)

cc_library(
    name = "presentation",
    srcs = [
        "presentation.cc",
    ],
    hdrs = [
        "presentation.h"
    ],
    deps = [
        ":layer",
    ],
)

cc_library(
    name = "transport",
    srcs = [
        "transport.cc",
    ],
    hdrs = [
        "transport.h"
    ],
    deps = [
        ":layer",
    ],
)

cc_library(
    name = "layers",
    deps = [
        ":layer",
        ":application",
        ":presentation",
        ":transport",
        ":datalink"
    ],
)
