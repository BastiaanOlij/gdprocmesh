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

/*
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
*/

	return arr;
}

Variant GDProcMesh::_get(String p_name) {
	if (p_name == "size") {
		printf("get size\n");
		return Variant(size);
	}

	// Must be a property of our superclass, returning an empty (NIL) variant will handle it further
	return Variant();
}

bool GDProcMesh::_set(String p_name, Variant p_value) {
	if (p_name == "size") {
		size = p_value;
		trigger_update();

		printf("set size to %0.2f\n", size);
		return true;
	}

	// Must be a property of our superclass, returning false will handle it further
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

	// trigger our trigger_update on a change of this node (commented out because I have a weird const error)
//	GDProcNode * node = p_node.ptr();
//	node->connect("changed", this, "trigger_update");

	nodes[p_id] = p_node;

	// trigger update
	trigger_update();
}

void GDProcMesh::_register_methods() {
	register_method("_post_init", &GDProcMesh::_post_init);
	register_method("_update", &GDProcMesh::_update);
	register_method("trigger_update", &GDProcMesh::trigger_update);

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

void GDProcMesh::trigger_update() {
	if (!is_dirty) {
		is_dirty = true;
		call_deferred("_update");
	}
}

void GDProcMesh::_init() {
	printf("_init called\n");

	// set some defaults...
	size = 1.0;

	// prevent deferred updates until we're ready
	is_dirty = true;

	// this should delay running _post_init until after all our data has been loaded
	call_deferred("_post_init");
}

void GDProcMesh::_post_init() {
	printf("_post_init called\n");
	is_dirty = false;

	if (nodes.size() == 0) {
		// we have no nodes, so create our defaults...
//		Ref<GDProcNode> surface;
		Ref<GDProcSurface> surface;

		printf("Creating instance\n");
		surface.instance();

		printf("Adding instance\n");
		add_node(surface);
	}
}

bool GDProcMesh::do_update_node(int p_id, Ref<GDProcNode> p_node) {
	switch (p_node->get_status()) {
		case GDProcNode::PROCESS_STATUS_INPROGRESS: {
			// this is bad, we have a circular dependency
			printf("Found circular dependency in procedural mesh!\n");
			return false;
		}; break;
		case GDProcNode::PROCESS_STATUS_UNCHANGED: {
			// already processed and found it to be unchanged
			return false;
		}; break;
		case GDProcNode::PROCESS_STATUS_CHANGED: {
			// already processed and found it to be changed
			return true;
		}; break;
		default: {
			bool updated = false;
			Array inputs;

			// set node as being in progress
			p_node->set_status(GDProcNode::PROCESS_STATUS_INPROGRESS);

			// check all our inputs
			for (int i = 0; i < p_node->get_input_connector_count(); i++) {
				// find if this has been connected

				// if so we need to call its update
				// if do_update_node(input_id, input_node) {
				//	updated = true
				// }

				// if not, just add a NIL input
				inputs.push_back(Variant());
			}

			// update this node
			if (p_node->update(updated, inputs)) {
				updated = true;
			}

			// set node as processed with our result status..
			p_node->set_status(updated ? GDProcNode::PROCESS_STATUS_CHANGED : GDProcNode::PROCESS_STATUS_UNCHANGED);

			return updated;
		}; break;
	}
}

void GDProcMesh::_update() {
	// if we've already updated this, exit....
	if (!is_dirty) {
		printf("Update called but not dirty\n");
		return;
	}

	printf("Update called\n");

	// if we change any surface we turn this to true and check if we need to do any post processing.
	bool has_changed = false; 

	// Reset our process status, we want to check each node atleast once...
	std::map<int, Ref<GDProcNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++) {
		it->second->set_status(GDProcNode::PROCESS_STATUS_PENDING);
	}

	// now find all our final nodes
	for (it = nodes.begin(); it != nodes.end(); it++) {
		if (it->second->get_output_connector_count() == 0) {
			// trigger updating this node
			if (do_update_node(it->first, it->second)) {
				String name = "Surface_1";
				Ref<Material> material;

				// find our surface and get some info we may want to cache like our material
				int64_t s = surface_find_by_name(name);
				if (s != -1) {
					// remember our material, we're reusing it!
					material = surface_get_material(s);

					// clear our surface
					surface_remove(s);
				}

				// get our new surface
				Variant surface = it->second->get_output(0);

				// check if this is a valid array and update!
				if (surface.get_type() == Variant::ARRAY) {
					Array arr = surface;

					// lets add a new surface
					add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arr);
					surface_set_name(0, name);
					surface_set_material(0, material);
				}
			}
		}
	}

	// clear left overs (we need to improve on this)
	for (int64_t s = get_surface_count() - 1; s >= 0; s--) {
		if (surface_get_name(s) != "Surface_1") {
			surface_remove(s);
		}
	}

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
