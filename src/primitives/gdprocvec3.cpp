#include "primitives/gdprocvec3.h"

using namespace godot;

void GDProcVec3::_register_methods() {
	register_property<GDProcVec3, float>("x", &GDProcVec3::set_x, &GDProcVec3::get_x, 0.0);
	register_property<GDProcVec3, float>("y", &GDProcVec3::set_y, &GDProcVec3::get_y, 0.0);
	register_property<GDProcVec3, float>("z", &GDProcVec3::set_z, &GDProcVec3::get_z, 0.0);
}

String GDProcVec3::get_type_name() {
	return "Vec3";
}

void GDProcVec3::_init() {
	// first call super class
	GDProcNode::_init();
}

void GDProcVec3::set_x(float x) {
	if (defaults.x != x) {
		defaults.x = x;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcVec3::get_x() const {
	return defaults.x;
}

void GDProcVec3::set_y(float y) {
	if (defaults.y != y) {
		defaults.y = y;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcVec3::get_y() const {
	return defaults.y;
}

void GDProcVec3::set_z(float z) {
	if (defaults.z != z) {
		defaults.z = z;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcVec3::get_z() const {
	return defaults.z;
}

bool GDProcVec3::update(bool p_inputs_updated, const Array &p_inputs) {
	Vector3 new_vector = defaults;

	if (must_update || p_inputs_updated) {
		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::REAL) {
				new_vector.x = p_inputs[0];
			}
		}

		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::REAL) {
				new_vector.y = p_inputs[1];
			}
		}

		if (input_count > 2) {
			if (p_inputs[2].get_type() == Variant::REAL) {
				new_vector.z = p_inputs[2];
			}
		}
	}

	must_update = false;

	if (vector == new_vector) {
		// printf("Vector remains %0.2f, %0.2f, %0.2f\n", vector.x, vector.y, vector.z);
		return false;
	} else {
		vector = new_vector;
		// printf("Vector was changed to %0.2f, %0.2f, %0.2f\n", vector.x, vector.y, vector.z);
		return true;
	}
}

int GDProcVec3::get_input_connector_count() const {
	return 3;
}

Variant::Type GDProcVec3::get_input_connector_type(int p_slot) const {
	return Variant::REAL;
}

const String GDProcVec3::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "x";
	} else if (p_slot == 1) {
		return "y";
	} else if (p_slot == 2) {
		return "z";
	}

	return "";
}

const String GDProcVec3::get_connector_property_name(int p_slot) const {
	if (p_slot == 0) {
		return "x";
	} else if (p_slot == 1) {
		return "y";
	} else if (p_slot == 2) {
		return "z";
	}

	return "";
}

int GDProcVec3::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcVec3::get_output_connector_type(int p_slot) const {
	// maybe make this return a pool vector with just one entry?
	return Variant::VECTOR3;
}

const String GDProcVec3::get_output_connector_name(int p_slot) const {
	return "vec3";
}

const Variant GDProcVec3::get_output(int p_slot) const {
	// maybe make this return a pool vector with just one entry?
	return Variant(vector);
}