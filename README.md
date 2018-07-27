Procedural Mesh Generation GDNative module
==========================================

This is still very early days so you're probably just looking at a module that creates a cube :P
We'll see how far we can take this.

You will need a recent Godot 3.1 master build to run this module

Downloading and compiling
=========================
You will need a C++ compiler, python and scons installed on your machine, same build tools as Godot so see [compiling](http://docs.godotengine.org/en/latest/development/compiling/index.html).
Instructions below have only been tested on Windows and the scons file may need tweaking on other platforms.

It is best to clone this repository from a terminal like so:
```
git clone --recursive https://github.com/BastiaanOlij/gdprocmesh.git
cd gdprocmesh
```

This project relies on [godot_headers](https://github.com/GodotNativeTools/godot_headers) and [godot-cpp](https://github.com/GodotNativeTools/godot-cpp) which have been submoduled in this repository.

These dependencies will need to be compiled:
```
cd godot-cpp
scons platform=windows generate-bindings=yes
cd ..
```

Now we can compile our module:
```
scons platform=windows
```

Note that you may want to compile both godot-cpp and our module with the added switch ```target=release``` to create a release build.

Trying out the test project
===========================
After successful compilation the dynamic library should be placed in the addons folder of the example demo project. Again I've only configured things for windows so you may need to tweak the gdprocmesh.gdnlib file for other platforms.

Simply import the demo project into Godot and have fun.

License
=======
I've made the source in this repository available under an MIT license. If you find it useful, it would always be nice to get a mention.

About this repository
---------------------
This repository was created by and is maintained by Bastiaan Olij a.k.a. Mux213

You can follow me on [Twitter](https://twitter.com/mux213) for regular updates.

Videos about my work with Godot can by found on my [Youtube page](https://www.youtube.com/channel/UCrbLJYzJjDf2p-vJC011lYw)
