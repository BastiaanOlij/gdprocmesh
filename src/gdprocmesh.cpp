#include "gdprocmesh.h"
#include <GlobalConstants.hpp>
#include <Dictionary.hpp>
#include <VisualServer.hpp>

using namespace godot;

Array GDProcMesh::_get_property_list() {
	Array arr;

	// add a property for each node that we have
	std::map<int, Ref<GDProcNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++) {
		Dictionary prop;
		String name = "nodes/";
		name += String::num_int64(it->first);

		prop["name"] = name;
		prop["type"] = GlobalConstants::TYPE_OBJECT;
		prop["hint"] = GlobalConstants::PROPERTY_HINT_RESOURCE_TYPE;
		prop["hint_string"] = "GDProcNode";
		prop["usage"] = GlobalConstants::PROPERTY_USAGE_NOEDITOR;
		// PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE is new and not yet in gdnative..
//		prop["usage"] = GlobalConstants::PROPERTY_USAGE_NOEDITOR | GlobalConstants::PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE

		arr.push_back(prop);		
	}

	// now add a property for our connection
	{
		Dictionary prop;
		prop["name"] = "graph/connections";
		prop["type"] = GlobalConstants::TYPE_INT_ARRAY;
//		prop["hint"] = GlobalConstants::PROPERTY_HINT_XYZ;
//		prop["hint_string"] = "";
		prop["usage"] = GlobalConstants::PROPERTY_USAGE_NOEDITOR;
		// PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE is new and not yet in gdnative..
//		prop["usage"] = GlobalConstants::PROPERTY_USAGE_NOEDITOR | GlobalConstants::PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE

		arr.push_back(prop);
	}


	// now add properties for any inputs

	return arr;
}

Variant GDProcMesh::_get(String p_name) {
	String str_nodes = String("nodes/"); // temporary workaround to get around compile issue on Linux, needs further research
	if (p_name.begins_with(str_nodes)) {
		String index = p_name.split('/')[1];
		int id = (int) index.to_int();
		
		std::map<int, Ref<GDProcNode> >::iterator it = nodes.find(id);
		if (it == nodes.end()) {
			printf("Couldn't find node %i\n", id);
			return Variant();
		} else {
			return Variant(it->second);
		}
	} else if (p_name == "graph/connections") {
		PoolIntArray ret;

		ret.resize((int) connections.size() * 4);
		{
			PoolIntArray::Write w = ret.write();
			int *i = w.ptr();
			int p = 0;

			// add our connection data here
			std::vector<connection>::iterator it;
			for (it = connections.begin(); it != connections.end(); it++) {
				i[p++] = it->input.node;
				i[p++] = it->input.connector;
				i[p++] = it->output.node;
				i[p++] = it->output.connector;
			}
		}

		return Variant(ret);
	}

	// Must be a property of our superclass, returning an empty (NIL) variant will handle it further
	return Variant();
}

bool GDProcMesh::_set(String p_name, Variant p_value) {
	String str_nodes = String("nodes/"); // temporary workaround to get around compile issue on Linux, needs further research
	if (p_name.begins_with(str_nodes)) {
		String index = p_name.split('/')[1];
		int id = (int) index.to_int();

		// printf("Loading node %i from scene\n", id);
		add_node(p_value, id);

		return true;
	} else if (p_name == "graph/connections") {
		PoolIntArray data = p_value;
		int count = data.size();

		for(int i = 0; i < count; i+=4) {
			add_connection(data[i], data[i+1], data[i+2], data[i+3]);
		}

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

int GDProcMesh::add_node(const Ref<GDProcNode> &p_node, int p_id) {
	if (!p_node.is_valid()) {
		printf("Invalid node passed to add_node\n");
		return -1;
	} else if (p_id == 0) {
		// no id set? get an unused id
		p_id = get_free_id();
	} else if (node_id_is_used(p_id)) {
		// can't add this!
		return -1;
	}
	printf("Adding node with id %i\n", p_id);

	// trigger our trigger_update on a change of this node (commented out because I have a weird const error)
	printf("Connecting changed trigger update\n");
	Ref<GDProcNode> r = p_node;
	r->connect("changed", this, "trigger_update");

	// Add our node
	printf("Assignig node to array\n");
	nodes[p_id] = p_node;

	// trigger update
	printf("Trigger update\n");
	trigger_update();

	// return the ID that we ended up using
	return p_id;
}

int GDProcMesh::find_node(const Ref<GDProcNode> &p_node) {
	std::map<int, Ref<GDProcNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++) {
		// note, == on Ref<> compares internal pointer :)
		if (it->second == p_node) {
			return it->first;
		}
	}

	return -1;
}

Ref<GDProcNode> GDProcMesh::get_node(int p_id) {
	std::map<int, Ref<GDProcNode> >::iterator it = nodes.find(p_id);
	if (it == nodes.end()) {
		return Ref<GDProcNode>();
	} else {
		return it->second;
	}
}

Array GDProcMesh::get_node_id_list() {
	Array arr;

	std::map<int, Ref<GDProcNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++) {
		arr.push_back(Variant(it->first));
	}

	return arr;
}

