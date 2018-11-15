#ifndef GD_PROC_OUTPUT_H
#define GD_PROC_OUTPUT_H

#include "gdprocnode.h"

namespace godot {

class GDProcOutput : public GDProcNode {
	GODOT_SUBCLASS(GDProcOutput, GDProcNode)

private:
	Array output_arr;

public:
	static void _register_methods();

	virtual String get_type_name();
	virtual String get_description() const;

	void _init();

	virtual bool update(bool p_inputs_updated, const Array &p_inputs);

	virtual int get_input_connector_count() const; // returns the number of input connectors this note has
	virtual Variant::Type get_input_connector_type(int p_slot) const; // returns the type of the data expected for this input
	virtual const String get_input_connector_name(int p_slot) const; // returns the name for this input

	// Here we make a special exception.
	// We don't declare our outputs because our user can't link up the output
	// but we do need to access our final surface array.
	virtual int get_output_connector_count() const;
	virtual const Variant get_output(int p_slot) const;
};

} // namespace godot

#endif /* !GD_PROC_OUTPUT_H */
