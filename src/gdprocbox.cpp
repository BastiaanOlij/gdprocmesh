#include "gdprocbox.h"

using namespace godot;

void GDProcBox::_register_methods() {

}

String GDProcBox::get_type_name() {
	return String("Box");
}

void GDProcBox::_init() {
	// first call super class
	GDProcNode::_init();
}

bool GDProcBox::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update;
	must_update = false;

	if (updated || p_inputs_updated) {
		printf("Updating box\n");
		Vector3 size = Vector3(1.0, 1.0, 1.0);

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::VECTOR3) {
				size = p_inputs[0];
			}
		}

		// prepare our data
		vertices.resize(8);
		{
			PoolVector3Array::Write w = vertices.write();
			Vector3 *v = w.ptr();
			float hsx = size.x / 2.0f;
			float hsy = size.y / 2.0f;
			float hsz = size.z / 2.0f;

			v[0].x = -hsx; v[0].y = -hsy; v[0].z = -hsz;
			v[1].x =  hsx; v[1].y = -hsy; v[1].z = -hsz;
			v[2].x =  hsx; v[2].y =  hsy; v[2].z = -hsz;
			v[3].x = -hsx; v[3].y =  hsy; v[3].z = -hsz;

			v[4].x = -hsx; v[4].y = -hsy; v[4].z =  hsz;
			v[5].x =  hsx; v[5].y = -hsy; v[5].z =  hsz;
			v[6].x =  hsx; v[6].y =  hsy; v[6].z =  hsz;
			v[7].x = -hsx; v[7].y =  hsy; v[7].z =  hsz;
		}

		// prepare our data
		normals.resize(8);
		{
			PoolVector3Array::Write w = normals.write();
			Vector3 *n = w.ptr();

			// this will look ugly but we're just testing..
			n[0].x = -1.0; n[0].y = -1.0; n[0].z = -1.0;
			n[1].x =  1.0; n[1].y = -1.0; n[1].z = -1.0;
			n[2].x =  1.0; n[2].y =  1.0; n[2].z = -1.0;
			n[3].x = -1.0; n[3].y =  1.0; n[3].z = -1.0;

			n[4].x = -1.0; n[4].y = -1.0; n[4].z =  1.0;
			n[5].x =  1.0; n[5].y = -1.0; n[5].z =  1.0;
			n[6].x =  1.0; n[6].y =  1.0; n[6].z =  1.0;
			n[7].x = -1.0; n[7].y =  1.0; n[7].z =  1.0;

			for (int i = 0; i < 8; i++) {
				n[i].normalize();
			}
		}

		indices.resize(12 * 3);
		{
			PoolIntArray::Write w = indices.write();
			int *i = w.ptr();

			// front
			i[ 0] = 0; i[ 1] = 1; i[ 2] = 3;
			i[ 3] = 1; i[ 4] = 2; i[ 5] = 3;

			// back
			i[ 6] = 5; i[ 7] = 4; i[ 8] = 6;
			i[ 9] = 4; i[10] = 7; i[11] = 6;

			// left
			i[12] = 4; i[13] = 0; i[14] = 3;
			i[15] = 4; i[16] = 3; i[17] = 7;

			// right
			i[18] = 1; i[19] = 5; i[20] = 6;
			i[21] = 1; i[22] = 6; i[23] = 2;

			// top
			i[24] = 4; i[25] = 5; i[26] = 1;
			i[27] = 4; i[28] = 1; i[29] = 0;

			// bottom
			i[30] = 3; i[31] = 2; i[32] = 6;
			i[33] = 3; i[34] = 6; i[35] = 7;
		}		

	}

	return updated;
}

int GDProcBox::get_input_connector_count() const {
	return 1;
}

Variant::Type GDProcBox::get_input_connector_type(int p_slot) const {
	return Variant::VECTOR3;
}

const String GDProcBox::get_input_connector_name(int p_slot) const {
	return "size";
}

int GDProcBox::get_output_connector_count() const {
	return 3;
}

Variant::Type GDProcBox::get_output_connector_type(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant::POOL_VECTOR3_ARRAY;
		case 1:
			return Variant::POOL_VECTOR3_ARRAY;
		case 2:
			return Variant::POOL_INT_ARRAY;
		default:
			return Variant::NIL;		
	}
}

const String GDProcBox::get_output_connector_name(int p_slot) const {
	switch (p_slot) {
		case 0:
			return "Vertices";
		case 1:
			return "Normals";
		case 2:
			return "Indices";
		default:
			return "???";
	}
}

const Variant GDProcBox::get_output(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant(vertices);
		case 1:
			return Variant(normals);
		case 2:
			return Variant(indices);
		default:
			return Variant();
	}
}