void GDProcMesh::remove_node(int p_id) {
	size_t max = connections.size(); // size_t is unsigned so will wrap around!!
	if (max != 0) {
		// first remove any connector related to this
		for (size_t i = max - 1; i >= 0 && i < max; i--) {
			if ((connections[i].input.node == p_id) || (connections[i].output.node == p_id)) {
				connections.erase(connections.begin() + i);
			}
		}
	}

	nodes[p_id]->disconnect("changed", this, "trigger_update");

	// now remove our node
	nodes.erase(p_id);

	// trigger update
	trigger_update();
}

void GDProcMesh::add_connection(int p_input_node, int p_input_connector, int p_output_node, int p_output_connector) {
	// does our input node exist?
	if (!node_id_is_used(p_input_node)) {
		printf("Unknown input node %i\n", p_input_node);
		return;
	}

	// does our output node exist?
	if (!node_id_is_used(p_output_node)) {
		printf("Unknown output node %i\n", p_output_node);
		return;
	}

	///@TODO we should add a check here to see if the type of the output node matches the type of the input node

	// first remove any existing connection on our input connector
	remove_connection(p_input_node, p_input_connector);

	// now add a new one
	connections.push_back(connection(p_input_node, p_input_connector, p_output_node, p_output_connector));

	// trigger an update
	trigger_update();
}

void GDProcMesh::remove_connection(int p_input_node, int p_input_connector) {
	size_t max = connections.size(); // size_t is unsigned so will wrap around!!
	if (max == 0) {
		return;
	}

	// there should be only one but better safe then sorry..
	for (size_t i = max - 1; i >= 0 && i < max; i--) {
		if ((connections[i].input.node == p_input_node) && (connections[i].input.connector == p_input_connector)) {
			connections.erase(connections.begin() + i);

			trigger_update();
		}
	}
}

Array GDProcMesh::get_connection_list() {
	Array arr;

	std::vector<connection>::iterator it;
	for (it = connections.begin(); it != connections.end(); it++) {
		Array row;

		row.push_back(it->input.node);
		row.push_back(it->input.connector);
		row.push_back(it->output.node);
		row.push_back(it->output.connector);

		arr.push_back(row);
	}

	return arr;
}

const GDProcMesh::ctor GDProcMesh::get_output_for_input(int p_input_node, int p_input_connector) const {
	size_t max = connections.size(); // size_t is unsigned so will wrap around!!
	if (max == 0) {
		return ctor(-1, 0);
	}

	for (size_t i = max - 1; i >= 0 && i < max; i--) {
		connection c = connections[i];
		if ((c.input.node == p_input_node) && (c.input.connector == p_input_connector)) {
			return c.output;
		}
	}

	return ctor(-1, 0);
}

void GDProcMesh::_register_methods() {
	/* init and update */
	register_method("_post_init", &GDProcMesh::_post_init);
	register_method("_update", &GDProcMesh::_update);
	register_method("trigger_update", &GDProcMesh::trigger_update);

	/* nodes */
	register_method("get_free_id", &GDProcMesh::get_free_id);
	register_method("add_node", &GDProcMesh::add_node);
	register_method("find_node", &GDProcMesh::find_node);
	register_method("get_node", &GDProcMesh::get_node);
	register_method("get_node_id_list", &GDProcMesh::get_node_id_list);
	register_method("remove_node", &GDProcMesh::remove_node);

	/* should add some methods to interact with nodes directly */

	/* connections */
	register_method("add_connection", &GDProcMesh::add_connection);
	register_method("remove_connection", &GDProcMesh::remove_connection);
	register_method("get_connection_list", &GDProcMesh::get_connection_list);

	/* get properties the more difficult way so we dynamically change the number of properties */
	register_method("_get_property_list", &GDProcMesh::_get_property_list);
	register_method("_get", &GDProcMesh::_get);
	register_method("_set", &GDProcMesh::_set);
}

