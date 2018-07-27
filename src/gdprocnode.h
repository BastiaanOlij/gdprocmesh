#ifndef GD_PROC_NODE_H
#define GD_PROC_NODE_H

#include <Godot.hpp>
#include <Resource.hpp>

namespace godot {

class GDProcNode : public Resource {
	GODOT_CLASS(GDProcNode, Resource)

private:
	Vector2	position;

public:
	static void _register_methods();

	void _init();

	Vector2 get_position() const;
	void set_position(Vector2 p_pos);
};

}

#endif /* !GD_PROC_NODE_H */
