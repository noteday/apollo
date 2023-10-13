load("@rules_cc//cc:defs.bzl", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "timing_channel",
    srcs = ["timing_channel.cc"],
    hdrs = ["timing_channel.h"],
    deps = [
        "//cyber",
        "//um_dev/profiling/timing_channel:timing_message_cc_proto",
    ],
)

proto_library(
    name = "timing_message_proto",
    srcs = ["timing_message.proto"],
)

cc_proto_library(
    name = "timing_message_cc_proto",
    deps = [":timing_message_proto"],
)


# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "test",
    srcs = ["test.cc"],
    deps = ["//um_dev/profiling/timing_channel:timing_channel"],
)

cpplint