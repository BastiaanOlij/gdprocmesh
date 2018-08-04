#include "gdproctranslate.h"

using namespace godot;

void GDProcTranslate::_register_methods() {
	register_property<GDProcTranslate, float>("x", &GDProcTranslate::set_x, &GDProcTranslate::get_x, 0.0);
	register_property<GDProcTranslate, float>("y", &GDProcTranslate::set_y, &GDProcTranslate::get_y, 0.0);
	register_property<GDProcTranslate, float>("z", &GDProcTranslate::set_z, &GDProcTranslate::get_z, 0.0);
}

String GDProcTranslate::get_type_name() {
	return "Translate";
}

void GDProcTranslate::_init() {
	// first call super class
	GDProcNode::_init();
}

void GDProcTranslate::set_x(float x) {
	if (default_translation.x != x) {
		default_translation.x = x;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcTranslate::get_x() const {
	return default_translation.x;	
}

void GDProcTranslate::set_y(float y) {
	if (default_translation.y != y) {
		default_translation.y = y;
		must_update = true;
		emit_signal("changed");
	}	
}

float GDProcTranslate::get_y() const {
	return default_translation.y;	
}

void GDProcTranslate::set_z(float z) {
	if (default_translation.z != z) {
		default_translation.z = z;
		must_update = true;
		emit_signal("changed");
	}	
}

float GDProcTranslate::get_z() const {
	return default_translation.z;
}

bool GDProcTranslate::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update;
	must_update = false;

	if (updated || p_inputs_updated) {
		printf("Translating\n");
		Vector3 translate = default_translation;
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
			if (p_inputs[1].get_type() == Variant::REAL) {
				translate.x = p_inputs[1];
			}
		}
		if (input_count > 2) {
			if (p_inputs[2].get_type() == Variant::REAL) {
				translate.y = p_inputs[2];
			}
		}
		if (input_count > 3) {
			if (p_inputs[3].get_type() == Variant::REAL) {
				translate.z = p_inputs[3];
			}
		}

		if (num_vectors > 0) {
			vectors.resize(num_vectors);

			PoolVector3Array::Write w = vectors.write();
			PoolVector3Array::Read r = input_vectors.read();

			for (int i = 0; i < num_vectors; i++) {
				w[i] = r[i] + translate;
			}

		} else {
			vectors.resize(0);
		}
	}

	return updated;
}

int GDProcTranslate::get_input_connector_count() const {
	return 4;
}

Variant::Type GDProcTranslate::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else {
		return Variant::REAL;
	}
}

const String GDProcTranslate::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "input";
	} else if (p_slot == 1) {
		return "x";
	} else if (p_slot == 2) {
		return "y";
	} else if (p_slot == 3) {
		return "z";
	}

	return "";
}

const String GDProcTranslate::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "x";
	} else if (p_slot == 2) {
		return "y";
	} else if (p_slot == 3) {
		return "z";
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
	return "output";
}

const Variant GDProcTranslate::get_output(int p_slot) const {
	return Variant(vectors);
}

