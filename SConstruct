#!python
import os, subprocess

target_path = 'demo/addons/gdprocmesh/bin/'
target_name = 'libgdprocmesh'

# Local dependency paths, adapt them to your setup
godot_headers_path = "godot-cpp/godot_headers/"
cpp_bindings_path = "godot-cpp/"
cpp_library = "godot-cpp"

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
    target_path += 'osx/'
    cpp_library += '.osx.64'
    env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64'])
    env.Append(LINKFLAGS = ['-arch', 'x86_64'])

if platform == "linux":
    target_path += 'x11/'
    cpp_library += '.linux.64'
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++14'])

if platform == "windows":
    target_path += 'win64/'
    cpp_library += '.windows.64'
    env.Append(CCFLAGS = ['-DWIN32', '-D_WIN32', '-D_WINDOWS', '-W3', '-GR', '-D_CRT_SECURE_NO_WARNINGS'])
    if target == "debug":
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])

env.Append(CPPPATH=['.', 'src/', godot_headers_path, cpp_bindings_path + 'include/', cpp_bindings_path + 'include/core/', cpp_bindings_path + 'include/gen/'])
env.Append(LIBPATH=[cpp_bindings_path + 'bin/'])
env.Append(LIBS=[cpp_library])

sources = []
add_sources(sources, "src")
add_sources(sources, "src/input")
add_sources(sources, "src/shapes")
add_sources(sources, "src/primitives")
add_sources(sources, "src/transforms")
add_sources(sources, "src/output")
add_sources(sources, "src/modifiers")

library = env.SharedLibrary(target=target_path + target_name, source=sources)
Default(library)
