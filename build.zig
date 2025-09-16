const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const dep_sdl = b.dependency("sdl", .{
        .target = target,
        .optimize = optimize,
    });
    const lib_sdl3 = dep_sdl.artifact("SDL3");

    const mod_exe = b.createModule(.{
        .root_source_file = null,
        .target = target,
        .optimize = optimize,
    });

    const exe = b.addExecutable(.{
        .name = "sdl_example",
        .root_module = mod_exe,
    });
    exe.addCSourceFiles(.{
        .root = b.path("src"),
        .flags = &.{},
        .files = &.{
            "main.cpp",
        },
    });

    exe.linkLibrary(lib_sdl3);
    exe.linkLibCpp();

    b.installArtifact(exe);
}
