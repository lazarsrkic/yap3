package(default_visibility = ["//visibility:public"])

cc_library(
    name = "layers",
    srcs = [
        "datalink.cc",
        "transport.cc",
        "presentation.cc",
        "application.cc"
    ],
    hdrs = [
        "datalink.h",
        "transport.h",
        "presentation.h",
        "application.h"
    ],
    deps = [
        "//source/backend",
        "//source/utils:ringbuffer",
        "//source/utils:timer",
        "//source/utils:crc16",
        "//source/utils:slidingwindow",
        "//source:configuration"
    ]
)
