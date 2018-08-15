#ifndef GD_PROC_VECTOR_H
#define GD_PROC_VECTOR_H

#include "gdprocnode.h"

namespace godot {

class GDProcVector : public GDProcNode {
	GODOT_SUBCLASS(GDProcVector, GDProcNode)

private:
	Vector3 defaults;
	PoolVector3Array value;

public:
	static void _register_methods();

	virtual String get_type_name();

	void _init();

	// properties
	void set_x(float x);
	float get_x() const;

	void set_y(float y);
	float get_y() const;

	void set_z(float z);
	float get_z() const;

	virtual bool update(bool p_inputs_updated, const Array &p_inputs); // checks if our node has to be updated and if so, applies our calculations

	virtual int get_input_connector_count() const; // returns the number of input connectors this note has
	virtual Variant::Type get_input_connector_type(int p_slot) const; // returns the type of the data expected for this input
	virtual const String get_input_connector_name(int p_slot) const; // returns the name for this input

	virtual const String get_connector_property_name(int p_slot) const; // if we want an editable field for this slot, returns the name of the related property

	virtual int get_output_connector_count() const; // returns the number of output connectors this node has
	virtual Variant::Type get_output_connector_type(int p_slot) const; // returns the type of the data that is output by this output
	virtual const String get_output_connector_name(int p_slot) const; // returns the name for this output

	virtual const Variant get_output(int p_slot) const; // returns the output data itself

};

}

#endif /* !GD_PROC_VECTOR_H */
