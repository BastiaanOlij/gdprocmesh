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
	return 2;
}

Variant::Type GDProcBox::get_output_connector_type(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant::POOL_VECTOR3_ARRAY;
		case 1:
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
			return Variant(indices);
		default:
			return Variant();
	}
}
