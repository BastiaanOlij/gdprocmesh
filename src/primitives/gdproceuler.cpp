#include "primitives/gdproceuler.h"

using namespace godot;

void GDProcEuler::_register_methods() {
	register_property<GDProcEuler, float>("x", &GDProcEuler::set_x, &GDProcEuler::get_x, 0.0);
	register_property<GDProcEuler, float>("y", &GDProcEuler::set_y, &GDProcEuler::get_y, 0.0);
	register_property<GDProcEuler, float>("z", &GDProcEuler::set_z, &GDProcEuler::get_z, 0.0);
}

String GDProcEuler::get_type_name() {
	return "Euler angles";
}

void GDProcEuler::_init() {
	// first call super class
	GDProcNode::_init();

	// init our buffer
	value.resize(1);
	value.set(0, Vector3(0.0, 0.0, 0.0));
}

void GDProcEuler::set_x(float x) {
	if (defaults.x != x) {
		defaults.x = x;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcEuler::get_x() const {
	return defaults.x;
}

void GDProcEuler::set_y(float y) {
	if (defaults.y != y) {
		defaults.y = y;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcEuler::get_y() const {
	return defaults.y;
}

void GDProcEuler::set_z(float z) {
	if (defaults.z != z) {
		defaults.z = z;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcEuler::get_z() const {
	return defaults.z;
}

bool GDProcEuler::update(bool p_inputs_updated, const Array &p_inputs) {
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
			float pi_180 = 3.14159265359f / 180.0f;
			Quat q;
			q.set_euler_xyz(Vector3(rx[i % num_x], ry[i % num_y], rz[i % num_z]) * Vector3(pi_180, pi_180, pi_180));

			vw[i] = Vector3(q.x, q.y, q.z); // quaternions should be normalized so w should be sqrt(1.0 - x2 - y2 - z2)
		}
	}

	return updated;
}

int GDProcEuler::get_input_connector_count() const {
	return 3;
}

Variant::Type GDProcEuler::get_input_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
}

const String GDProcEuler::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "x";
	} else if (p_slot == 1) {
		return "y";
	} else if (p_slot == 2) {
		return "z";
	}

	return "";
}

const String GDProcEuler::get_connector_property_name(int p_slot) const {
	if (p_slot == 0) {
		return "x";
	} else if (p_slot == 1) {
		return "y";
	} else if (p_slot == 2) {
		return "z";
	}

	return "";
}

int GDProcEuler::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcEuler::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcEuler::get_output_connector_name(int p_slot) const {
	return "vector";
}

const Variant GDProcEuler::get_output(int p_slot) const {
	return Variant(value);
}