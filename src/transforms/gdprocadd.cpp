#include "transforms/gdprocadd.h"

using namespace godot;

void GDProcAdd::_register_methods() {
	register_property<GDProcAdd, float>("add", &GDProcAdd::set_add, &GDProcAdd::get_add, 0.0);
}

String GDProcAdd::get_type_name() {
	return "Add";
}

String GDProcAdd::get_description() const {
	return "Adds all reals in input by add.";
}

void GDProcAdd::_init() {
	// first call super class
	GDProcNode::_init();

	// default values
	values.resize(1);
	values.set(0, 0.0f);
	default_add = 1.0f;
}

void GDProcAdd::set_add(float p_add) {
	if (default_add != p_add) {
		default_add = p_add;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcAdd::get_add() {
	return default_add;
}

bool GDProcAdd::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_values = 0;
		PoolRealArray input_values;
		int num_adds = 0;
		PoolRealArray adds;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_REAL_ARRAY) {
				input_values = p_inputs[0];
				num_values = input_values.size();
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY) {
				adds = p_inputs[1];
				num_adds = adds.size();
			}
		}

		if (num_adds == 0) {
			adds.push_back(default_add);
			num_adds++;
		}

		if (num_values > 0) {
			int new_size = num_values > num_adds ? num_values : num_adds;
			values.resize(new_size);

			PoolRealArray::Write w = values.write();
			PoolRealArray::Read r = input_values.read();
			PoolRealArray::Read a = adds.read();

			for (int i = 0; i < new_size; i++) {
				w[i] = r[i % num_values] * a[i % num_adds];
			}

		} else {
			values.resize(0);
		}
	}

	return updated;
}

int GDProcAdd::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcAdd::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_REAL_ARRAY;
	} else {
		return Variant::POOL_REAL_ARRAY;
	}
}

const String GDProcAdd::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "values";
	} else if (p_slot == 1) {
		return "add";
	}

	return "";
}

const String GDProcAdd::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "add";
	}

	return "";
}

int GDProcAdd::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcAdd::get_output_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
}

const String GDProcAdd::get_output_connector_name(int p_slot) const {
	return "values";
}

const Variant GDProcAdd::get_output(int p_slot) const {
	return Variant(values);
}

