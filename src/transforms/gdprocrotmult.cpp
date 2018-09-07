#include "transforms/gdprocrotmult.h"

using namespace godot;

void GDProcRotMult::_register_methods() {
}

String GDProcRotMult::get_type_name() {
	return "Multiply rotation";
}

String GDProcRotMult::get_description() const {
	return "Multiplies/combined two rotations.";
}

void GDProcRotMult::_init() {
	// first call super class
	GDProcNode::_init();

	// default values
}

bool GDProcRotMult::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_values_a = 0;
		PoolColorArray input_values_a;
		int num_values_b = 0;
		PoolColorArray input_values_b;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_COLOR_ARRAY) {
				input_values_a = p_inputs[0];
				num_values_a = input_values_a.size();
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_COLOR_ARRAY) {
				input_values_b = p_inputs[1];
				num_values_b = input_values_b.size();
			}
		}

		if (num_values_a == 0) {
			input_values_a.push_back(Color(0.0, 0.0, 0.0, 1.0));
			num_values_a++;
		}
		if (num_values_b == 0) {
			input_values_b.push_back(Color(0.0, 0.0, 0.0, 1.0));
			num_values_b++;
		}

		{
			int new_size = num_values_a > num_values_b ? num_values_a : num_values_b;
			values.resize(new_size);

			PoolColorArray::Write w = values.write();
			PoolColorArray::Read a = input_values_a.read();
			PoolColorArray::Read b = input_values_b.read();

			for (int i = 0; i < new_size; i++) {
				Color r = a[i % num_values_a];
				Quat qa(r.r, r.g, r.b, r.a);

				r = b[i % num_values_b];
				Quat qb(r.r, r.g, r.b, r.a);

				//printf("%0.2f, %0.2f, %0.2f, %0.2f * %0.2f, %0.2f, %0.2f, %0.2f\n", qa.x, qa.y, qa.z, qa.w, qb.x, qb.y, qb.z, qb.w);

				// having weird issues with quarternion multiply...
				// qa = qa * qb;
				// qa.normalize();

				// so we take the long way around for now...
				Basis ba = qa;
				Basis bb = qb;
				ba *= bb;
				qa = ba;

				w[i] = Color(qa.x, qa.y, qa.z, qa.w);
			}
		}
	}

	return updated;
}

int GDProcRotMult::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcRotMult::get_input_connector_type(int p_slot) const {
	// we don't have a POOL_QUAD_ARRAY, abusing color for storing 
	if (p_slot == 0) {
		return Variant::POOL_COLOR_ARRAY;
	} else {
		return Variant::POOL_COLOR_ARRAY;
	}
}

const String GDProcRotMult::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "a";
	} else if (p_slot == 1) {
		return "b";
	}

	return "";
}

int GDProcRotMult::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcRotMult::get_output_connector_type(int p_slot) const {
	// we don't have a POOL_QUAD_ARRAY, abusing color for storing 
	return Variant::POOL_COLOR_ARRAY;
}

const String GDProcRotMult::get_output_connector_name(int p_slot) const {
	return "values";
}

const Variant GDProcRotMult::get_output(int p_slot) const {
	return Variant(values);
}

