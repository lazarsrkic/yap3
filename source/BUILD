
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

cc_binary(
    name = "yap3",
    srcs = ["main.cc"],
    deps = [
        ":backend_uart",
        "@spdlog"
    ],
)
