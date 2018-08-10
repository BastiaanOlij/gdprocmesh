#include <Godot.hpp>

#include "gdprocmesh.h"
#include "gdprocnode.h"

#include "input/gdprocinreal.h"
#include "input/gdprocinvec3.h"

#include "primitives/gdprocvec3.h"

#include "transforms/gdprocgennormals.h"
#include "transforms/gdprocvec3translate.h"

#include "shapes/gdprocbox.h"

#include "output/gdprocoutput.h"
#include "output/gdprocsurface.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);

	// register our procedural mesh class
	godot::register_tool_class<godot::GDProcMesh>();

	// register all our nodes
	godot::register_tool_class<godot::GDProcNode>();

	// inputs
	godot::register_tool_class<godot::GDProcInReal>();
	godot::register_tool_class<godot::GDProcInVec3>();

	// primitives
	godot::register_tool_class<godot::GDProcVec3>();

	// transforms (work on primitives)
	godot::register_tool_class<godot::GDProcVec3Translate>();
	godot::register_tool_class<godot::GDProcGenNormals>();

	// shapes
	godot::register_tool_class<godot::GDProcBox>();

	// output
	godot::register_tool_class<godot::GDProcSurface>();
	godot::register_tool_class<godot::GDProcOutput>();

	// modifiers (work on surfaces)
}
