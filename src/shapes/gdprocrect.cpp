#include "shapes/gdprocrect.h"

using namespace godot;

void GDProcRect::_register_methods() {
	register_property<GDProcRect, float>("width", &GDProcRect::set_width, &GDProcRect::get_width, 1.0);
	register_property<GDProcRect, float>("height", &GDProcRect::set_height, &GDProcRect::get_height, 1.0);
}

String GDProcRect::get_type_name() {
	return String("Rectangle");
}

void GDProcRect::_init() {
	// first call super class
	GDProcNode::_init();

	// default size
	default_size = Vector2(1.0, 1.0);
}

void GDProcRect::set_width(float p_width) {
	if (default_size.x != p_width) {
		default_size.x = p_width;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcRect::get_width() const {
	return default_size.x;
}

void GDProcRect::set_height(float p_height) {
	if (default_size.y != p_height) {
		default_size.y = p_height;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcRect::get_height() const {
	return default_size.y;
}

bool GDProcRect::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		Vector2 size = default_size;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::REAL) {
				size.x = p_inputs[0];
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::REAL) {
				size.y = p_inputs[1];
			}
		}

		// prepare our data
		vertices.resize(4);
		{
			PoolVector2Array::Write wv = vertices.write();
			float hsx = size.x / 2.0f;
			float hsy = size.y / 2.0f;

			wv[0].x = -hsx; wv[0].y = -hsy;
			wv[1].x =  hsx; wv[1].y = -hsy;
			wv[2].x =  hsx; wv[2].y =  hsy;
			wv[3].x = -hsx; wv[3].y =  hsy;
		}
	}

	return updated;
}

int GDProcRect::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcRect::get_input_connector_type(int p_slot) const {
	return Variant::REAL;
}

const String GDProcRect::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "width";
	} else if (p_slot == 1) {
		return "height";
	}

	return "";
}

const String GDProcRect::get_connector_property_name(int p_slot) const {
	if (p_slot == 0) {
		return "width";
	} else if (p_slot == 1) {
		return "height";
	}

	return "";
}

int GDProcRect::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcRect::get_output_connector_type(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant::POOL_VECTOR2_ARRAY;
		default:
			return Variant::NIL;		
	}
}

const String GDProcRect::get_output_connector_name(int p_slot) const {
	switch (p_slot) {
		case 0:
			return "vec2s";
		default:
			return "???";
	}
}

const Variant GDProcRect::get_output(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant(vertices);
		default:
			return Variant();
	}
}
