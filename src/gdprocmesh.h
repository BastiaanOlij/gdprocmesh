#ifndef GD_PROC_MESH_H
#define GD_PROC_MESH_H

#include <Godot.hpp>
#include <ArrayMesh.hpp>
#include <Material.hpp>

// yes, yuck, using std::map for now....
#include <map>

#include "gdprocnode.h"
#include "gdprocsurface.h"
#include "gdprocbox.h"

namespace godot {

class GDProcMesh : public ArrayMesh {
	GODOT_CLASS(GDProcMesh, ArrayMesh)

private:
	bool is_dirty;
	void trigger_update();

	std::map<int, Ref<GDProcNode> > nodes;
	int get_free_id();
	bool node_id_is_used(int p_id);
	bool do_update_node(int p_id, Ref<GDProcNode> p_node);

	// testing
	float size;

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
	int add_node(const Ref<GDProcNode> &p_node, int p_id = 0);

	/* old
	float get_size() const;
	void set_size(float new_size);
	*/

	GDProcMesh();	
	~GDProcMesh();
};

}

#endif /* !GD_PROC_MESH_H */
