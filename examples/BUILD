cc_binary(
    name = "yap3client",
    srcs = ["yap3client.cc"],
    deps = [
        "//source:client_mqueue",
        "@spdlog"
    ],
)

cc_binary(
    name = "yap3peer",
    srcs = ["yap3peer.cc"],
    deps = [
        "//source:backend_uart",
        "//source:client_mqueue",
        "@spdlog"
    ],
)
