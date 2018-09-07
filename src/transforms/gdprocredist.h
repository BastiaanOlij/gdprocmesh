#ifndef GD_PROC_REDIST_H
#define GD_PROC_REDIST_H

#include "gdprocnode.h"

namespace godot {

class GDProcRedist : public GDProcNode {
	GODOT_SUBCLASS(GDProcRedist, GDProcNode)

private:
	// input
	float default_offset;
	float default_strafe;
	bool default_is_closed;
	bool default_follow;

	// output
	PoolVector3Array vectors;
	PoolColorArray rotations; // we don't have a PoolQuadArray so abuse Color

public:
	static void _register_methods();

	virtual String get_type_name();
	virtual String get_description() const;

	void _init();

	void set_is_closed(bool p_is_closed);
	bool get_is_closed() const;

	void set_follow(bool p_follow);
	bool get_follow() const;

	// properties
	void set_offset(float p_offset);
	float get_offset();

	void set_strafe(float p_strafe);
	float get_strafe();

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

#endif /* !GD_PROC_REDIST_H */
