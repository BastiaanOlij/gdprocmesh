#ifndef GD_PROC_INPOOLVECTORS_H
#define GD_PROC_INPOOLVECTORS_H

#include "gdprocnode.h"

namespace godot {

class GDProcInPoolVectors : public GDProcNode {
	GODOT_SUBCLASS(GDProcInPoolVectors, GDProcNode)

private:
	PoolVector3Array value;

public:
	static void _register_methods();

	virtual String get_type_name();

	void _init();

	virtual bool update(bool p_inputs_updated, const Array &p_inputs); // checks if our node has to be updated and if so, applies our calculations

	virtual Variant::Type get_input_property_type() const; // if this is an input property, what is its type?
	virtual void set_input(Variant p_input); // if this is an input property, set its value
	virtual Variant get_input(); // if this is an input property, get its value

	virtual int get_output_connector_count() const; // returns the number of output connectors this node has
	virtual Variant::Type get_output_connector_type(int p_slot) const; // returns the type of the data that is output by this output
	virtual const String get_output_connector_name(int p_slot) const; // returns the name for this output

	virtual const Variant get_output(int p_slot) const; // returns the output data itself

};

}

#endif /* !GD_PROC_INPOOLVECTORS_H */
