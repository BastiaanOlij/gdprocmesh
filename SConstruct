#!python
import os

# https://www.scons.org/doc/production/HTML/scons-man.html

# Reads variables from an optional file.
customs = ['../custom.py']
opts = Variables(customs, ARGUMENTS)

# Gets the standart flags CC, CCX, etc.
env = DefaultEnvironment()

opts.Add(EnumVariable('target', "Compilation target", 'debug',
    ['d', 'debug', 'r', 'release']))

# Godot: android  haiku  iphone  javascript  osx  server  uwp  windows  x11.
# Add linux alias for x11.
opts.Add(EnumVariable('platform', "Compilation platform", 'windows',
    ['windows', 'x11', 'linux', 'osx']))

# Keeps p for backwards compatibility.
opts.Add(EnumVariable('p', "Compilation target, alias for 'platform'", 'not set',
    ['windows', 'x11', 'linux', 'osx', 'not set']))

opts.Add(BoolVariable('use_llvm', "Use the LLVM / Clang compiler",
    'no'))


# Local dependency paths, adapt them to your setup.
opts.AddVariables(
    PathVariable('target_path', 'The path where the lib is installed.', 'demo/addons/gdprocmesh/bin/'),
    PathVariable('target_name', 'The library name.', 'libgdprocmesh', PathVariable.PathAccept),
)
godot_headers_path = "godot-cpp/godot_headers/"
cpp_bindings_path = "godot-cpp/"
cpp_library = "godot-cpp"


# Compilation environment setup.
opts.Add("CXX", "C++ compiler")


# Updates the environment with the option variables.
opts.Update(env)

if env['use_llvm']:
    env['CXX'] = 'clang++'

if env['p'] != 'not set':
    env['platform'] = env['p']

if env['platform'] == 'windows':
    env['target_path'] += 'win64/'
    env['cpp_library'] += '.windows.64'
    if not env['use_llvm']:

        # This makes sure to keep the session environment variables on windows,
        # that way you can run scons in a vs 2017 prompt and it will find all the required tools
        env.Append(ENV = os.environ)

        env.Append(CCFLAGS = ['-DWIN32', '-D_WIN32', '-D_WINDOWS', '-W3', '-GR', '-D_CRT_SECURE_NO_WARNINGS'])
        if env['target'] in ('debug', 'd'):
            env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
        else:
            env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])
    # untested
    else:
        if env['target'] in ('debug', 'd'):
            env.Append(CCFLAGS = ['-fPIC', '-g3','-Og', '-std=c++17'])
        else:
            env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++17'])

# untested
elif env['platform'] == 'osx':
    env['target_path'] += 'osx/'
    env['cpp_library'] += '.osx.64'
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS = ['-g','-O2', '-arch', 'x86_64'])
        env.Append(LINKFLAGS = ['-arch', 'x86_64'])
    else:
        env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64'])
        env.Append(LINKFLAGS = ['-arch', 'x86_64'])

elif env['platform'] in ('x11', 'linux'):
    env['target_path'] += 'x11/'
    cpp_library += '.linux.64'
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS = ['-fPIC', '-g3','-Og', '-std=c++17'])
    else:
        env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++17'])

env.Append(CPPPATH=['.', 'src/', godot_headers_path,
    cpp_bindings_path + 'include/',
    cpp_bindings_path + 'include/core/',
    cpp_bindings_path + 'include/gen/'])
env.Append(LIBPATH=[cpp_bindings_path + 'bin/'])
env.Append(LIBS=[cpp_library])

sources = Glob('src/*.cpp')
sources += Glob('src/*/*.cpp')

library = env.SharedLibrary(target=env['target_path'] + env['target_name'],
    source=sources)

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))
