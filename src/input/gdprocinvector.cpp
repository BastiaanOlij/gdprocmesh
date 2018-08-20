#include "input/gdprocinvector.h"

using namespace godot;

void GDProcInVector::_register_methods() {
}

String GDProcInVector::get_type_name() {
	return "Input Vector";
}

String GDProcInVector::get_description() const {
	return "Allows you to specify a single vector as input for the procedural mesh.\n Can be set from the editor.";
}

void GDProcInVector::_init() {
	// first call super class
	GDProcNode::_init();

	// init our buffer
	value.resize(1);
	value.set(0, Vector3(0.0, 0.0, 0.0));
}

void GDProcInVector::set_input(Variant p_input) {
	Vector3 new_vec = p_input;
	if (value[0] != new_vec) {
		value.set(0, new_vec);
		must_update = true;
		emit_signal("changed");
	}
}

Variant GDProcInVector::get_input() {
	return Variant(value[0]);
}

bool GDProcInVector::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	return updated;
}

Variant::Type GDProcInVector::get_input_property_type() const {
	return Variant::VECTOR3;
}

int GDProcInVector::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcInVector::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcInVector::get_output_connector_name(int p_slot) const {
	return "vector";
}

const Variant GDProcInVector::get_output(int p_slot) const {
	return Variant(value);
}