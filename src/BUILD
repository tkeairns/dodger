load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

# cc_library(
#     name = "hello-greet",
#     srcs = ["hello-greet.cc"],
#     hdrs = ["hello-greet.h"],
# )
# 
# cc_binary(
#     name = "hello-world",
#     srcs = ["hello-world.cc"],
#     deps = [
#         ":hello-greet",
#         "//lib:hello-time",
#     ],
# )

cc_binary(
    name = "libdodger.so",
    srcs = ["dodger.cpp"],
    deps = [
        "//lidar:lidarlite_v3",
    ],
    linkstatic=False,
    linkshared=True,
)

