#include "input/gdprocinpoolvec2.h"

using namespace godot;

void GDProcInPoolVec2::_register_methods() {
}

String GDProcInPoolVec2::get_type_name() {
	return "Input Vec2s";
}

void GDProcInPoolVec2::_init() {
	// first call super class
	GDProcNode::_init();

	// add some defaults
	value.push_back(Vector2(-0.5, -0.5));
	value.push_back(Vector2( 0.5, -0.5));
	value.push_back(Vector2( 0.5,  0.5));
	value.push_back(Vector2(-0.5,  0.5));
}

void GDProcInPoolVec2::set_input(Variant p_input) {
	// assume changed, maybe do something at some point to check if it actually has..
	value = p_input;
	must_update = true;
	emit_signal("changed");
}

Variant GDProcInPoolVec2::get_input() const {
	return Variant(value);
}

bool GDProcInPoolVec2::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	return updated;
}

Variant::Type GDProcInPoolVec2::get_input_property_type() const {
	return Variant::POOL_VECTOR2_ARRAY;
}

int GDProcInPoolVec2::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcInPoolVec2::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR2_ARRAY;
}

const String GDProcInPoolVec2::get_output_connector_name(int p_slot) const {
	return "vec2s";
}

const Variant GDProcInPoolVec2::get_output(int p_slot) const {
	return Variant(value);
}