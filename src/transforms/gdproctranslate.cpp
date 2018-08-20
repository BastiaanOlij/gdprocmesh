#include "transforms/gdproctranslate.h"

using namespace godot;

void GDProcTranslate::_register_methods() {
	register_property<GDProcTranslate, Vector3>("translation", &GDProcTranslate::set_translation, &GDProcTranslate::get_translation, Vector3(0.0, 0.0, 0.0));
}

String GDProcTranslate::get_type_name() {
	return "Translate";
}

String GDProcTranslate::get_description() const {
	return "Adds together vectors from translation and vectors. In other words:\noutput[i] = vectors[i % vectors.size()] + translation[i % translation.size()]";
}

void GDProcTranslate::_init() {
	// first call super class
	GDProcNode::_init();

	default_translation = Vector3(0.0, 0.0, 0.0);
}

void GDProcTranslate::set_translation(Vector3 new_vector) {
	if (default_translation != new_vector) {
		default_translation = new_vector;
		must_update = true;
		emit_signal("changed");
	}
}

Vector3 GDProcTranslate::get_translation() {
	return default_translation;
}

bool GDProcTranslate::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_translations = 0;
		PoolVector3Array translations;
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
				translations = p_inputs[1];
				num_translations = translations.size();
			}
		}

		if (num_translations == 0) {
			translations.push_back(default_translation);
			num_translations++;
		}

		if (num_vectors > 0) {
			int new_size = num_vectors > num_translations ? num_vectors : num_translations;
			vectors.resize(new_size);

			PoolVector3Array::Write w = vectors.write();
			PoolVector3Array::Read r = input_vectors.read();
			PoolVector3Array::Read t = translations.read();

			for (int i = 0; i < new_size; i++) {
				w[i] = r[i % num_vectors] + t[i % num_translations];
			}

		} else {
			vectors.resize(0);
		}
	}

	return updated;
}

int GDProcTranslate::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcTranslate::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else {
		return Variant::POOL_VECTOR3_ARRAY;
	}
}

const String GDProcTranslate::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "vectors";
	} else if (p_slot == 1) {
		return "translation";
	}

	return "";
}

const String GDProcTranslate::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "translation";
	}

	return "";
}

int GDProcTranslate::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcTranslate::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcTranslate::get_output_connector_name(int p_slot) const {
	return "vectors";
}

const Variant GDProcTranslate::get_output(int p_slot) const {
	return Variant(vectors);
}

