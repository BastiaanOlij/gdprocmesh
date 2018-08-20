#include "transforms/gdprocdiv.h"

using namespace godot;

void GDProcDiv::_register_methods() {
	register_property<GDProcDiv, float>("divider", &GDProcDiv::set_divider, &GDProcDiv::get_divider, 1.0);
}

String GDProcDiv::get_type_name() {
	return "Division";
}

String GDProcDiv::get_description() const {
	return "Divides all reals in input by divider.";
}

void GDProcDiv::_init() {
	// first call super class
	GDProcNode::_init();

	// default values
	values.resize(1);
	values.set(0, 0.0f);
	default_div = 1.0f;
}

void GDProcDiv::set_divider(float p_divider) {
	if (default_div != p_divider) {
		default_div = p_divider;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcDiv::get_divider() {
	return default_div;
}

bool GDProcDiv::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_values = 0;
		PoolRealArray input_values;
		int num_divs = 0;
		PoolRealArray divs;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_REAL_ARRAY) {
				input_values = p_inputs[0];
				num_values = input_values.size();
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY) {
				divs = p_inputs[1];
				num_divs = divs.size();
			}
		}

		if (num_divs == 0) {
			divs.push_back(default_div);
			num_divs++;
		}

		if (num_values > 0) {
			int new_size = num_values > num_divs ? num_values : num_divs;
			values.resize(new_size);

			PoolRealArray::Write w = values.write();
			PoolRealArray::Read r = input_values.read();
			PoolRealArray::Read m = divs.read();

			for (int i = 0; i < new_size; i++) {
				w[i] = r[i % num_values] / m[i % num_divs];
			}

		} else {
			values.resize(0);
		}
	}

	return updated;
}

int GDProcDiv::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcDiv::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_REAL_ARRAY;
	} else {
		return Variant::POOL_REAL_ARRAY;
	}
}

const String GDProcDiv::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "values";
	} else if (p_slot == 1) {
		return "divider";
	}

	return "";
}

const String GDProcDiv::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "divider";
	}

	return "";
}

int GDProcDiv::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcDiv::get_output_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
}

const String GDProcDiv::get_output_connector_name(int p_slot) const {
	return "values";
}

const Variant GDProcDiv::get_output(int p_slot) const {
	return Variant(values);
}

