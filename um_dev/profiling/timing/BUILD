load("@rules_cc//cc:defs.bzl", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "timing",
    srcs = ["timing.cc"],
    hdrs = ["timing.h"],
    deps = [
        "//cyber/time:time",
        "//cyber/time:duration",
        "//um_dev/profiling:result_writer",
    ],
)