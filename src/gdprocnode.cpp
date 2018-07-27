#include "gdprocnode.h"

using namespace godot;

void GDProcNode::_register_methods() {
	// position
	register_method("get_position", &GDProcNode::get_position);
	register_method("set_position", &GDProcNode::set_position);
	register_property<GDProcNode, Vector2>("size", &GDProcNode::set_position, &GDProcNode::get_position, Vector2());
}

void GDProcNode::_init() {
	position = Vector2(0.0, 0.0);
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
