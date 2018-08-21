#include "transforms/gdprocredist.h"

using namespace godot;

void GDProcRedist::_register_methods() {
	register_property<GDProcRedist, float>("offset", &GDProcRedist::set_offset, &GDProcRedist::get_offset, 0.0f);
	register_property<GDProcRedist, float>("strafe", &GDProcRedist::set_strafe, &GDProcRedist::get_strafe, 0.2f);
}

String GDProcRedist::get_type_name() {
	return "Redistribute";
}

String GDProcRedist::get_description() const {
	return "This node creates an array of vertices by following the path set out by the input array and taking a sample according to the strafe settings.";
}

void GDProcRedist::_init() {
	// first call super class
	GDProcNode::_init();

	default_offset = 0.0f;
	default_strafe = 0.2f;
}

void GDProcRedist::set_offset(float p_offset) {
	if (default_offset != p_offset) {
		default_offset = p_offset;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcRedist::get_offset() {
	return default_offset;
}

void GDProcRedist::set_strafe(float p_strafe) {
	if (default_strafe != p_strafe) {
		default_strafe = p_strafe;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcRedist::get_strafe() {
	return default_strafe;
}

bool GDProcRedist::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		float offset = default_offset;
		int num_strafes = 0;
		PoolRealArray strafes;
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
			if (p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY) {
				// just get our first entry
				PoolRealArray offsets = p_inputs[1];
				if (offsets.size() > 0) {
					offset = offsets[0];
				}
			}
		}
		if (input_count > 2) {
			if (p_inputs[2].get_type() == Variant::POOL_REAL_ARRAY) {
				strafes = p_inputs[2];
				num_strafes = strafes.size();
			}
		}

		if (num_strafes == 0) {
			strafes.push_back(default_strafe);
			num_strafes++;
		}

		vectors.resize(0);
		if (num_vectors > 0) {
			int cur_vector = 0;
			int cur_strafe = 0;
			float cur_dist = offset < 0.0f ? 0.0f : offset;

			PoolVector3Array::Read r = input_vectors.read();
			PoolRealArray::Read s = strafes.read();

			// get our starting pos...
			while (cur_vector + 1 < num_vectors) {
				Vector3 cur_pos = r[cur_vector];
				Vector3 next_pos = r[cur_vector + 1];
				Vector3 delta = next_pos - cur_pos;
				float used = 0.0f;
				float distance = delta.length();

				while (distance >= used + cur_dist) {
					// add
					used += cur_dist;
					Vector3 v = cur_pos + (delta * used / distance);
					vectors.push_back(v);

					// next distance
					cur_dist = s[cur_strafe % num_strafes];
					if (cur_dist <= 0.0f) cur_dist = 0.1f;
					cur_strafe++;
				}

				// subtract our left over
				cur_dist -= (distance - used);

				// next
				cur_vector++;
			}
		}
	}

	return updated;
}

int GDProcRedist::get_input_connector_count() const {
	return 3;
}

Variant::Type GDProcRedist::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else {
		return Variant::POOL_REAL_ARRAY;
	}
}

const String GDProcRedist::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "vectors";
	} else if (p_slot == 1) {
		return "offset";
	} else if (p_slot == 2) {
		return "strafe";
	}

	return "";
}

const String GDProcRedist::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "offset";
	} else if (p_slot == 2) {
		return "strafe";
	}

	return "";
}

int GDProcRedist::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcRedist::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcRedist::get_output_connector_name(int p_slot) const {
	return "vectors";
}

const Variant GDProcRedist::get_output(int p_slot) const {
	return Variant(vectors);
}

