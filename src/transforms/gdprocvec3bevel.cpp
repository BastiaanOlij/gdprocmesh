#include "transforms/gdprocvec3bevel.h"

using namespace godot;

void GDProcVec3Bevel::_register_methods() {
	register_property<GDProcVec3Bevel, float>("distance", &GDProcVec3Bevel::set_distance, &GDProcVec3Bevel::get_distance, 0.1f);
	register_property<GDProcVec3Bevel, int>("iterations", &GDProcVec3Bevel::set_iterations, &GDProcVec3Bevel::get_iterations, 1);
	register_property<GDProcVec3Bevel, bool>("is_closed", &GDProcVec3Bevel::set_is_closed, &GDProcVec3Bevel::get_is_closed, false);
}

String GDProcVec3Bevel::get_type_name() {
	return "Vec3 Bevel";
}

void GDProcVec3Bevel::_init() {
	// first call super class
	GDProcNode::_init();

	// defaults
	distance = 0.1f;
	iterations = 1;
	is_closed = false;
}

void GDProcVec3Bevel::set_distance(float p_distance) {
	if (distance != p_distance) {
		distance = p_distance;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcVec3Bevel::get_distance() const {
	return distance;	
}

void GDProcVec3Bevel::set_iterations(int p_iterations) {
	if (p_iterations <= 0) {
		printf("Can't set iterations to %i\n", p_iterations);
		return;
	} else if (p_iterations > 10) {
		printf("Can't set iterations to %i\n", p_iterations);
		return;
	}

	if (iterations != p_iterations) {
		iterations = p_iterations;
		must_update = true;
		emit_signal("changed");		
	}
}

int GDProcVec3Bevel::get_iterations() const {
	return iterations;
}

void GDProcVec3Bevel::set_is_closed(bool p_is_closed) {
	if (is_closed != p_is_closed) {
		is_closed = p_is_closed;
		must_update = true;
		emit_signal("changed");
	}
}

bool GDProcVec3Bevel::get_is_closed() const {
	return is_closed;
}

void GDProcVec3Bevel::do_bevel(PoolVector3Array::Write &p_w, Vector3 p1, Vector3 p2, Vector3 p3, float p_distance, int p_iterations, int &p_idx) {
	Vector3 n = (p1 - p2).normalized() * p_distance;
	Vector3 a = p2 + n;
	n = (p3 - p2).normalized() * p_distance;
	Vector3 b = p2 + n;

	if (p_iterations > 1) {
		float new_dist = (b - a).length() / 4.0f;

		do_bevel(p_w, p1, a, b, new_dist, p_iterations - 1, p_idx);
		do_bevel(p_w, a, b, p3, new_dist, p_iterations - 1, p_idx);
	} else {
		// printf("Adding %i -> %0.2f, %0.2f, %0.2f\n", p_idx, a.x, a.y, a.z);
		p_w[p_idx++] = a;
		// printf("Adding %i -> %0.2f, %0.2f, %0.2f\n", p_idx, b.x, b.y, b.z);
		p_w[p_idx++] = b;
	}
}

bool GDProcVec3Bevel::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		float d = distance;
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
				d = p_inputs[1];
			}
		}

		if (num_vectors > 0) {
			int o = 0;
			int cnt = 1;
			for (int i = 0; i < iterations; i++) {
				cnt *= 2;
			}
			if (is_closed) {
				cnt = num_vectors * cnt;
			} else {
				cnt = 2 + ((num_vectors - 2) * cnt);
			}
			// printf("Input points: %i\nIterations: %i\nNew number of points: %i\n", num_vectors, iterations, cnt);
			vectors.resize(cnt);

			PoolVector3Array::Write w = vectors.write();
			PoolVector3Array::Read r = input_vectors.read();

			if (is_closed) {
				for (int b = 0; b < num_vectors; b++) {
					int a = b == 0 ? num_vectors - 1 : (b - 1);
					int c = (b + 1) % num_vectors;

					do_bevel(w, r[a], r[b], r[c], d, iterations, o);
				}
			} else {
				// copy our first one
				w[o++] = r[0];

				for (int i = 1; i < num_vectors - 1; i++) {
					do_bevel(w, r[i-1], r[i], r[i+1], d, iterations, o);
				}

				// copy our last one
				w[o++] = r[num_vectors - 1];
			}

		} else {
			vectors.resize(0);
		}
	}

	return updated;
}

int GDProcVec3Bevel::get_input_connector_count() const {
	return 4;
}

Variant::Type GDProcVec3Bevel::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 1) {
		return Variant::REAL;
	} else if (p_slot == 2) {
		return Variant::INT;
	} else if (p_slot == 3) {
		return Variant::BOOL;
	}
	return Variant::NIL;
}

const String GDProcVec3Bevel::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "vec3s";
	} else if (p_slot == 1) {
		return "distance";
	} else if (p_slot == 2) {
		return "iterations";
	} else if (p_slot == 3) {
		return "is_closed";
	}

	return "";
}

const String GDProcVec3Bevel::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "distance";
	} else if (p_slot == 2) {
		return "iterations";
	} else if (p_slot == 3) {
		return "is_closed";
	}

	return "";
}

int GDProcVec3Bevel::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcVec3Bevel::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcVec3Bevel::get_output_connector_name(int p_slot) const {
	return "vec3s";
}

const Variant GDProcVec3Bevel::get_output(int p_slot) const {
	return Variant(vectors);
}

