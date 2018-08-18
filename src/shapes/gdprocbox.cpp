#include "shapes/gdprocbox.h"

using namespace godot;

void GDProcBox::_register_methods() {
	register_property<GDProcBox, float>("width", &GDProcBox::set_width, &GDProcBox::get_width, 1.0);
	register_property<GDProcBox, float>("height", &GDProcBox::set_height, &GDProcBox::get_height, 1.0);
	register_property<GDProcBox, float>("depth", &GDProcBox::set_depth, &GDProcBox::get_depth, 1.0);
}

String GDProcBox::get_type_name() {
	return String("Box");
}

void GDProcBox::_init() {
	// first call super class
	GDProcNode::_init();

	// default size
	default_size = Vector3(1.0, 1.0, 1.0);
}

void GDProcBox::set_width(float p_width) {
	if (default_size.x != p_width) {
		default_size.x = p_width;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcBox::get_width() const {
	return default_size.x;
}

void GDProcBox::set_height(float p_height) {
	if (default_size.y != p_height) {
		default_size.y = p_height;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcBox::get_height() const {
	return default_size.y;
}

void GDProcBox::set_depth(float p_depth) {
	if (default_size.z != p_depth) {
		default_size.z = p_depth;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcBox::get_depth() const {
	return default_size.z;
}

bool GDProcBox::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		Vector3 size = default_size;
		PoolVector3Array vertices;
		PoolVector3Array normals;
		PoolRealArray tangents;
		PoolVector2Array uvs;
		PoolIntArray indices;

		// resize our array just in case
		surface_arr.resize(ArrayMesh::ARRAY_MAX);

		// process our inputs
		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_REAL_ARRAY) {
				PoolRealArray input = p_inputs[0];
				if (input.size() > 0) {
					size.x = input[0];
				}
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY) {
				PoolRealArray input = p_inputs[1];
				if (input.size() > 0) {
					size.y = input[0];
				}
			}
		}
		if (input_count > 2) {
			if (p_inputs[2].get_type() == Variant::POOL_REAL_ARRAY) {
				PoolRealArray input = p_inputs[2];
				if (input.size() > 0) {
					size.z = input[0];
				}
			}
		}

		int num_verts = 24;

		// prepare our vertices
		vertices.resize(num_verts);
		{
			PoolVector3Array::Write v = vertices.write();
			float hsx = size.x / 2.0f;
			float hsy = size.y / 2.0f;
			float hsz = size.z / 2.0f;

			// front
			v[ 0].x = -hsx; v[ 0].y =  hsy; v[ 0].z =  hsz;
			v[ 1].x =  hsx; v[ 1].y =  hsy; v[ 1].z =  hsz;
			v[ 2].x =  hsx; v[ 2].y = -hsy; v[ 2].z =  hsz;
			v[ 3].x = -hsx; v[ 3].y = -hsy; v[ 3].z =  hsz;

			// back
			v[ 4].x =  hsx; v[ 4].y =  hsy; v[ 4].z = -hsz;
			v[ 5].x = -hsx; v[ 5].y =  hsy; v[ 5].z = -hsz;
			v[ 6].x = -hsx; v[ 6].y = -hsy; v[ 6].z = -hsz;
			v[ 7].x =  hsx; v[ 7].y = -hsy; v[ 7].z = -hsz;

			// left
			v[ 8].x = -hsx; v[ 8].y =  hsy; v[ 8].z = -hsz;
			v[ 9].x = -hsx; v[ 9].y =  hsy; v[ 9].z =  hsz;
			v[10].x = -hsx; v[10].y = -hsy; v[10].z =  hsz;
			v[11].x = -hsx; v[11].y = -hsy; v[11].z = -hsz;

			// right
			v[12].x =  hsx; v[12].y =  hsy; v[12].z =  hsz;
			v[13].x =  hsx; v[13].y =  hsy; v[13].z = -hsz;
			v[14].x =  hsx; v[14].y = -hsy; v[14].z = -hsz;
			v[15].x =  hsx; v[15].y = -hsy; v[15].z =  hsz;

			// top
			v[16].x = -hsx; v[16].y =  hsy; v[16].z = -hsz;
			v[17].x =  hsx; v[17].y =  hsy; v[17].z = -hsz;
			v[18].x =  hsx; v[18].y =  hsy; v[18].z =  hsz;
			v[19].x = -hsx; v[19].y =  hsy; v[19].z =  hsz;

			// bottom
			v[20].x = -hsx; v[20].y = -hsy; v[20].z =  hsz;
			v[21].x =  hsx; v[21].y = -hsy; v[21].z =  hsz;
			v[22].x =  hsx; v[22].y = -hsy; v[22].z = -hsz;
			v[23].x = -hsx; v[23].y = -hsy; v[23].z = -hsz;
		}

		// prepare our normals
		normals.resize(num_verts);
		{
			PoolVector3Array::Write n = normals.write();

			// front
			n[ 0].x =  0.0; n[ 0].y =  0.0; n[ 0].z =  1.0;
			n[ 1].x =  0.0; n[ 1].y =  0.0; n[ 1].z =  1.0;
			n[ 2].x =  0.0; n[ 2].y =  0.0; n[ 2].z =  1.0;
			n[ 3].x =  0.0; n[ 3].y =  0.0; n[ 3].z =  1.0;

			// back
			n[ 4].x =  0.0; n[ 4].y =  0.0; n[ 4].z = -1.0;
			n[ 5].x =  0.0; n[ 5].y =  0.0; n[ 5].z = -1.0;
			n[ 6].x =  0.0; n[ 6].y =  0.0; n[ 6].z = -1.0;
			n[ 7].x =  0.0; n[ 7].y =  0.0; n[ 7].z = -1.0;

			// left
			n[ 8].x = -1.0; n[ 8].y =  0.0; n[ 8].z =  0.0;
			n[ 9].x = -1.0; n[ 9].y =  0.0; n[ 9].z =  0.0;
			n[10].x = -1.0; n[10].y =  0.0; n[10].z =  0.0;
			n[11].x = -1.0; n[11].y =  0.0; n[11].z =  0.0;

			// right
			n[12].x =  1.0; n[12].y =  0.0; n[12].z =  0.0;
			n[13].x =  1.0; n[13].y =  0.0; n[13].z =  0.0;
			n[14].x =  1.0; n[14].y =  0.0; n[14].z =  0.0;
			n[15].x =  1.0; n[15].y =  0.0; n[15].z =  0.0;

			// top
			n[16].x =  0.0; n[16].y =  1.0; n[16].z =  0.0;
			n[17].x =  0.0; n[17].y =  1.0; n[17].z =  0.0;
			n[18].x =  0.0; n[18].y =  1.0; n[18].z =  0.0;
			n[19].x =  0.0; n[19].y =  1.0; n[19].z =  0.0;

			// bottom
			n[20].x =  0.0; n[20].y = -1.0; n[20].z =  0.0;
			n[21].x =  0.0; n[21].y = -1.0; n[21].z =  0.0;
			n[22].x =  0.0; n[22].y = -1.0; n[22].z =  0.0;
			n[23].x =  0.0; n[23].y = -1.0; n[23].z =  0.0;
		}

		// prepare our tangents
		tangents.resize(num_verts * 4);
		{
			PoolRealArray::Write t = tangents.write();
			int i = 0;

			// front
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;

			// back
			t[i++] = -1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] = -1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] = -1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] = -1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;

			// left
			t[i++] =  0.0; t[i++] =  0.0; t[i++] =  1.0; t[i++] = -1.0;
			t[i++] =  0.0; t[i++] =  0.0; t[i++] =  1.0; t[i++] = -1.0;
			t[i++] =  0.0; t[i++] =  0.0; t[i++] =  1.0; t[i++] = -1.0;
			t[i++] =  0.0; t[i++] =  0.0; t[i++] =  1.0; t[i++] = -1.0;

			// right
			t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0; t[i++] = -1.0;
			t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0; t[i++] = -1.0;
			t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0; t[i++] = -1.0;
			t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0; t[i++] = -1.0;

			// top
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;

			// bottom
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
			t[i++] =  1.0; t[i++] =  0.0; t[i++] =  0.0; t[i++] = -1.0;
		}


		// prepare our uvs
		uvs.resize(num_verts);
		{
			PoolVector2Array::Write uv = uvs.write();
			float onet = 1.0f / 3.0f;
			float twot = 2.0f / 3.0f;

			// front (1)
			uv[ 0].x =  0.0; uv[0].y =  0.0;
			uv[ 1].x = onet; uv[1].y =  0.0;
			uv[ 2].x = onet; uv[2].y =  0.5;
			uv[ 3].x =  0.0; uv[3].y =  0.5;

			// back (3)
			uv[ 4].x = twot; uv[4].y =  0.0;
			uv[ 5].x =  1.0; uv[5].y =  0.0;
			uv[ 6].x =  1.0; uv[6].y =  0.5;
			uv[ 7].x = twot; uv[7].y =  0.5;

			// left (4)
			uv[ 8].x =  0.0; uv[ 8].y =  0.5;
			uv[ 9].x = onet; uv[ 9].y =  0.5;
			uv[10].x = onet; uv[10].y =  1.0;
			uv[11].x =  0.0; uv[11].y =  1.0;

			// right (2)
			uv[12].x = onet; uv[12].y =  0.0;
			uv[13].x = twot; uv[13].y =  0.0;
			uv[14].x = twot; uv[14].y =  0.5;
			uv[15].x = onet; uv[15].y =  0.5;

			// top (5)
			uv[16].x = onet; uv[16].y =  0.5;
			uv[17].x = twot; uv[17].y =  0.5;
			uv[18].x = twot; uv[18].y =  1.0;
			uv[19].x = onet; uv[19].y =  1.0;

			// bottom (6)
			uv[20].x = twot; uv[20].y =  0.5;
			uv[21].x =  1.0; uv[21].y =  0.5;
			uv[22].x =  1.0; uv[22].y =  1.0;
			uv[23].x = twot; uv[23].y =  1.0;
		}

		// and our indices
		indices.resize(12 * 3);
		{
			PoolIntArray::Write i = indices.write();

			// front
			i[ 0] =  0; i[ 1] =  1; i[ 2] =  2;
			i[ 3] =  0; i[ 4] =  2; i[ 5] =  3;

			// back
			i[ 6] =  4; i[ 7] =  5; i[ 8] =  6;
			i[ 9] =  4; i[10] =  6; i[11] =  7;

			// left
			i[12] =  8; i[13] =  9; i[14] = 10;
			i[15] =  8; i[16] = 10; i[17] = 11;

			// right
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;

			// top
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;

			// bottom
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;
		}

		surface_arr[ArrayMesh::ARRAY_VERTEX] = vertices;
		surface_arr[ArrayMesh::ARRAY_NORMAL] = normals;
		surface_arr[ArrayMesh::ARRAY_TANGENT] = tangents;
		surface_arr[ArrayMesh::ARRAY_TEX_UV] = uvs;
		surface_arr[ArrayMesh::ARRAY_INDEX] = indices;
	}

	return updated;
}

int GDProcBox::get_input_connector_count() const {
	return 3;
}

Variant::Type GDProcBox::get_input_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
}

const String GDProcBox::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "width";
	} else if (p_slot == 1) {
		return "height";
	} else if (p_slot == 2) {
		return "depth";
	}

	return "";
}

const String GDProcBox::get_connector_property_name(int p_slot) const {
	if (p_slot == 0) {
		return "width";
	} else if (p_slot == 1) {
		return "height";
	} else if (p_slot == 2) {
		return "depth";
	}

	return "";
}

int GDProcBox::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcBox::get_output_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcBox::get_output_connector_name(int p_slot) const {
	return "surface";
}

const Variant GDProcBox::get_output(int p_slot) const {
	return Variant(surface_arr);
}
