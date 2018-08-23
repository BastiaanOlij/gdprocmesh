#ifndef GD_PROC_ADD_H
#define GD_PROC_ADD_H

#include "gdprocnode.h"

namespace godot {

class GDProcAdd : public GDProcNode {
	GODOT_SUBCLASS(GDProcAdd, GDProcNode)

private:
	float default_add;
	PoolRealArray values;

public:
	static void _register_methods();

	virtual String get_type_name();
	virtual String get_description() const;

	void _init();

	// properties
	void set_add(float p_add);
	float get_add();

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

#endif /* !GD_PROC_ADD_H */
