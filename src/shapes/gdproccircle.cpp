#include "shapes/gdproccircle.h"

using namespace godot;

void GDProcCircle::_register_methods() {
	register_property<GDProcCircle, float>("radius", &GDProcCircle::set_radius, &GDProcCircle::get_radius, 1.0);
	register_property<GDProcCircle, int>("segments", &GDProcCircle::set_segments, &GDProcCircle::get_segments, 16);
}

String GDProcCircle::get_type_name() {
	return String("Circle");
}

void GDProcCircle::_init() {
	// first call super class
	GDProcNode::_init();

	// default size
	default_radius = 1.0;
	default_segments = 16;
}

void GDProcCircle::set_radius(float p_radius) {
	if (default_radius != p_radius) {
		default_radius = p_radius;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcCircle::get_radius() const {
	return default_radius;
}

void GDProcCircle::set_segments(int p_segments) {
	if (default_segments != p_segments) {
		default_segments = p_segments;
		must_update = true;
		emit_signal("changed");
	}
}

int GDProcCircle::get_segments() const {
	return default_segments;
}

bool GDProcCircle::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		float radius = default_radius;
		int segments = default_segments;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::REAL) {
				radius = p_inputs[0];
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::INT) {
				segments = p_inputs[1];
			}
		}

		// prepare our data
		vertices.resize(segments);
		{
			PoolVector2Array::Write wv = vertices.write();
			float two_pi = 2.0f * 3.14159265359f;
			float step = two_pi / segments;
			float angle = 0.0;

			for (int s = 0; s < segments; s++) {
				wv[s].x = radius * sin(angle);
				wv[s].y = -radius * cos(angle);

				angle += step;
			}
		}
	}

	return updated;
}

int GDProcCircle::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcCircle::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::REAL;
	} else if (p_slot == 1) {
		return Variant::INT;
	}

	return Variant::NIL;
}

const String GDProcCircle::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "radius";
	} else if (p_slot == 1) {
		return "segments";
	}

	return "";
}

const String GDProcCircle::get_connector_property_name(int p_slot) const {
	if (p_slot == 0) {
		return "radius";
	} else if (p_slot == 1) {
		return "segments";
	}

	return "";
}

int GDProcCircle::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcCircle::get_output_connector_type(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant::POOL_VECTOR2_ARRAY;
		default:
			return Variant::NIL;		
	}
}

const String GDProcCircle::get_output_connector_name(int p_slot) const {
	switch (p_slot) {
		case 0:
			return "vec2s";
		default:
			return "???";
	}
}

const Variant GDProcCircle::get_output(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant(vertices);
		default:
			return Variant();
	}
}
