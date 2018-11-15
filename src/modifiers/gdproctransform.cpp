#include "modifiers/gdproctransform.h"

using namespace godot;

void GDProcTransform::_register_methods() {
	register_property<GDProcTransform, Vector3>("rotation", &GDProcTransform::set_rotation, &GDProcTransform::get_rotation, Vector3(0.0, 0.0, 0.0));
	register_property<GDProcTransform, Vector3>("translation", &GDProcTransform::set_translation, &GDProcTransform::get_translation, Vector3(0.0, 0.0, 0.0));
	register_property<GDProcTransform, Vector3>("scale", &GDProcTransform::set_scale, &GDProcTransform::get_scale, Vector3(1.0, 1.0, 1.0));
}

String GDProcTransform::get_type_name() {
	return "Transform";
}

String GDProcTransform::get_description() const {
	return "Transforms the surface by the given translation, rotation and scale.";
}

void GDProcTransform::_init() {
	// first call super class
	GDProcNode::_init();

	default_rotation = Vector3(0.0, 0.0, 0.0);
	default_translation = Vector3(0.0, 0.0, 0.0);
	default_scale = Vector3(1.0, 1.0, 1.0);
}

void GDProcTransform::set_rotation(Vector3 p_rotation) {
	if (default_rotation != p_rotation) {
		default_rotation = p_rotation;
		must_update = true;
		emit_signal("changed");
	}
}

Vector3 GDProcTransform::get_rotation() {
	return default_rotation;
}

void GDProcTransform::set_translation(Vector3 p_translation) {
	if (default_translation != p_translation) {
		default_translation = p_translation;
		must_update = true;
		emit_signal("changed");
	}
}

Vector3 GDProcTransform::get_translation() {
	return default_translation;
}

void GDProcTransform::set_scale(Vector3 p_scale) {
	if (default_scale != p_scale) {
		default_scale = p_scale;
		must_update = true;
		emit_signal("changed");
	}
}

Vector3 GDProcTransform::get_scale() {
	return default_scale;
}

