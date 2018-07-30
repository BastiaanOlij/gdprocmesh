#include "gdprocnode.h"

using namespace godot;

void GDProcNode::_register_methods() {
	// position
	register_method("get_position", &GDProcNode::get_position);
	register_method("set_position", &GDProcNode::set_position);
	register_property<GDProcNode, Vector2>("size", &GDProcNode::set_position, &GDProcNode::get_position, Vector2());

	// connectors
	register_method("get_input_connector_count", &GDProcNode::get_input_connector_count);
	register_method("get_input_connector_type", &GDProcNode::get_input_connector_type);
	register_method("get_input_connector_name", &GDProcNode::get_input_connector_name);

	register_method("get_output_connector_count", &GDProcNode::get_output_connector_count);
	register_method("get_output_connector_type", &GDProcNode::get_output_connector_type);
	register_method("get_output_connector_name", &GDProcNode::get_output_connector_name);

	// signal that our procmesh can subscribe to so we know things need to be regenerated
	register_signal<GDProcNode>((char *)"changed");
}

GDProcNode::ProcessStatus GDProcNode::get_status() const {
	return status;
}

void GDProcNode::set_status(GDProcNode::ProcessStatus p_status) {
	status = p_status;
}

void GDProcNode::_init() {
	printf("GDProcNode::_init\n");
	must_update = true;
	position = Vector2(0.0, 0.0);
}

bool GDProcNode::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update;
	must_update = false;

	// nothing implemented here so just return false. This should become a template function
	return updated;
}

int GDProcNode::get_input_connector_count() const {
	return 0;
}

Variant::Type GDProcNode::get_input_connector_type(int p_idx) const {
	return Variant::NIL;
}

const String GDProcNode::get_input_connector_name(int p_idx) const {
	return "";
}

int GDProcNode::get_output_connector_count() const {
	// we should always have one output unless this is our final node
	return 1;
}

Variant::Type GDProcNode::get_output_connector_type(int p_idx) const {
	return Variant::NIL;
}

const String GDProcNode::get_output_connector_name(int p_idx) const {
	return "Default";
}

const Variant GDProcNode::get_output(int p_idx) const {
	return Variant();
}

Vector2 GDProcNode::get_position() const {
	return position;
}

void GDProcNode::set_position(Vector2 p_pos) {
	if (position != p_pos) {
		position = p_pos;

		// probably should send signal
	}
}

