#include "input/gdprocinpoolvectors.h"

using namespace godot;

void GDProcInPoolVectors::_register_methods() {
}

String GDProcInPoolVectors::get_type_name() {
	return "Input Vectors";
}

void GDProcInPoolVectors::_init() {
	// first call super class
	GDProcNode::_init();

	// add some defaults
	value.push_back(Vector3(0.0, 0.0, 0.0));
	value.push_back(Vector3(0.0, 0.0, 1.0));
}

void GDProcInPoolVectors::set_input(Variant p_input) {
	// assume changed, maybe do something at some point to check if it actually has..
	value = p_input;
	must_update = true;
	emit_signal("changed");
}

Variant GDProcInPoolVectors::get_input() {
	return Variant(value);
}

bool GDProcInPoolVectors::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	return updated;
}

Variant::Type GDProcInPoolVectors::get_input_property_type() const {
	return Variant::POOL_VECTOR3_ARRAY;
}

int GDProcInPoolVectors::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcInPoolVectors::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcInPoolVectors::get_output_connector_name(int p_slot) const {
	return "vectors";
}

const Variant GDProcInPoolVectors::get_output(int p_slot) const {
	return Variant(value);
}