#include "gdprocsurface.h"

using namespace godot;

void GDProcSurface::_register_methods() {

}

String GDProcSurface::get_type_name() {
	return String("Surface");
}

void GDProcSurface::_init() {
	// first call super class
	GDProcNode::_init();

	// nothing to do yet for ourselves.
}

bool GDProcSurface::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update;
	must_update = false;

	if (updated || p_inputs_updated) {
		printf("Updating our surface\n");

		// resize our array just in case
		surface_arr.resize(ArrayMesh::ARRAY_MAX);

		int input_count = p_inputs.size();
		if (input_count > ArrayMesh::ARRAY_VERTEX) {
			if (p_inputs[ArrayMesh::ARRAY_VERTEX].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_VERTEX] = p_inputs[ArrayMesh::ARRAY_VERTEX];
			}
		}

		if (input_count > ArrayMesh::ARRAY_NORMAL) {
			if (p_inputs[ArrayMesh::ARRAY_NORMAL].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_NORMAL] = p_inputs[ArrayMesh::ARRAY_NORMAL];
			}
		}

		if (input_count > ArrayMesh::ARRAY_TANGENT) {
			if (p_inputs[ArrayMesh::ARRAY_TANGENT].get_type() == Variant::POOL_REAL_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_TANGENT] = p_inputs[ArrayMesh::ARRAY_TANGENT];
			}
		}

		if (input_count > ArrayMesh::ARRAY_COLOR) {
			if (p_inputs[ArrayMesh::ARRAY_COLOR].get_type() == Variant::POOL_COLOR_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_COLOR] = p_inputs[ArrayMesh::ARRAY_COLOR];
			}
		}

		if (input_count > ArrayMesh::ARRAY_TEX_UV) {
			if (p_inputs[ArrayMesh::ARRAY_TEX_UV].get_type() == Variant::POOL_VECTOR2_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_TEX_UV] = p_inputs[ArrayMesh::ARRAY_TEX_UV];
			}
		}

		if (input_count > ArrayMesh::ARRAY_TEX_UV2) {
			if (p_inputs[ArrayMesh::ARRAY_TEX_UV2].get_type() == Variant::POOL_VECTOR2_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_TEX_UV2] = p_inputs[ArrayMesh::ARRAY_TEX_UV2];
			}
		}

		if (input_count > ArrayMesh::ARRAY_BONES) {
			if (p_inputs[ArrayMesh::ARRAY_BONES].get_type() == Variant::POOL_REAL_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_BONES] = p_inputs[ArrayMesh::ARRAY_BONES];
			}
		}

		if (input_count > ArrayMesh::ARRAY_WEIGHTS) {
			if (p_inputs[ArrayMesh::ARRAY_WEIGHTS].get_type() == Variant::POOL_REAL_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_WEIGHTS] = p_inputs[ArrayMesh::ARRAY_WEIGHTS];
			}
		}

		if (input_count > ArrayMesh::ARRAY_INDEX) {
			if (p_inputs[ArrayMesh::ARRAY_INDEX].get_type() == Variant::POOL_INT_ARRAY) {
				surface_arr[ArrayMesh::ARRAY_INDEX] = p_inputs[ArrayMesh::ARRAY_INDEX];
			}
		}
	}

	return updated;
}

int GDProcSurface::get_input_connector_count() const {
	return ArrayMesh::ARRAY_MAX;
}

Variant::Type GDProcSurface::get_input_connector_type(int p_slot) const {
	switch (p_slot) {
		case ArrayMesh::ARRAY_VERTEX:
			return Variant::POOL_VECTOR3_ARRAY;
		case ArrayMesh::ARRAY_NORMAL:
			return Variant::POOL_VECTOR3_ARRAY;
		case ArrayMesh::ARRAY_TANGENT:
			return Variant::POOL_REAL_ARRAY;
		case ArrayMesh::ARRAY_COLOR:
			return Variant::POOL_COLOR_ARRAY;
		case ArrayMesh::ARRAY_TEX_UV:
			return Variant::POOL_VECTOR2_ARRAY;
		case ArrayMesh::ARRAY_TEX_UV2:
			return Variant::POOL_VECTOR2_ARRAY;
		case ArrayMesh::ARRAY_BONES:
			return Variant::POOL_REAL_ARRAY;
		case ArrayMesh::ARRAY_WEIGHTS:
			return Variant::POOL_REAL_ARRAY;
		case ArrayMesh::ARRAY_INDEX:
			return Variant::POOL_INT_ARRAY;
		default:
			return Variant::NIL;
	}
}

const String GDProcSurface::get_input_connector_name(int p_slot) const {
	switch (p_slot) {
		case ArrayMesh::ARRAY_VERTEX:
			return "Vertices";
		case ArrayMesh::ARRAY_NORMAL:
			return "Normals";
		case ArrayMesh::ARRAY_TANGENT:
			return "Tangents";
		case ArrayMesh::ARRAY_COLOR:
			return "Colors";
		case ArrayMesh::ARRAY_TEX_UV:
			return "UVs";
		case ArrayMesh::ARRAY_TEX_UV2:
			return "UV2s";
		case ArrayMesh::ARRAY_BONES:
			return "Bones";
		case ArrayMesh::ARRAY_WEIGHTS:
			return "Weights";
		case ArrayMesh::ARRAY_INDEX:
			return "Indices";
		default:
			return "???";
	}

}

int GDProcSurface::get_output_connector_count() const {
	// We set this to zero even though we output our final array, we don't want this connector to show
	// and it means this will be treated as a final output node
	return 0;
}

const Variant GDProcSurface::get_output(int p_slot) const {
	// this should only be called after update is called. 
	if (p_slot == 0) {
		return Variant(surface_arr);
	}

	return Variant();
}
