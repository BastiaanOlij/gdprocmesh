#include <Godot.hpp>
#include "gdprocmesh.h"
#include "gdprocnode.h"
#include "gdprocsurface.h"
#include "gdprocbox.h"
#include "gdprocvector.h"
#include "gdprocgennormals.h"
#include "gdproctranslate.h"

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
	godot::register_tool_class<godot::GDProcSurface>();
	godot::register_tool_class<godot::GDProcBox>();
	godot::register_tool_class<godot::GDProcGenNormals>();
	godot::register_tool_class<godot::GDProcVector>();
	godot::register_tool_class<godot::GDProcTranslate>();
}
