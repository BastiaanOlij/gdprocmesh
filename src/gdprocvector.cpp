#include "gdprocvector.h"

using namespace godot;

void GDProcVector::_register_methods() {
	register_property<GDProcVector, float>("x", &GDProcVector::set_x, &GDProcVector::get_x, 0.0);
	register_property<GDProcVector, float>("y", &GDProcVector::set_y, &GDProcVector::get_y, 0.0);
	register_property<GDProcVector, float>("z", &GDProcVector::set_z, &GDProcVector::get_z, 0.0);
}

String GDProcVector::get_type_name() {
	return "Vector";
}

void GDProcVector::_init() {
	// first call super class
	GDProcNode::_init();
}

void GDProcVector::set_x(float x) {
	if (defaults.x != x) {
		defaults.x = x;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcVector::get_x() const {
	return defaults.x;
}

void GDProcVector::set_y(float y) {
	if (defaults.y != y) {
		defaults.y = y;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcVector::get_y() const {
	return defaults.y;
}

void GDProcVector::set_z(float z) {
	if (defaults.z != z) {
		defaults.z = z;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcVector::get_z() const {
	return defaults.z;
}

bool GDProcVector::update(bool p_inputs_updated, const Array &p_inputs) {
	Vector3 new_vector = defaults;

	if (must_update || p_inputs_updated) {
		printf("Updating vector\n");

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
		printf("Vector was changed to %0.2f, %0.2f, %0.2f\n", vector.x, vector.y, vector.z);
		return true;
	}
}

int GDProcVector::get_input_connector_count() const {
	return 3;
}

Variant::Type GDProcVector::get_input_connector_type(int p_slot) const {
	return Variant::REAL;
}

const String GDProcVector::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "x";
	} else if (p_slot == 1) {
		return "y";
	} else if (p_slot == 2) {
		return "z";
	}

	return "";
}

const String GDProcVector::get_connector_property_name(int p_slot) const {
	if (p_slot == 0) {
		return "x";
	} else if (p_slot == 1) {
		return "y";
	} else if (p_slot == 2) {
		return "z";
	}

	return "";
}

int GDProcVector::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcVector::get_output_connector_type(int p_slot) const {
	return Variant::VECTOR3;
}

const String GDProcVector::get_output_connector_name(int p_slot) const {
	return "vector";
}

const Variant GDProcVector::get_output(int p_slot) const {
	return Variant(vector);
}