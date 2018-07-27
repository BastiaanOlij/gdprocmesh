#include "gdprocmesh.h"
#include <GlobalConstants.hpp>
#include <Dictionary.hpp>
#include <VisualServer.hpp>

using namespace godot;

/* Keeping this as example code, we're changing to _get_property_list, _get and _set because our properties will change

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

*/

Array GDProcMesh::_get_property_list() {
	Array arr;

	{
		Dictionary prop;
		prop["name"] = "size";
		prop["type"] = GlobalConstants::TYPE_REAL;
//		prop["hint"] = GlobalConstants::PROPERTY_HINT_XYZ;
//		prop["hint_string"] = "";
//		prop["usage"] = PROPERTY_USAGE_XYZ;

		arr.push_back(prop);
	}

/*
	{
		Dictionary prop;

		prop["name"] = "test";
		prop["type"] = GlobalConstants::TYPE_INT;

		arr.push_back(prop);
	}
*/

	// add a property for each node that we have
	std::map<int, Ref<GDProcNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++) {
		Dictionary prop;
		String name = "nodes/";
		name += String(it->first);

		prop["name"] = name;
		prop["type"] = GlobalConstants::TYPE_OBJECT;
		prop["hint"] = GlobalConstants::PROPERTY_HINT_RESOURCE_TYPE;
		prop["hint_string"] = "GDProcNode";
//		prop["usage"] = GlobalConstants::PROPERTY_USAGE_NOEDITOR | GlobalConstants::PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE

		arr.push_back(prop);		
	}


	return arr;
}

Variant GDProcMesh::_get(String p_name) {
	if (p_name == "size") {
		printf("get size\n");
		return Variant(size);
	}

	printf("get unknown: %s\n", p_name.utf8().get_data());
	return Variant();
}

bool GDProcMesh::_set(String p_name, Variant p_value) {
	if (p_name == "size") {
		size = p_value;
		is_dirty = true;
		call_deferred("_update");

		printf("set size to %0.2f\n", size);
		return true;
	}

	// Didn't handle it? exit!
	printf("set unknown: %s\n", p_name.utf8().get_data());
	return false;
}

int GDProcMesh::get_free_id() {
	int new_id = 1;

	// loop through our keys to find our highest + 1
	std::map<int, Ref<GDProcNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++) {
		if (it->first >= new_id) {
			new_id = it->first + 1;
		}
	}

	return new_id;
}

bool GDProcMesh::node_id_is_used(int p_id) {
	if (nodes.find(p_id) != nodes.end()) {
		return true;
	}

	return false;
}

void GDProcMesh::add_node(const Ref<GDProcNode> &p_node, int p_id) {
	if (p_id == 0) {
		// no id set? get an unused id
		p_id = get_free_id();
	} else if (node_id_is_used(p_id)) {
		// can't add this!
		return;
	}

	nodes[p_id] = p_node;
}

void GDProcMesh::_register_methods() {
	register_method("_update", &GDProcMesh::_update);

	/* get properties the more difficult way so we dynamically change the number of properties */
	register_method("_get_property_list", &GDProcMesh::_get_property_list);
	register_method("_get", &GDProcMesh::_get);
	register_method("_set", &GDProcMesh::_set);

	/* old register property setter and getter, easier but fixed number of properties
	register_method("get_size", &GDProcMesh::get_size);
	register_method("set_size", &GDProcMesh::set_size);
	register_property<GDProcMesh, float>("size", &GDProcMesh::set_size, &GDProcMesh::get_size, 1.0);
	*/
}

void GDProcMesh::_init() {
	printf("_init called\n");

	// set some defaults...
	size = 1.0;

	/* need to think of a way to skip the first update if this is an existing node as our resulting mesh will be cached thanks to using ArrayMesh */
	is_dirty = true;
	call_deferred("_update");
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

	String name = "Default";
	Ref<Material> material;

	// clear meshes in reverse order
	for (int64_t s = get_surface_count() - 1; s >= 0; s--) {
		if (s==0) {
			name = surface_get_name(s);
			material = surface_get_material(s);
		}
		surface_remove(s);
	}

	// lets add a new surface
	add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arr);
	surface_set_name(0, name);
	surface_set_material(0, material);

	// and we're good
	is_dirty = false;
}

GDProcMesh::GDProcMesh() {
	printf("GDProcMesh called\n");
}

GDProcMesh::~GDProcMesh() {
	// do we need to clean up our map?

	printf("~GDProcMesh called\n");
}
