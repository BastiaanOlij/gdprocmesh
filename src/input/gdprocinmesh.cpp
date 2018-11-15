#include "input/gdprocinmesh.h"

#include "godot-cpp/src/gen/__icalls.hpp"

using namespace godot;

void GDProcInMesh::_register_methods() {
	register_method("_mesh_changed", &GDProcInMesh::_mesh_changed);
}

String GDProcInMesh::get_type_name() {
	return "Input Mesh";
}

String GDProcInMesh::get_description() const {
	return "Allows you to specify a Mesh as input for the procedural mesh.";
}

void GDProcInMesh::_init() {
	// first call super class
	GDProcNode::_init();
}

GDProcInMesh::~GDProcInMesh() {
	if (mesh.is_valid()) {
		mesh->disconnect("changed", this, "_mesh_changed");
	}
}

bool GDProcInMesh::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		printf("Update mesh\n");

		int source_surface_count = 0;

		if (mesh.is_valid()) {
			source_surface_count = mesh->get_surface_count();
		}

		for (int s = 0; s < 5; s++) {
			// resize our array
			surface_arr[s].resize(ArrayMesh::ARRAY_MAX);

			if (s < source_surface_count) {
				Array surface = mesh->surface_get_arrays(s);
				int cnt = surface.size();

				// copy surface
				for (int i = 0; i < ArrayMesh::ARRAY_MAX; i++) {
					if (i < cnt) {
						surface_arr[s][i] = surface[i];
					} else {
						surface_arr[s][i] = Variant();
					}
				}
			} else {
				// clear it out
				for (int i = 0; i < ArrayMesh::ARRAY_MAX; i++) {
					surface_arr[s][i] = Variant();
				}
			}
		}
	}

	return updated;
}

Variant::Type GDProcInMesh::get_input_property_type() const {
	return Variant::OBJECT;
}

String GDProcInMesh::get_input_property_hint() const {
	return "Mesh";
}

void GDProcInMesh::set_input(Variant p_input) {
	if (p_input.get_type() != Variant::OBJECT) {
		printf("Not an object\n");
		return;
	}

	Ref<Mesh> new_mesh = p_input;
	if (new_mesh.is_null()) {
		printf("Not a valid object\n");
		return;
	} else if (!new_mesh->is_class("Mesh")) {
		printf("Not a valid object\n");
		return;
	}

	// disconnect from the old...
	if (mesh.is_valid()) {
		mesh->disconnect("changed", this, "_mesh_changed");
	}

	// and update
	mesh = new_mesh;

	// connect to the new
	if (mesh.is_valid()) {
		mesh->connect("changed", this, "_mesh_changed");
	}

	must_update = true;
	emit_signal("changed");
}

Variant GDProcInMesh::get_input() {
	if (mesh.is_valid()) {
		return Variant(mesh);
	}
	return Variant();
}

void GDProcInMesh::_mesh_changed() {
	must_update = true;
	emit_signal("changed");
}

int GDProcInMesh::get_output_connector_count() const {
	return 5;
}

Variant::Type GDProcInMesh::get_output_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcInMesh::get_output_connector_name(int p_slot) const {
	return "surface";
}

const Variant GDProcInMesh::get_output(int p_slot) const {
	if (surface_arr[p_slot].size() > 0) {
		return Variant(surface_arr[p_slot]);
	} else {
		return Variant();
	}
}