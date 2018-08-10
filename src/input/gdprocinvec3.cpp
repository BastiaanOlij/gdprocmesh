#include "input/gdprocinvec3.h"

using namespace godot;

void GDProcInVec3::_register_methods() {
}

String GDProcInVec3::get_type_name() {
	return "Input Vec3";
}

void GDProcInVec3::_init() {
	// first call super class
	GDProcNode::_init();
}

void GDProcInVec3::set_input(Variant p_input) {
	Vector3 new_vec = p_input;
	if (vector != new_vec) {
		vector = new_vec;
		must_update = true;
		emit_signal("changed");
	}
}

Variant GDProcInVec3::get_input() const {
	return Variant(vector);
}

bool GDProcInVec3::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	return updated;
}

Variant::Type GDProcInVec3::get_input_property_type() const {
	return Variant::VECTOR3;
}

int GDProcInVec3::get_output_connector_count() const {
	return 3;
}

Variant::Type GDProcInVec3::get_output_connector_type(int p_slot) const {
	return Variant::REAL;
}

const String GDProcInVec3::get_output_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "x";
	} else if (p_slot == 1) {
		return "y";
	} else if (p_slot == 2) {
		return "z";
	}

	return "";
}

const Variant GDProcInVec3::get_output(int p_slot) const {
	if (p_slot == 0) {
		return Variant(vector.x);
	} else if (p_slot == 1) {
		return Variant(vector.y);
	} else if (p_slot == 2) {
		return Variant(vector.z);
	}

	return Variant();
}