void GDProcMesh::trigger_update() {
	if (!is_dirty) {
		is_dirty = true;
		call_deferred("_update");
	}
}

void GDProcMesh::_init() {
	// prevent deferred updates until we're ready
	is_dirty = true;

	// this should delay running _post_init until after all our data has been loaded
	call_deferred("_post_init");
}

void GDProcMesh::_post_init() {
	// Set dirty as false even though we haven't called update.
	// This means that if we've loaded a graph and our
	// previously generated mesh, which is saved in the scene,
	// we skip a bunch of expensive recalculating.
	is_dirty = false;

	if (nodes.size() == 0) {
		// we have no nodes, so create our defaults...

		// create our surface, keep this as the first
		Ref<GDProcSurface> surface;
		surface.instance();
		surface->set_position(Vector2(500.0, 50.0));
		int surface_id = add_node(surface);

		// create a box
		Ref<GDProcBox> box;
		box.instance();
		box->set_position(Vector2(250.0, 50.0));
		int box_id = add_node(box);

		// create a box
		Ref<GDProcVector> vector;
		vector.instance();
		vector->set_position(Vector2(10.0, 50.0));
		vector->set_x(1.0);
		vector->set_y(1.0);
		vector->set_z(1.0);
		int vector_id = add_node(vector);

		// add our connections
		add_connection(surface_id, 0, box_id, 0); // vertices
		add_connection(surface_id, 1, box_id, 1); // normals
		add_connection(surface_id, 8, box_id, 2); // indices

		add_connection(box_id, 0, vector_id, 0); // size

		// note that this will have trigger an update...
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
				ctor c = get_output_for_input(p_id, i);

				if (c.node == -1 ){
					printf("Node %i, Connector %i is not connected\n", p_id, i);
					// if not, just add a NIL input
					inputs.push_back(Variant());
				} else {
					// make sure this is a valid node
					std::map<int, Ref<GDProcNode> >::iterator it = nodes.find(c.node);
					if (it == nodes.end()) {
						printf("Unknown node %i\n", c.node);
						inputs.push_back(Variant());
					} else {
						Ref<GDProcNode> output_node = it->second;

						// lets make sure this has been updated (if we already did then that is fine)
						if (do_update_node(c.node, output_node)) {
							updated = true;
						}

						// and get our output
						inputs.push_back(output_node->get_output(c.connector));
					}
				}
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
		// printf("Update called but not dirty\n");
		return;
	}

	// if we change any surface we turn this to true and check if we need to do any post processing.
	bool has_changed = false; 

	// clear surfaces we'll no longer be needing
	for (int64_t s = get_surface_count() - 1; s >= 0; s--) {
		String name = surface_get_name(s);

		String str_surface = String("Surface_"); // temporary workaround to get around compile issue on Linux, needs further research
		if (name.begins_with(str_surface)) {
			String index = name.split('_')[1];
			int id = (int) index.to_int();

			std::map<int, Ref<GDProcNode> >::iterator it = nodes.find(id);
			if (it == nodes.end()) {
				// node has been removed
				// printf("Removing unused surface %s\n", name.utf8().get_data());
				surface_remove(s);
			} else if (it->second->get_output_connector_count() != 0) {
				// This is not a final node.
				// printf("Removing unused surface %s\n", name.utf8().get_data());
				surface_remove(s);
			}
		} else {
			// not one of ours?!
			// printf("Removing unused surface %s\n", name.utf8().get_data());
			surface_remove(s);
		}
	}

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
				String name = "Surface_";
				name += String::num_int64(it->first);
				Ref<Material> material;

				// find our surface and get some info we may want to cache like our material
				int64_t s = surface_find_by_name(name);
				if (s != -1) {
					// printf("Removing changed surface %s\n", name.utf8().get_data());

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

					if (arr.size() == ArrayMesh::ARRAY_MAX) {
						// log
						// printf("Updating surface %s\n", name.utf8().get_data());

						// lets add a new surface
						int64_t new_surface_id = get_surface_count();
						add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arr);

						surface_set_name(new_surface_id, name);

						if (material.is_valid()) {
							surface_set_material(new_surface_id, material);								
						}
					} else {
						printf("Final node is not returning a correctly sized array\n");	
					}
				} else {
					printf("Final node is not returning an array \n");
				}
			}
		}
	}

	// and we're good
	is_dirty = false;
}

GDProcMesh::GDProcMesh() {
	// nothing to do here yet
}

GDProcMesh::~GDProcMesh() {
	// do we need to clean up our map?
}
