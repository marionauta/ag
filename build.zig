const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "ag",
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    exe.linkLibC();

    exe.addIncludePath(.{ .cwd_relative = "/opt/homebrew/opt/luajit/include/luajit-2.1" });
    exe.linkSystemLibrary("luajit");

    // raylib
    exe.linkSystemLibrary("raylib");

    // raygui
    exe.addIncludePath(b.path("vendor"));
    exe.addCSourceFile(.{ .file = b.path("vendor/raygui.o") });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    const run_step = b.step("run", "Run agents");
    run_step.dependOn(&run_cmd.step);
}
