#ifndef GD_PROC_BOX_H
#define GD_PROC_BOX_H

#include "gdprocnode.h"

namespace godot {

class GDProcBox : public GDProcNode {
	GODOT_SUBCLASS(GDProcBox, GDProcNode)

private:
	Vector3 size;

	// should add a smooth option?

	PoolVector3Array vertices;
	PoolVector3Array normals; // might decide to remove normals
	PoolIntArray indices;

public:
	static void _register_methods();

	virtual String get_type_name();

	void _init();

	virtual bool update(bool p_inputs_updated, const Array &p_inputs);

	// properties
	void set_size(Vector3 p_size);
	Vector3 get_size() const;

	// need to add input(s) to overrule size (and smooth)

	virtual int get_output_connector_count() const;
	virtual Variant::Type get_output_connector_type(int p_idx) const;
	virtual const String get_output_connector_name(int p_idx) const;
	virtual const Variant get_output(int p_idx) const;

};

}

#endif /* !GD_PROC_BOX_H */
