#!python
import os, subprocess

# Local dependency paths, adapt them to your setup
godot_headers_path = ARGUMENTS.get("headers", "godot_headers/")
zlib_path = ARGUMENTS.get("zlib_path", "zlib/")
zlib_library_path = ARGUMENTS.get("zlib_library", "zlib/build/release/zlibstatic")
libpng_library_path = ARGUMENTS.get("libpng_library", "libpng/build/Release/libpng16_static")
libvnc_path = ARGUMENTS.get("libvnc_path", "libvncserver/")
libvnc_library_path = ARGUMENTS.get("libvnc_library", "libvncserver/build/Release/vncclient")
cpp_bindings_path = ARGUMENTS.get("cpp_bindings_path", "cpp_bindings/")
cpp_bindings_library_path = ARGUMENTS.get("cpp_bindings_library", "cpp_bindings/bin/godot_cpp_bindings")

target = ARGUMENTS.get("target", "debug")

# platform= makes it in line with Godots scons file, keeping p for backwards compatibility
platform = ARGUMENTS.get("p", "windows")
platform = ARGUMENTS.get("platform", platform)

# This makes sure to keep the session environment variables on windows, 
# that way you can run scons in a vs 2017 prompt and it will find all the required tools
env = Environment()
if platform == "windows":
    env = Environment(ENV = os.environ)

if ARGUMENTS.get("use_llvm", "no") == "yes":
    env["CXX"] = "clang++"

def add_sources(sources, directory):
    for file in os.listdir(directory):
        if file.endswith('.cpp'):
            sources.append(directory + '/' + file)

if platform == "osx":
    env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64'])
    env.Append(LINKFLAGS = ['-arch', 'x86_64'])

if platform == "linux":
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++14'])

if platform == "windows":
    env.Append(CCFLAGS = ['-DWIN32', '-D_WIN32', '-D_WINDOWS', '-W3', '-GR', '-D_CRT_SECURE_NO_WARNINGS'])
    if target == "debug":
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])

# , 'include', 'include/core'
env.Append(CPPPATH=['.', 'src/', godot_headers_path, zlib_path, libvnc_path, libvnc_path + 'common/', libvnc_path + 'build/', cpp_bindings_path + 'include/', cpp_bindings_path + 'include/core/'])
env.Append(LIBS=[libvnc_library_path, libpng_library_path, zlib_library_path, cpp_bindings_library_path, 'WS2_32'])

sources = []
add_sources(sources, "src")

library = env.SharedLibrary(target='demo/bin/gdvnc', source=sources)
Default(library)
