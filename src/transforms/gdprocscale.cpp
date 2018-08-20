#include "transforms/gdprocscale.h"

using namespace godot;

void GDProcScale::_register_methods() {
	register_property<GDProcScale, Vector3>("scale", &GDProcScale::set_scale, &GDProcScale::get_scale, Vector3(1.0, 1.0, 1.0));
}

String GDProcScale::get_type_name() {
	return "Scale";
}

String GDProcScale::get_description() const {
	return "Multiplies together vectors from translation and vectors. In other words:\noutput[i] = vectors[i % vectors.size()] * scale[i % scale.size()]";
}

void GDProcScale::_init() {
	// first call super class
	GDProcNode::_init();

	default_scale = Vector3(1.0, 1.0, 1.0);
}

void GDProcScale::set_scale(Vector3 p_scale) {
	if (default_scale != p_scale) {
		default_scale = p_scale;
		must_update = true;
		emit_signal("changed");
	}
}

Vector3 GDProcScale::get_scale() {
	return default_scale;
}

bool GDProcScale::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_scales = 0;
		PoolVector3Array scales;
		int num_vectors = 0;
		PoolVector3Array input_vectors;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				input_vectors = p_inputs[0];
				num_vectors = input_vectors.size();
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				scales = p_inputs[1];
				num_scales = scales.size();
			}
		}

		if (num_scales == 0) {
			scales.push_back(default_scale);
			num_scales++;
		}

		if (num_vectors > 0) {
			int new_size = num_vectors > num_scales ? num_vectors : num_scales;
			vectors.resize(new_size);

			PoolVector3Array::Write w = vectors.write();
			PoolVector3Array::Read r = input_vectors.read();
			PoolVector3Array::Read s = scales.read();

			for (int i = 0; i < new_size; i++) {
				w[i] = r[i % num_vectors] * s[i % num_scales];
			}

		} else {
			vectors.resize(0);
		}
	}

	return updated;
}

int GDProcScale::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcScale::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else {
		return Variant::POOL_VECTOR3_ARRAY;
	}
}

const String GDProcScale::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "vectors";
	} else if (p_slot == 1) {
		return "scale";
	}

	return "";
}

const String GDProcScale::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "scale";
	}

	return "";
}

int GDProcScale::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcScale::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcScale::get_output_connector_name(int p_slot) const {
	return "vectors";
}

const Variant GDProcScale::get_output(int p_slot) const {
	return Variant(vectors);
}

