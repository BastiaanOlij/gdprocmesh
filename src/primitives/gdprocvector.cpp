#include "primitives/gdprocvector.h"

using namespace godot;

void GDProcVector::_register_methods() {
	register_property<GDProcVector, float>("x", &GDProcVector::set_x, &GDProcVector::get_x, 0.0);
	register_property<GDProcVector, float>("y", &GDProcVector::set_y, &GDProcVector::get_y, 0.0);
	register_property<GDProcVector, float>("z", &GDProcVector::set_z, &GDProcVector::get_z, 0.0);
}

String GDProcVector::get_type_name() {
	return "Vector Combine";
}

String GDProcVector::get_description() const {
	return "Helper node to combine 3 reals into one vec3.";
}

void GDProcVector::_init() {
	// first call super class
	GDProcNode::_init();

	// init our buffer
	value.resize(1);
	value.set(0, Vector3(0.0, 0.0, 0.0));
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
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		PoolRealArray x;
		PoolRealArray y;
		PoolRealArray z;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_REAL_ARRAY) {
				x = p_inputs[0];
			}
		}

		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY) {
				y = p_inputs[1];
			}
		}

		if (input_count > 2) {
			if (p_inputs[2].get_type() == Variant::POOL_REAL_ARRAY) {
				z = p_inputs[2];
			}
		}

		// check our input and add defaults where we need them
		int num_x = x.size();
		if (num_x == 0) {
			x.push_back(defaults.x);
			num_x++;
		}

		int num_y = y.size();
		if (num_y == 0) {
			y.push_back(defaults.y);
			num_y++;
		}

		int num_z = z.size();
		if (num_z == 0) {
			z.push_back(defaults.z);
			num_z++;
		}

		// how many vertices will we output?
		int max = num_x > num_y ? num_x : num_y;
		if (max < num_z) {
			max = num_z;
		}

		// and lets process...
		PoolRealArray::Read rx = x.read();
		PoolRealArray::Read ry = y.read();
		PoolRealArray::Read rz = z.read();

		value.resize(max);
		PoolVector3Array::Write vw = value.write();

		for (int i = 0; i < max; i++) {
			vw[i] = Vector3(rx[i % num_x], ry[i % num_y], rz[i % num_z]);
		}
	}

	return updated;
}

int GDProcVector::get_input_connector_count() const {
	return 3;
}

Variant::Type GDProcVector::get_input_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
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
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcVector::get_output_connector_name(int p_slot) const {
	return "vector";
}

const Variant GDProcVector::get_output(int p_slot) const {
	return Variant(value);
}