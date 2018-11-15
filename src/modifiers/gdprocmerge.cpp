#include "modifiers/gdprocmerge.h"

using namespace godot;

void GDProcMerge::_register_methods() {
}

String GDProcMerge::get_type_name() {
	return "Merge";
}

String GDProcMerge::get_description() const {
	return "Merges up to five surfaces into one.";
}

void GDProcMerge::_init() {
	// first call super class
	GDProcNode::_init();
}

bool GDProcMerge::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		Array input_surface;
		PoolVector3Array vertices;
		PoolVector3Array normals;
		PoolRealArray tangents;
		PoolColorArray colors;
		PoolVector2Array uvs;
		PoolVector2Array uv2s;
		PoolRealArray bones;
		PoolRealArray weights;
		PoolIntArray indices;

		// resize our array just in case
		surface_arr.resize(ArrayMesh::ARRAY_MAX);

		// clear our array
		for (int i = 0; i < ArrayMesh::ARRAY_MAX; i++) {
			surface_arr[i] = Variant();
		}

		// parse our inputs
		int input_count = p_inputs.size();
		for (int i = 0; i < input_count; i++) {
			if (p_inputs[i].get_type() == Variant::ARRAY) {
				// get our surface
				Array input_surface = p_inputs[i];

				// can we add it?
				if (input_surface.size() == ArrayMesh::ARRAY_MAX) {
					int offset = vertices.size();

					if (input_surface[ArrayMesh::ARRAY_VERTEX].get_type() == Variant::POOL_VECTOR3_ARRAY) {
						// just merge
						PoolVector3Array input = input_surface[ArrayMesh::ARRAY_VERTEX];
						vertices.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_NORMAL].get_type() == Variant::POOL_VECTOR3_ARRAY) {
						// just merge
						if (normals.size() < offset) {
							// add empty normals for our previous meshes
							normals.resize(offset);
						}

						PoolVector3Array input = input_surface[ArrayMesh::ARRAY_NORMAL];
						normals.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_TANGENT].get_type() == Variant::POOL_REAL_ARRAY) {
						// just merge
						if (tangents.size() < offset * 4) {
							// add empty tangents for our previous meshes
							tangents.resize(offset * 4);
						}

						PoolRealArray input = input_surface[ArrayMesh::ARRAY_TANGENT];
						tangents.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_COLOR].get_type() == Variant::POOL_COLOR_ARRAY) {
						// just merge
						if (colors.size() < offset) {
							// add empty colors for our previous meshes
							colors.resize(offset);
						}

						PoolColorArray input = input_surface[ArrayMesh::ARRAY_COLOR];
						colors.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_TEX_UV].get_type() == Variant::POOL_VECTOR2_ARRAY) {
						// just merge
						if (uvs.size() < offset) {
							// add empty colors for our previous meshes
							uvs.resize(offset);
						}

						PoolVector2Array input = input_surface[ArrayMesh::ARRAY_TEX_UV];
						uvs.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_TEX_UV2].get_type() == Variant::POOL_VECTOR2_ARRAY) {
						// just merge
						if (uv2s.size() < offset) {
							// add empty colors for our previous meshes
							uv2s.resize(offset);
						}

						PoolVector2Array input = input_surface[ArrayMesh::ARRAY_TEX_UV2];
						uv2s.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_BONES].get_type() == Variant::POOL_REAL_ARRAY) {
						// just merge (do we need to multiply here?)
						if (bones.size() < offset) {
							// add empty colors for our previous meshes
							bones.resize(offset);
						}

						PoolRealArray input = input_surface[ArrayMesh::ARRAY_BONES];
						bones.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_WEIGHTS].get_type() == Variant::POOL_REAL_ARRAY) {
						// just merge (do we need to multiply here?)
						if (weights.size() < offset) {
							// add empty colors for our previous meshes
							weights.resize(offset);
						}

						PoolRealArray input = input_surface[ArrayMesh::ARRAY_WEIGHTS];
						weights.append_array(input);
					}

					if (input_surface[ArrayMesh::ARRAY_INDEX].get_type() == Variant::POOL_INT_ARRAY) {
						PoolIntArray input = input_surface[ArrayMesh::ARRAY_INDEX];
						int cur_size = indices.size();
						int input_size = input.size();

						// double the size of our buffer
						indices.resize(cur_size + input_size);
						PoolIntArray::Write w = indices.write();
						PoolIntArray::Read r = input.read();

						for (int i = 0; i < input_size; i++) {
							w[i + cur_size] = r[i] + offset;
						}
					}
				}
			}

			if (vertices.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_VERTEX] = vertices;
			}
			if (normals.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_NORMAL] = normals;
			}
			if (tangents.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_TANGENT] = tangents;
			}
			if (colors.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_COLOR] = colors;
			}
			if (uvs.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_TEX_UV] = uvs;
			}
			if (uv2s.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_TEX_UV2] = uv2s;
			}
			if (bones.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_BONES] = bones;
			}
			if (weights.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_WEIGHTS] = weights;
			}
			if (indices.size() > 0) {
				surface_arr[ArrayMesh::ARRAY_INDEX] = indices;
			}
		}
	}

	return updated;
}

int GDProcMerge::get_input_connector_count() const {
	return 5;
}

Variant::Type GDProcMerge::get_input_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcMerge::get_input_connector_name(int p_slot) const {
	return "surface";
}

int GDProcMerge::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcMerge::get_output_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcMerge::get_output_connector_name(int p_slot) const {
	return "surface";
}

const Variant GDProcMerge::get_output(int p_slot) const {
	return surface_arr;
}
