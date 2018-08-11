#include "input/gdprocinpoolvec3.h"

using namespace godot;

void GDProcInPoolVec3::_register_methods() {
}

String GDProcInPoolVec3::get_type_name() {
	return "Input Vec3s";
}

void GDProcInPoolVec3::_init() {
	// first call super class
	GDProcNode::_init();

	// add some defaults
	value.push_back(Vector3(0.0, 0.0, 0.0));
	value.push_back(Vector3(0.0, 0.0, 1.0));
}

void GDProcInPoolVec3::set_input(Variant p_input) {
	// assume changed, maybe do something at some point to check if it actually has..
	value = p_input;
	must_update = true;
	emit_signal("changed");
}

Variant GDProcInPoolVec3::get_input() const {
	return Variant(value);
}

bool GDProcInPoolVec3::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	return updated;
}

Variant::Type GDProcInPoolVec3::get_input_property_type() const {
	return Variant::POOL_VECTOR3_ARRAY;
}

int GDProcInPoolVec3::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcInPoolVec3::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcInPoolVec3::get_output_connector_name(int p_slot) const {
	return "vec3s";
}

const Variant GDProcInPoolVec3::get_output(int p_slot) const {
	return Variant(value);
}