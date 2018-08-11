#include "input/gdprocinreal.h"

using namespace godot;

void GDProcInReal::_register_methods() {
}

String GDProcInReal::get_type_name() {
	return "Input Real";
}

void GDProcInReal::_init() {
	// first call super class
	GDProcNode::_init();
}

void GDProcInReal::set_input(Variant p_input) {
	float new_value = p_input;
	if (value != new_value) {
		value = new_value;
		must_update = true;
		emit_signal("changed");
	}
}

Variant GDProcInReal::get_input() const {
	return Variant(value);
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
	return Variant::REAL;
}

const String GDProcInReal::get_output_connector_name(int p_slot) const {
	return "value";
}

const Variant GDProcInReal::get_output(int p_slot) const {
	return Variant(value);
}