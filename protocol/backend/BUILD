package(default_visibility = ["//visibility:public"])

cc_library(
    name = "interface",
    srcs = ["backend.cc"],
    hdrs = ["backend.h"],
)

cc_library(
    name = "backend_uart",
    srcs = [
        "uart/backend_uart.cc",
        "uart/baudrate.cc",
        "uart/serial.cc",
        "uart/baudrate.h",
        "uart/serial.h",
    ],
    hdrs = ["uart/backend_uart.h"],
    deps = [
        ":interface",
        "@spdlog"
    ],
)

cc_library(
    name = "backend",
    deps = [
        ":interface",
        ":backend_uart",
    ],
)
