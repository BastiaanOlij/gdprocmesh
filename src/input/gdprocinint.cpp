#include "input/gdprocinint.h"

using namespace godot;

void GDProcInInt::_register_methods() {
}

String GDProcInInt::get_type_name() {
	return "Input Int";
}

void GDProcInInt::_init() {
	// first call super class
	GDProcNode::_init();
}

void GDProcInInt::set_input(Variant p_input) {
	int new_value = p_input;
	if (value != new_value) {
		value = new_value;
		must_update = true;
		emit_signal("changed");
	}
}

Variant GDProcInInt::get_input() const {
	return Variant(value);
}

bool GDProcInInt::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	return updated;
}

Variant::Type GDProcInInt::get_input_property_type() const {
	return Variant::INT;
}

int GDProcInInt::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcInInt::get_output_connector_type(int p_slot) const {
	return Variant::INT;
}

const String GDProcInInt::get_output_connector_name(int p_slot) const {
	return "value";
}

const Variant GDProcInInt::get_output(int p_slot) const {
	return Variant(value);
}