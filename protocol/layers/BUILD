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
        "//protocol/backend",
        "//protocol/utils:ringbuffer",
        "//protocol/utils:timer",
        "//protocol/utils:crc16",
        "//protocol/utils:slidingwindow",
        "//protocol:configuration"
    ]
)
