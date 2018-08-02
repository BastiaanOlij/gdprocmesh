#ifndef GD_PROC_SURFACE_H
#define GD_PROC_SURFACE_H

#include "gdprocnode.h"

namespace godot {

class GDProcSurface : public GDProcNode {
	GODOT_SUBCLASS(GDProcSurface, GDProcNode)

private:
	Array surface_arr;

public:
	static void _register_methods();

	virtual String get_type_name();

	void _init();

	virtual bool update(bool p_inputs_updated, const Array &p_inputs);

	virtual int get_input_connector_count() const;
	virtual Variant::Type get_input_connector_type(int p_idx) const;
	virtual const String get_input_connector_name(int p_idx) const;

	// Here we make a special exception.
	// We don't declare our outputs because our user can't link up the output 
	// but we do need to access our final surface array.
	virtual int get_output_connector_count() const;
	virtual const Variant get_output(int p_idx) const;

};

}

#endif /* !GD_PROC_SURFACE_H */
