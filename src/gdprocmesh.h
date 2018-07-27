#ifndef GD_PROC_MESH_H
#define GD_PROC_MESH_H

#include <Godot.hpp>
#include <ArrayMesh.hpp>

namespace godot {

class GDProcMesh : public ArrayMesh {
	GODOT_CLASS(GDProcMesh, ArrayMesh)

	bool is_dirty;

	float size;

public:
	static void _register_methods();
	
	void _init();
	void _update();

	float get_size() const;
	void set_size(float new_size);

	GDProcMesh();	
	~GDProcMesh();
};

}

#endif /* !GD_PROC_MESH_H */
