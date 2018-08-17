#include <Godot.hpp>

#include "gdprocmesh.h"
#include "gdprocnode.h"

#include "input/gdprocincurve.h"
#include "input/gdprocinint.h"
#include "input/gdprocinpoolvectors.h"
#include "input/gdprocinreal.h"
#include "input/gdprocinvector.h"

#include "primitives/gdproceuler.h"
#include "primitives/gdprocvector.h"

#include "transforms/gdprocbevel.h"
#include "transforms/gdprocgennormals.h"
#include "transforms/gdprocmult.h"
#include "transforms/gdprocredist.h"
#include "transforms/gdprocrotate.h"
#include "transforms/gdprocscale.h"
#include "transforms/gdproctranslate.h"

#include "shapes/gdprocbox.h"
#include "shapes/gdproccircle.h"
#include "shapes/gdprocrect.h"

#include "surfaces/gdprocextrudeshape.h"
#include "surfaces/gdprocsurface.h"

#include "modifiers/gdprocmirror.h"
#include "modifiers/gdprocplaceonpath.h"
#include "modifiers/gdproctransform.h"

#include "output/gdprocoutput.h"

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
	godot::register_tool_class<godot::GDProcInCurve>();
	godot::register_tool_class<godot::GDProcInInt>();
	godot::register_tool_class<godot::GDProcInPoolVectors>();
	godot::register_tool_class<godot::GDProcInReal>();
	godot::register_tool_class<godot::GDProcInVector>();

	// primitives
	godot::register_tool_class<godot::GDProcEuler>();
	godot::register_tool_class<godot::GDProcVector>();

	// transforms (work on primitives)
	godot::register_tool_class<godot::GDProcBevel>();
	godot::register_tool_class<godot::GDProcGenNormals>();
	godot::register_tool_class<godot::GDProcMult>();
	godot::register_tool_class<godot::GDProcRedist>();
	godot::register_tool_class<godot::GDProcRotate>();
	godot::register_tool_class<godot::GDProcScale>();
	godot::register_tool_class<godot::GDProcTranslate>();

	// shapes
	godot::register_tool_class<godot::GDProcBox>();
	godot::register_tool_class<godot::GDProcCircle>();
	godot::register_tool_class<godot::GDProcRect>();

	// surfaces
	godot::register_tool_class<godot::GDProcExtrudeShape>();
	godot::register_tool_class<godot::GDProcSurface>();

	// modifiers (work on surfaces)
	godot::register_tool_class<godot::GDProcMirror>();
	godot::register_tool_class<godot::GDProcPlaceOnPath>();
	godot::register_tool_class<godot::GDProcTransform>();

	// output
	godot::register_tool_class<godot::GDProcOutput>();
}

