#include <Godot.hpp>

#include "gdprocoutput.h"
#include "gdprocvec3.h"
#include "gdprocmesh.h"
#include "gdprocnode.h"
#include "gdprocsurface.h"
#include "gdprocbox.h"
#include "gdprocgennormals.h"
#include "gdprocvec3translate.h"

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

	// primitives
	godot::register_tool_class<godot::GDProcVec3>();

	// transforms
	godot::register_tool_class<godot::GDProcVec3Translate>();
	godot::register_tool_class<godot::GDProcGenNormals>();

	// shapes
	godot::register_tool_class<godot::GDProcBox>();

	// modifiers

	// output
	godot::register_tool_class<godot::GDProcSurface>();
	godot::register_tool_class<godot::GDProcOutput>();
}
