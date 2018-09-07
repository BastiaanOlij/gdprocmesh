#include "transforms/gdprocredist.h"

using namespace godot;

void GDProcRedist::_register_methods() {
	register_property<GDProcRedist, float>("offset", &GDProcRedist::set_offset, &GDProcRedist::get_offset, 0.0f);
	register_property<GDProcRedist, float>("strafe", &GDProcRedist::set_strafe, &GDProcRedist::get_strafe, 0.2f);
	register_property<GDProcRedist, bool>("is_closed", &GDProcRedist::set_is_closed, &GDProcRedist::get_is_closed, false);
	register_property<GDProcRedist, bool>("follow", &GDProcRedist::set_follow, &GDProcRedist::get_follow, false);
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
	default_is_closed = false;
	default_follow = false;
}

void GDProcRedist::set_is_closed(bool p_is_closed) {
	if (default_is_closed != p_is_closed) {
		default_is_closed = p_is_closed;
		must_update = true;
		emit_signal("changed");
	}
}

bool GDProcRedist::get_is_closed() const {
	return default_is_closed;
}

void GDProcRedist::set_follow(bool p_follow) {
	if (default_follow != p_follow) {
		default_follow = p_follow;
		must_update = true;
		emit_signal("changed");
	}
}

bool GDProcRedist::get_follow() const {
	return default_follow;
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
		PoolColorArray input_rotations; // calculated, and we're abusing color here because its basically a vec4
		bool is_closed = default_is_closed;
		bool follow = default_follow;

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

		if (input_count > 3) {
			if (p_inputs[3].get_type() == Variant::BOOL) {
				is_closed = p_inputs[3];
			}
		}

		if (input_count > 4) {
			if (p_inputs[4].get_type() == Variant::BOOL) {
				follow = p_inputs[4];
			}
		}


		if (num_strafes == 0) {
			strafes.push_back(default_strafe);
			num_strafes++;
		}

		// Calculate rotations at each source vertex
		if (num_vectors > 0) {
			input_rotations.resize(num_vectors);
			PoolColorArray::Write w = input_rotations.write();

			PoolVector3Array::Read vec = input_vectors.read();

			Vector3 up = Vector3(0.0, 1.0, 0.0);
			for (int p = 0; p < num_vectors; p++) {
				// calculate our transform
				Transform xf;
				Vector3 dir;
				if (is_closed) {
					Vector3 p1 = vec[p == 0 ? (num_vectors - 1) : ((p - 1) % num_vectors)];
					Vector3 p2 = vec[(p + 1) % num_vectors];
					dir = (p2 - p1).normalized();
				} else {
					Vector3 p1 = vec[p == 0 ? 0 : (p - 1)];
					Vector3 p2 = vec[(p + 1) < num_vectors ? (p + 1) : (num_vectors - 1)];
					dir = (p2 - p1).normalized();
				}

				// our first entry is straight up or down?
				if ((p == 0) && (fabs(dir.dot(up)) > 0.999f)) {
					// let's start with a different value
					up = Vector3(1.0, 0.0, 0.0);
				}

				// calculate our new transform
				xf.origin = vec[p % num_vectors];
				xf = xf.looking_at(xf.origin + dir, up);

				// convert to quat and store as a color (we don't have a quad array or I would use that)
				Quat q = xf.basis;
				w[p] = Color(q.x, q.y, q.z, q.w);

				// use our new up for our last up
				if (follow) {
					up = xf.basis.get_axis(1);
				}
			}
		}

		vectors.resize(0);
		rotations.resize(0);
		if (num_vectors > 0) {
			int cur_vector = 0;
			int cur_strafe = 0;
			float cur_dist = offset < 0.0f ? 0.0f : offset;

			PoolVector3Array::Read vec = input_vectors.read();
			PoolColorArray::Read rot = input_rotations.read();
			PoolRealArray::Read s = strafes.read();

			// get our starting pos...
			while (cur_vector + 1 < (is_closed ? num_vectors + 1 : num_vectors)) {
				// get our position, next position and delta
				int cur_p = cur_vector % num_vectors;
				int next_p = (cur_vector + 1) % num_vectors;
				Vector3 cur_pos = vec[cur_p];
				Vector3 next_pos = vec[next_p];
				Vector3 delta_pos = next_pos - cur_pos;

				// get our rotation, next rotation
				Quat cur_rot(rot[cur_p].r, rot[cur_p].g, rot[cur_p].b, rot[cur_p].a);
				Quat next_rot(rot[next_p].r, rot[next_p].g, rot[next_p].b, rot[next_p].a);

				float used = 0.0f;
				float distance = delta_pos.length();

				while (distance >= used + cur_dist) {
					// add
					used += cur_dist;
					float f = used / distance;
					Vector3 v = cur_pos + (delta_pos * f);
					vectors.push_back(v);

					// interpolate our rotation, then encode in a vector
					// Quat qr = cur_rot.slerp(next_rot, f);
					Quat qr = cur_rot;
					rotations.push_back(Color(qr.x, qr.y, qr.z, qr.w));

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
	return 5;
}

Variant::Type GDProcRedist::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 1) {
		return Variant::POOL_REAL_ARRAY;
	} else if (p_slot == 2) {
		return Variant::POOL_REAL_ARRAY;
	} else if (p_slot == 3) {
		return Variant::BOOL;
	} else if (p_slot == 4) {
		return Variant::BOOL;
	} else {
		return Variant::NIL;
	}
}

const String GDProcRedist::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "vectors";
	} else if (p_slot == 1) {
		return "offset";
	} else if (p_slot == 2) {
		return "strafe";
	} else if (p_slot == 3) {
		return "is_closed";
	} else if (p_slot == 4) {
		return "follow";
	}

	return "";
}

const String GDProcRedist::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "offset";
	} else if (p_slot == 2) {
		return "strafe";
	} else if (p_slot == 3) {
		return "is_closed";
	} else if (p_slot == 4) {
		return "follow";
	}

	return "";
}

int GDProcRedist::get_output_connector_count() const {
	return 2;
}

Variant::Type GDProcRedist::get_output_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 1) {
		return Variant::POOL_COLOR_ARRAY;
	} else {
		return Variant::NIL;
	}
}

const String GDProcRedist::get_output_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "vectors";
	} else if (p_slot == 1) {
		return "rotations";
	} else {
		return "";
	}
}

const Variant GDProcRedist::get_output(int p_slot) const {
	if (p_slot == 0) {
		return Variant(vectors);
	} else if (p_slot == 1) {
		return Variant(rotations);
	} else {
		return Variant();
	}
}

