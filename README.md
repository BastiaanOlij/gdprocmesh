Procedural Mesh Generation GDNative module
==========================================

This is my attempt at creating a procedural mesh generator for the [Godot Game Engine](https://godotengine.org/).

![example](https://github.com/BastiaanOlij/gdprocmesh/raw/master/example.png) 

The "normal" way of creating 3D assets for your game involves using modeling software such as Maya, Blender, 3DSMax or any of the other tools out there and for many of the assets you'll want to use in your game this is by far the prefered way of creating assets. There is however one limitation to using these types of software to create 3D assets and that is that they are essentially static. Yes you can animate them but if you create a 2 story house, you'll have to create a new asset if you wish to have a 3 story house.

This is the type of problem procedural modeling attempts to solve. Instead of focussing on an end result the goal is to tell the computer how an asset is to be constructed. 
This is best explained with a video:
[Timelapse - Procedural modeling in Godot](https://youtu.be/X574IIBgOko)

That video shows the construction of a simple rope bridge where the length and width can be adjusted and instead of the bridge getting stretched, as would happen if you scale the bridge, it actually adds ropes and boards.

You will need a recent Godot 3.1 master build to run this module.
You can download a nightly build of Godot 3.1 from Hugos [Unofficial Godot Engine builds](https://hugo.pro/projects/godot-builds/) website.
A copy of the 64bit Windows DLL is included in the demo folder, use at your own risk.

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
scons platform=windows generate_bindings=yes use_custom_api_file=yes custom_api_file=../api.json
cd ..
```
(custom api is temporary as we needed a newer api file)

You can add ```-j<n>``` to this to speed up compiling, replace ```<n>``` with the number of cores your machine has.

Now we can compile our module:
```
scons platform=windows
```

Note that you may want to compile both godot-cpp and our module with the added switch ```target=release``` to create a release build.

Trying out the test project
===========================
After successful compilation the dynamic library should be placed in the addons folder of the example demo project. Again I've only configured things for windows so you may need to tweak the gdprocmesh.gdnlib file for other platforms.

Simply import the demo project into Godot and have fun.

If you want to use this for your own projects, simply copy the addons folder into your project and make sure that the plugin is turned on.

Now create a ```MeshInstance``` and create an ```ArrayMesh``` for this node. You can now drag the gdprocmesh.gdns file into the script property of the ArrayMesh. This will create a simple box as a starting template.
Simply re-select the ArrayMesh in the property manager and you'll see that you're given a new graph editor.

Everything is an array
======================
Well, almost everything. Most nodes are designed to take arrays as input and you can do some funky things with this. 

For instance, on face value GDProcScale will scale an array of vertices. So your input is an array of vertices, you specify a vertex by which to scale this input, and your output results in an array of vertices that have been scaled according to the 2nd input.
However the 2nd input can be an array too. Let's say you input 5 vertices with 3 scales:
Vertice 1 will be scaled by the first scale
Vertice 2 will be scaled by the second scale
Vertice 3 will be scaled by the third scale
Vertice 4 will be scaled by the first scale
Vertice 5 will be scaled by the second scale
So the scales are applied in order and it simply repeats the pattern until all vertices are scaled.

Many of the nodes work in this way where the different input arrays are combined to create the output. 

Similarly primitives like the vector and euler nodes will output arrays if the input is an array of value.

Rotation
========
To rotate elements of your mesh there are various nodes that take a rotation as input. The current input options all center around euler angles but internally these are converted to quaternions.
Due to lacking a quaternion array construct we're using a vec3 array and passing uniform quaternions around where w is assumed to be sqrt(1.0 - (x² + y² + z²)).
It is then these quaternions that are passed from node to node.
Additional nodes will be added in the near future that will allow you to construct rotations on other inputs like a rotation around an axis.

License
=======
I've made the source in this repository available under an MIT license. If you find it useful, it would always be nice to get a mention.

The following 3rd party libraries are used:

* [Fast Quadric Mesh Simplication](https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification) - MIT licensed

Note that for the demo:

* HRDI texture is from [HDRI Haven](https://hdrihaven.com)
* Rope and plank texture is from [CC0 Textures.com](https://cc0textures.com)
* Not sure where the numbered textures came from

Please visit these sites for further license details

About this repository
---------------------
This repository was created by and is maintained by Bastiaan Olij a.k.a. Mux213

You can follow me on [Twitter](https://twitter.com/mux213) for regular updates.

Videos about my work with Godot can by found on my [Youtube page](https://www.youtube.com/channel/UCrbLJYzJjDf2p-vJC011lYw)
