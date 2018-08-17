#include "modifiers/gdprocplaceonpath.h"

using namespace godot;

void GDProcPlaceOnPath::_register_methods() {
}

String GDProcPlaceOnPath::get_type_name() {
	return "Place on path";
}

void GDProcPlaceOnPath::_init() {
	// first call super class
	GDProcNode::_init();

}

bool GDProcPlaceOnPath::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		Array input_surface;
		int strafe = 0;
		int num_rotations = 0;
		PoolVector3Array rotations;
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
				input_surface = p_inputs[0];
			}
		}
		// add rotations
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
			rotations.push_back(Vector3(0.0, 0.0, 0.0));
			num_rotations++;
		}

		// if we don't have inputs just default to center
		if (num_translations == 0) {
			translations.push_back(Vector3(0.0, 0.0, 0.0));
			num_translations++;
		}

		if (num_scales == 0) {
			scales.push_back(Vector3(1.0, 1.0, 1.0));
			num_scales++;
		}

		if (input_surface.size() == ArrayMesh::ARRAY_MAX) {
			int copies = num_translations > num_scales ? num_translations : num_scales;

			PoolVector3Array::Read t = translations.read();
			PoolVector3Array::Read s = scales.read();

			// Convert my quarternion array so we don't keep doing a square root
			PoolVector3Array::Read q = rotations.read();
			Basis *rots = (Basis *)api->godot_alloc(sizeof(Basis) * num_rotations);
			for (int i = 0; i < num_rotations; i++) {
				Vector3 rot = q[i];
				Quat quat(rot.x, rot.y, rot.z, (float) sqrt(1.0 - (rot.x * rot.x + rot.y * rot.y + rot.z * rot.z)));

				// convert to basis, I had problems with quat.xform...
				rots[i] = Basis(quat);
			}

			///@TODO this is a lot of duplication of code, there must be an easier way

			// Duplicate vertices and place
			if (input_surface[ArrayMesh::ARRAY_VERTEX].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				PoolVector3Array input = input_surface[ArrayMesh::ARRAY_VERTEX];
				PoolVector3Array output;

				int n = 0;
				int size = input.size();
				strafe = size; // remember this

				// printf("Duplicating %i vertices %i times\n", size, copies);

				output.resize(size * copies);
				PoolVector3Array::Write w = output.write();
				PoolVector3Array::Read r = input.read();
				for (int copy = 0; copy < copies; copy++) {
					Basis rotation = rots[copy % num_rotations];
					Vector3 translation = t[copy % num_translations];
					Vector3 scale = s[copy % num_scales];
					for(int i = 0; i < size; i++) {
						w[n++] = (rotation.xform(r[i]) * scale) + translation;
					}
				}

				surface_arr[ArrayMesh::ARRAY_VERTEX] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_VERTEX] = Variant();
			}

			// Duplicate normals
			if (input_surface[ArrayMesh::ARRAY_NORMAL].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				PoolVector3Array input = input_surface[ArrayMesh::ARRAY_NORMAL];
				PoolVector3Array output;

				// printf("Duplicating normals %i times\n", copies);

				for (int copy = 0; copy < copies; copy++) {
					output.append_array(input);
				}

				surface_arr[ArrayMesh::ARRAY_NORMAL] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_NORMAL] = Variant();
			}

			// Duplicate tangents
			if (input_surface[ArrayMesh::ARRAY_TANGENT].get_type() == Variant::POOL_REAL_ARRAY) {
				PoolRealArray input = input_surface[ArrayMesh::ARRAY_TANGENT];
				PoolRealArray output;

				// printf("Duplicating tangents %i times\n", copies);

				for (int copy = 0; copy < copies; copy++) {
					output.append_array(input);
				}

				surface_arr[ArrayMesh::ARRAY_TANGENT] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_TANGENT] = Variant();
			}

			// Duplicate color
			if (input_surface[ArrayMesh::ARRAY_COLOR].get_type() == Variant::POOL_COLOR_ARRAY) {
				PoolColorArray input = input_surface[ArrayMesh::ARRAY_COLOR];
				PoolColorArray output;

				// printf("Duplicating color %i times\n", copies);

				for (int copy = 0; copy < copies; copy++) {
					output.append_array(input);
				}

				surface_arr[ArrayMesh::ARRAY_COLOR] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_COLOR] = Variant();
			}

			// Duplicate uv (should I change this to vec3? or support both?)
			if (input_surface[ArrayMesh::ARRAY_TEX_UV].get_type() == Variant::POOL_VECTOR2_ARRAY) {
				PoolVector2Array input = input_surface[ArrayMesh::ARRAY_TEX_UV];
				PoolVector2Array output;

				// printf("Duplicating uvs %i times\n", copies);

				for (int copy = 0; copy < copies; copy++) {
					output.append_array(input);
				}

				surface_arr[ArrayMesh::ARRAY_TEX_UV] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_TEX_UV] = Variant();
			}

			// Duplicate uv2 (should I change this to vec3? or support both?)
			if (input_surface[ArrayMesh::ARRAY_TEX_UV2].get_type() == Variant::POOL_VECTOR2_ARRAY) {
				PoolVector2Array input = input_surface[ArrayMesh::ARRAY_TEX_UV2];
				PoolVector2Array output;

				// printf("Duplicating uv2s %i times\n", copies);

				for (int copy = 0; copy < copies; copy++) {
					output.append_array(input);
				}

				surface_arr[ArrayMesh::ARRAY_TEX_UV2] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_TEX_UV2] = Variant();
			}

			// Duplicate bones
			if (input_surface[ArrayMesh::ARRAY_BONES].get_type() == Variant::POOL_REAL_ARRAY) {
				PoolRealArray input = input_surface[ArrayMesh::ARRAY_BONES];
				PoolRealArray output;

				// printf("Duplicating bones %i times\n", copies);

				for (int copy = 0; copy < copies; copy++) {
					output.append_array(input);
				}

				surface_arr[ArrayMesh::ARRAY_BONES] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_BONES] = Variant();
			}

			// Duplicate weights
			if (input_surface[ArrayMesh::ARRAY_WEIGHTS].get_type() == Variant::POOL_REAL_ARRAY) {
				PoolRealArray input = input_surface[ArrayMesh::ARRAY_WEIGHTS];
				PoolRealArray output;

				// printf("Duplicating bones %i times\n", copies);

				for (int copy = 0; copy < copies; copy++) {
					output.append_array(input);
				}

				surface_arr[ArrayMesh::ARRAY_WEIGHTS] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_WEIGHTS] = Variant();
			}

			// Duplicate indices
			if (input_surface[ArrayMesh::ARRAY_INDEX].get_type() == Variant::POOL_INT_ARRAY) {
				PoolIntArray input = input_surface[ArrayMesh::ARRAY_INDEX];
				PoolIntArray output;

				int n = 0;
				int size = input.size();

				// printf("Duplicating %i indices %i times\n", size, copies);

				output.resize(size * copies);
				PoolIntArray::Write w = output.write();
				PoolIntArray::Read r = input.read();
				for (int copy = 0; copy < copies; copy++) {
					int offset = copy * strafe;
					for(int i = 0; i < size; i++) {
						w[n++] = r[i] + offset;
					}
				}

				surface_arr[ArrayMesh::ARRAY_INDEX] = output;
			} else {
				surface_arr[ArrayMesh::ARRAY_INDEX] = Variant();
			}

			// free 
			api->godot_free(rots);
		}
	}

	return updated;
}

int GDProcPlaceOnPath::get_input_connector_count() const {
	return 4;
}

Variant::Type GDProcPlaceOnPath::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::ARRAY;
	} else if (p_slot == 1) {
		// we don't have a POOL_BASIS_ARRAY, abusing vec3 for storing 
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 2) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 3) {
		return Variant::POOL_VECTOR3_ARRAY;
	}

	return Variant::NIL;
}

const String GDProcPlaceOnPath::get_input_connector_name(int p_slot) const {
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

int GDProcPlaceOnPath::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcPlaceOnPath::get_output_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcPlaceOnPath::get_output_connector_name(int p_slot) const {
	return "surface";
}

const Variant GDProcPlaceOnPath::get_output(int p_slot) const {
	return Variant(surface_arr);
}

