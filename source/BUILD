package(default_visibility = ["//visibility:public"])

cc_library(
    name = "backend",
    srcs = ["datalink/backend.cc"],
    hdrs = ["datalink/backend.h"],
)

cc_library(
    name = "baudrate",
    srcs = ["datalink/uart/baudrate.cc"],
    hdrs = ["datalink/uart/baudrate.h"],
    deps = [
        "@spdlog"
    ]
)

cc_library(
    name = "serial",
    srcs = ["datalink/uart/serial.cc"],
    hdrs = ["datalink/uart/serial.h"],
    deps = [
        ":baudrate",
        "@spdlog"
    ]
)

cc_library(
    name = "backend_uart",
    srcs = ["datalink/uart/backend_uart.cc"],
    hdrs = ["datalink/uart/backend_uart.h"],
    deps = [
        ":backend",
        ":serial",
        "@spdlog"
    ]
)

cc_library(
    name = "mqueue",
    srcs = ["utils/mqueue.cc"],
    hdrs = ["utils/mqueue.h"],
    deps = [
        "@spdlog"
    ],
)

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
        ":mqueue",
        "@spdlog"
    ],
)

cc_binary(
    name = "yap3",
    srcs = ["main.cc"],
    deps = [
        ":backend_uart",
        ":mqueue",
        ":client_mqueue",
        "@spdlog"
    ],
)