bool GDProcTransform::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_vectors = 0;
		PoolVector3Array input_vectors;
		int num_normals = 0;
		PoolVector3Array input_normals;
		int num_tangents = 0;
		PoolRealArray input_tangents;
		int num_rotations = 0;
		PoolColorArray rotations;
		int num_translations = 0;
		PoolVector3Array translations;
		int num_scales = 0;
		PoolVector3Array scales;

		// resize our array just in case
		surface_arr.resize(ArrayMesh::ARRAY_MAX);

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::ARRAY) {
				// get our surface
				Array surface = p_inputs[0];
				if (surface.size() == ArrayMesh::ARRAY_MAX) {
					// we're going to process our vectors
					input_vectors = surface[ArrayMesh::ARRAY_VERTEX];
					num_vectors = input_vectors.size();
					input_normals = surface[ArrayMesh::ARRAY_NORMAL];
					num_normals = input_normals.size();
					input_tangents = surface[ArrayMesh::ARRAY_TANGENT];
					num_tangents = input_tangents.size();

					// and just copy the rest
					surface_arr[ArrayMesh::ARRAY_COLOR] = surface[ArrayMesh::ARRAY_COLOR];
					surface_arr[ArrayMesh::ARRAY_TEX_UV] = surface[ArrayMesh::ARRAY_TEX_UV];
					surface_arr[ArrayMesh::ARRAY_TEX_UV2] = surface[ArrayMesh::ARRAY_TEX_UV2];
					surface_arr[ArrayMesh::ARRAY_BONES] = surface[ArrayMesh::ARRAY_BONES];
					surface_arr[ArrayMesh::ARRAY_WEIGHTS] = surface[ArrayMesh::ARRAY_WEIGHTS];
					surface_arr[ArrayMesh::ARRAY_INDEX] = surface[ArrayMesh::ARRAY_INDEX];
				}
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_COLOR_ARRAY) {
				rotations = p_inputs[1];
				num_rotations = rotations.size();
			}
		}
		if (input_count > 2) {
			if (p_inputs[2].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				translations = p_inputs[2];
				num_translations = translations.size();
			}
		}
		if (input_count > 3) {
			if (p_inputs[3].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				scales = p_inputs[3];
				num_scales = scales.size();
			}
		}

		if (num_rotations == 0) {
			// our default rotation contains euler angles, but we're using quaternions
			Quat q;
			float pi_180 = 3.14159265359f / 180.0f;
			q.set_euler_xyz(default_rotation * Vector3(pi_180, pi_180, pi_180));
			rotations.push_back(Color(q.x, q.y, q.z, q.w)); // quaternions should be normalized so w should be sqrt(1.0 - x2 - y2 - z2)
			num_rotations++;
		}

		if (num_translations == 0) {
			translations.push_back(default_translation);
			num_translations++;
		}

		if (num_scales == 0) {
			scales.push_back(default_scale);
			num_scales++;
		}

		// Convert my quaternion array to basis, xform on quaternion seems broken
		PoolColorArray::Read q = rotations.read();
		Basis *rots = (Basis *)api->godot_alloc(sizeof(Basis) * num_rotations);
		for (int i = 0; i < num_rotations; i++) {
			Color rot = q[i];
			Quat quat(rot.r, rot.g, rot.b, rot.a);

			// convert to basis, I had problems with quat.xform...
			rots[i] = Basis(quat);
		}

		// Lock our inputs for reading
		PoolVector3Array::Read t = translations.read();
		PoolVector3Array::Read s = scales.read();

		if (num_vectors > 0) {
			PoolVector3Array vectors;
			vectors.resize(num_vectors);

			PoolVector3Array::Write w = vectors.write();
			PoolVector3Array::Read r = input_vectors.read();

			for (int i = 0; i < num_vectors; i++) {
				w[i] = (rots[i % num_rotations].xform(r[i]) * s[i % num_scales]) + t[i % num_translations];
			}

			surface_arr[ArrayMesh::ARRAY_VERTEX] = vectors;
		}

		if (num_normals > 0) {
			PoolVector3Array normals;
			normals.resize(num_normals);

			PoolVector3Array::Write w = normals.write();
			PoolVector3Array::Read r = input_normals.read();

			for (int i = 0; i < num_normals; i++) {
				w[i] = rots[i % num_rotations].xform(r[i]);
			}

			surface_arr[ArrayMesh::ARRAY_NORMAL] = normals;
		}

		if (num_tangents > 0) {
			int j = 0;
			PoolRealArray tangents;
			tangents.resize(num_tangents);

			PoolRealArray::Write w = tangents.write();
			PoolRealArray::Read r = input_tangents.read();

			for (int i = 0; i < num_tangents; i += 4) {
				Vector3 v(r[i], r[i + 1], r[i + 2]);
				v = rots[j % num_rotations].xform(v);
				w[i] = v.x;
				w[i + 1] = v.y;
				w[i + 2] = v.z;
				w[i + 3] = r[i + 3]; // no need to update this...
				j++;
			}

			surface_arr[ArrayMesh::ARRAY_TANGENT] = tangents;
		}

		// free
		api->godot_free(rots);
	}

	return updated;
}

int GDProcTransform::get_input_connector_count() const {
	return 4;
}

Variant::Type GDProcTransform::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::ARRAY;
	} else if (p_slot == 1) {
		// we don't have a POOL_QUAD_ARRAY, abusing color for storing
		return Variant::POOL_COLOR_ARRAY;
	} else if (p_slot == 2) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 3) {
		return Variant::POOL_VECTOR3_ARRAY;
	}

	return Variant::NIL;
}

const String GDProcTransform::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "surface";
	} else if (p_slot == 1) {
		return "rotations";
	} else if (p_slot == 2) {
		return "translations";
	} else if (p_slot == 3) {
		return "scales";
	}

	return "";
}

const String GDProcTransform::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "rotation";
	} else if (p_slot == 2) {
		return "translation";
	} else if (p_slot == 3) {
		return "scale";
	}

	return "";
}

int GDProcTransform::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcTransform::get_output_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcTransform::get_output_connector_name(int p_slot) const {
	return "surface";
}

const Variant GDProcTransform::get_output(int p_slot) const {
	return Variant(surface_arr);
}
