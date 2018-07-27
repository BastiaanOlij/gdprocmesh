#include "gdprocmesh.h"
#include <VisualServer.hpp>

using namespace godot;

void GDProcMesh::_register_methods() {
	register_method("_update", &GDProcMesh::_update);

	/* register property setter and getter */
	register_method("get_size", &GDProcMesh::get_size);
	register_method("set_size", &GDProcMesh::set_size);
	register_property<GDProcMesh, float>("size", &GDProcMesh::set_size, &GDProcMesh::get_size, 1.0);
}

void GDProcMesh::_init() {
	printf("_init called\n");

	/* need to think of a way to skip the first update if this is an existing node as our resulting mesh will be cached thanks to using ArrayMesh */
	is_dirty = true;
	call_deferred("_update");
}

float GDProcMesh::get_size() const {
	return size;
}

void GDProcMesh::set_size(float new_size) {
	printf("Changing size from %0.2f to %0.2f\n", size, new_size);
	if (size != new_size) {
		size = new_size;
		is_dirty = true;
		call_deferred("_update");
	}
}

void GDProcMesh::_update() {
	// if we've already updated this, exit....
	if (!is_dirty) {
		printf("Update called but not dirty\n");
		return;
	}

	// just testing with a cube for now
	printf("Update called, updating mesh\n");

	// define some variables
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
	Array arr;
	arr.resize(ArrayMesh::ARRAY_MAX);

	arr[ArrayMesh::ARRAY_VERTEX] = Variant(vertices);
	arr[ArrayMesh::ARRAY_NORMAL] = Variant(normals);
	arr[ArrayMesh::ARRAY_INDEX] = Variant(indices);

	// clear meshes in reverse order
	for (int64_t s = get_surface_count() - 1; s >= 0; s--) {
		surface_remove(s);
	}

	// lets add a new surface
	add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arr);

	// and we're good
	is_dirty = false;
}

GDProcMesh::GDProcMesh() {
	printf("GDProcMesh called\n");

	size = 1.0;
}

GDProcMesh::~GDProcMesh() {
	printf("~GDProcMesh called\n");
}
