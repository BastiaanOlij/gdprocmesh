#include "gdprocnode.h"

using namespace godot;

////////////////////////////////////////////////////////////////////////////////////////
// Our base node

void GDProcNode::_register_methods() {
	// position
	register_method("get_position", &GDProcNode::get_position);
	register_method("set_position", &GDProcNode::set_position);
	register_property<GDProcNode, Vector2>("size", &GDProcNode::set_position, &GDProcNode::get_position, Vector2());

	// connectors
	register_method("get_input_connector_count", &GDProcNode::get_input_connector_count);
	register_method("get_input_connector_type", &GDProcNode::get_input_connector_type);
	register_method("get_input_connector_name", &GDProcNode::get_input_connector_name);

	register_method("get_output_connector_count", &GDProcNode::get_output_connector_count);
	register_method("get_output_connector_type", &GDProcNode::get_output_connector_type);
	register_method("get_output_connector_name", &GDProcNode::get_output_connector_name);

	// signal that our procmesh can subscribe to so we know things need to be regenerated
	register_signal<GDProcNode>((char *)"changed");
}

GDProcNode::ProcessStatus GDProcNode::get_status() const {
	return status;
}

void GDProcNode::set_status(GDProcNode::ProcessStatus p_status) {
	status = p_status;
}

void GDProcNode::_init() {
	must_update = true;
	position = Vector2(0.0, 0.0);
}

bool GDProcNode::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update;
	must_update = false;

	// nothing implemented here so just return false. This should become a template function
	return updated;
}

int GDProcNode::get_input_connector_count() const {
	return 0;
}

Variant::Type GDProcNode::get_input_connector_type(int p_idx) const {
	return Variant::NIL;
}

const String GDProcNode::get_input_connector_name(int p_idx) const {
	return "";
}

int GDProcNode::get_output_connector_count() const {
	// we should always have one output unless this is our final node
	return 1;
}

Variant::Type GDProcNode::get_output_connector_type(int p_idx) const {
	return Variant::NIL;
}

const String GDProcNode::get_output_connector_name(int p_idx) const {
	return "Default";
}

const Variant GDProcNode::get_output(int p_idx) const {
	return Variant();
}

Vector2 GDProcNode::get_position() const {
	return position;
}

void GDProcNode::set_position(Vector2 p_pos) {
	if (position != p_pos) {
		position = p_pos;

		// probably should send signal
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Surface (our final output)

void GDProcSurface::_register_methods() {

}

void GDProcSurface_init() {

}

bool GDProcSurface::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update;
	must_update = false;

	if (updated || p_inputs_updated) {
		printf("Updating our surface\n");

		// resize our array just in case
		surface_arr.resize(ArrayMesh::ARRAY_MAX);

		// prepare our array by copying the data from our inputs into our array.

		// for now we hardcode our cube ;)

		// define some variables
		float size = 2.0;
		
		PoolVector3Array vertices;
		PoolVector3Array normals;
		PoolIntArray indices;

		// prepare our data
		vertices.resize(8);
		{
			PoolVector3Array::Write w = vertices.write();
			Vector3 *v = w.ptr();
			float hs = size / 2.0f;

			v[0].x = -hs; v[0].y = -hs; v[0].z = -hs;
			v[1].x =  hs; v[1].y = -hs; v[1].z = -hs;
			v[2].x =  hs; v[2].y =  hs; v[2].z = -hs;
			v[3].x = -hs; v[3].y =  hs; v[3].z = -hs;

			v[4].x = -hs; v[4].y = -hs; v[4].z =  hs;
			v[5].x =  hs; v[5].y = -hs; v[5].z =  hs;
			v[6].x =  hs; v[6].y =  hs; v[6].z =  hs;
			v[7].x = -hs; v[7].y =  hs; v[7].z =  hs;
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

		// load up our array
		surface_arr[ArrayMesh::ARRAY_VERTEX] = Variant(vertices);
		surface_arr[ArrayMesh::ARRAY_NORMAL] = Variant(normals);
		surface_arr[ArrayMesh::ARRAY_INDEX] = Variant(indices);

	}

	return updated;
}

int GDProcSurface::get_input_connector_count() const {
	return ArrayMesh::ARRAY_MAX;
}

Variant::Type GDProcSurface::get_input_connector_type(int p_idx) const {
	switch (p_idx) {
		case ArrayMesh::ARRAY_VERTEX:
			return Variant::POOL_VECTOR3_ARRAY;
		case ArrayMesh::ARRAY_NORMAL:
			return Variant::POOL_VECTOR3_ARRAY;
		case ArrayMesh::ARRAY_TANGENT:
			return Variant::POOL_REAL_ARRAY;
		case ArrayMesh::ARRAY_COLOR:
			return Variant::POOL_COLOR_ARRAY;
		case ArrayMesh::ARRAY_TEX_UV:
			return Variant::POOL_VECTOR2_ARRAY;
		case ArrayMesh::ARRAY_TEX_UV2:
			return Variant::POOL_VECTOR2_ARRAY;
		case ArrayMesh::ARRAY_BONES:
			return Variant::POOL_REAL_ARRAY;
		case ArrayMesh::ARRAY_WEIGHTS:
			return Variant::POOL_REAL_ARRAY;
		case ArrayMesh::ARRAY_INDEX:
			return Variant::POOL_INT_ARRAY;
		default:
			return Variant::NIL;
	}
}

const String GDProcSurface::get_input_connector_name(int p_idx) const {
	switch (p_idx) {
		case ArrayMesh::ARRAY_VERTEX:
			return "Vertices";
		case ArrayMesh::ARRAY_NORMAL:
			return "Normals";
		case ArrayMesh::ARRAY_TANGENT:
			return "Tangents";
		case ArrayMesh::ARRAY_COLOR:
			return "Colors";
		case ArrayMesh::ARRAY_TEX_UV:
			return "UVs";
		case ArrayMesh::ARRAY_TEX_UV2:
			return "UV2s";
		case ArrayMesh::ARRAY_BONES:
			return "Bones";
		case ArrayMesh::ARRAY_WEIGHTS:
			return "Weights";
		case ArrayMesh::ARRAY_INDEX:
			return "Indices";
		default:
			return Variant::NIL;
	}

}

int GDProcSurface::get_output_connector_count() const {
	// We set this to zero even though we output our final array, we don't want this connector to show
	// and it means this will be treated as a final output node
	return 0;
}

const Variant GDProcSurface::get_output(int p_idx) const {
	// this should only be called after update is called. 
	if (p_idx == 0) {
		return Variant(surface_arr);
	}

	return Variant();
}

