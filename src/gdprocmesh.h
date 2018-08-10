#ifndef GD_PROC_MESH_H
#define GD_PROC_MESH_H

#include <Godot.hpp>
#include <ArrayMesh.hpp>
#include <Material.hpp>

// yes using std C++
#include <vector>
#include <map>

#include "gdprocnode.h"
#include "output/gdprocoutput.h"
#include "output/gdprocsurface.h"
#include "transforms/gdprocgennormals.h"
#include "shapes/gdprocbox.h"

namespace godot {

class GDProcMesh : public ArrayMesh {
	GODOT_CLASS(GDProcMesh, ArrayMesh)

private:
	bool is_dirty;
	void trigger_update();

	std::map<int, Ref<GDProcNode> > nodes;
	int get_free_id();
	bool node_id_is_used(int p_id) const;
	bool node_name_used(const String p_name);
	const String get_unique_node_name(const String p_base_name);
	void _child_name_changed(Ref<GDProcNode> p_child, String p_from, String p_to);

	bool do_update_node(int p_id, Ref<GDProcNode> p_node);

	struct ctor {
		int node;
		int connector;

		ctor() {
			node = -1;
			connector = 0;
		}

		ctor(int p_node, int p_connector) {
			node = p_node;
			connector = p_connector;
		}
	};

	struct connection {
		ctor input;
		ctor output;

		connection(int p_input_node, int p_input_connector, int p_output_node, int p_output_connector) {
			input.node = p_input_node;
			input.connector = p_input_connector;
			output.node = p_output_node;
			output.connector = p_output_connector;
		}
	};

	std::vector<connection> connections;
	const ctor get_output_for_input(int p_input_node, int p_input_connector) const;

public:
	static void _register_methods();
	
	void _init();
	void _post_init();
	void _update();

	/* properties */
	Array _get_property_list();
	Variant _get(String p_name);
	bool _set(String p_name, Variant p_value);

	/* nodes */
	int add_node(Ref<GDProcNode> p_node, int p_id = 0);
	int find_node(Ref<GDProcNode> p_node);
	Ref<GDProcNode> get_node(int p_id);
	Array get_node_id_list();
	void remove_node(int p_id);

	/* connections */
	void add_connection(int p_input_node, int p_input_connector, int p_output_node, int p_output_connector);
	void remove_connection(int p_input_node, int p_input_connector);
	Array get_connection_list();

	/* old
	float get_size() const;
	void set_size(float new_size);
	*/

	GDProcMesh();	
	~GDProcMesh();
};

}

#endif /* !GD_PROC_MESH_H */
