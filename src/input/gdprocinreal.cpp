#include "input/gdprocinreal.h"

using namespace godot;

void GDProcInReal::_register_methods() {
}

String GDProcInReal::get_type_name() {
	return "Input Real";
}

String GDProcInReal::get_description() const {
	return "Allows you to specify a real as input for the procedural mesh.\nCan be set from the editor.";
}

void GDProcInReal::_init() {
	// first call super class
	GDProcNode::_init();

	// init our buffer
	value.resize(1);
	value.set(0, 0.0f);
}

void GDProcInReal::set_input(Variant p_input) {
	float new_value = p_input;
	if (value[0] != new_value) {
		value.set(0, new_value);
		must_update = true;
		emit_signal("changed");
	}
}

Variant GDProcInReal::get_input() {
	return Variant(value[0]);
}

bool GDProcInReal::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	return updated;
}

Variant::Type GDProcInReal::get_input_property_type() const {
	return Variant::REAL;
}

int GDProcInReal::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcInReal::get_output_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
}

const String GDProcInReal::get_output_connector_name(int p_slot) const {
	return "value";
}

const Variant GDProcInReal::get_output(int p_slot) const {
	return Variant(value);
}