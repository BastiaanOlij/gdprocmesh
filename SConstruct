#!python
import os, subprocess

# Local dependency paths, adapt them to your setup
godot_headers_path = ARGUMENTS.get("headers", "godot-cpp/godot_headers/")
cpp_bindings_path = ARGUMENTS.get("cpp_bindings_path", "godot-cpp/")
cpp_bindings_library_path = ARGUMENTS.get("cpp_bindings_library", "godot-cpp/bin/godot-cpp")

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

target_name = 'demo/addons/gdprocmesh/bin/'

if platform == "osx":
    env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64'])
    env.Append(LINKFLAGS = ['-arch', 'x86_64'])
    target_name += 'osx/'

if platform == "linux":
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++14'])
    target_name += 'x11/'

if platform == "windows":
    env.Append(CCFLAGS = ['-DWIN32', '-D_WIN32', '-D_WINDOWS', '-W3', '-GR', '-D_CRT_SECURE_NO_WARNINGS'])
    if target == "debug":
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])
    target_name += 'win64/'
    cpp_bindings_library_path += '.windows.64'

# , 'include', 'include/core'
env.Append(CPPPATH=['.', 'src/', godot_headers_path, cpp_bindings_path + 'include/', cpp_bindings_path + 'include/core/', cpp_bindings_path + 'include/gen/'])
env.Append(LIBS=[cpp_bindings_library_path])

sources = []
add_sources(sources, "src")

library = env.SharedLibrary(target=target_name + 'libgdprocmesh', source=sources)
Default(library)
