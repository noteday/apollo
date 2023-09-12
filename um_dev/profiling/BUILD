load("@rules_cc//cc:defs.bzl", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "result_writer",
    srcs = ["result_writer.cc"],
    hdrs = ["result_writer.h"],
    deps = [
        "//cyber/time:time",
        "//cyber/time:duration",
        "//um_dev/utils:files",
    ],
)

cpplint()