#include "transforms/gdprocmult.h"

using namespace godot;

void GDProcMult::_register_methods() {
	register_property<GDProcMult, float>("mult", &GDProcMult::set_mult, &GDProcMult::get_mult, 1.0);
}

String GDProcMult::get_type_name() {
	return "Multiply";
}

String GDProcMult::get_description() const {
	return "Multiplies all reals in input by mult.";
}

void GDProcMult::_init() {
	// first call super class
	GDProcNode::_init();

	// default values
	values.resize(1);
	values.set(0, 0.0f);
	default_mult = 1.0f;
}

void GDProcMult::set_mult(float p_mult) {
	if (default_mult != p_mult) {
		default_mult = p_mult;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcMult::get_mult() {
	return default_mult;
}

bool GDProcMult::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_values = 0;
		PoolRealArray input_values;
		int num_mults = 0;
		PoolRealArray mults;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_REAL_ARRAY) {
				input_values = p_inputs[0];
				num_values = input_values.size();
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY) {
				mults = p_inputs[1];
				num_mults = mults.size();
			}
		}

		if (num_mults == 0) {
			mults.push_back(default_mult);
			num_mults++;
		}

		if (num_values > 0) {
			int new_size = num_values > num_mults ? num_values : num_mults;
			values.resize(new_size);

			PoolRealArray::Write w = values.write();
			PoolRealArray::Read r = input_values.read();
			PoolRealArray::Read m = mults.read();

			for (int i = 0; i < new_size; i++) {
				w[i] = r[i % num_values] * m[i % num_mults];
			}

		} else {
			values.resize(0);
		}
	}

	return updated;
}

int GDProcMult::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcMult::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_REAL_ARRAY;
	} else {
		return Variant::POOL_REAL_ARRAY;
	}
}

const String GDProcMult::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "values";
	} else if (p_slot == 1) {
		return "multiply";
	}

	return "";
}

const String GDProcMult::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "mult";
	}

	return "";
}

int GDProcMult::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcMult::get_output_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
}

const String GDProcMult::get_output_connector_name(int p_slot) const {
	return "values";
}

const Variant GDProcMult::get_output(int p_slot) const {
	return Variant(values);
}

