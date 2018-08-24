#ifndef GD_PROC_EXEC_H
#define GD_PROC_EXEC_H

#include "gdprocnode.h"

namespace godot {

	class GDProcExec : public GDProcNode {
		GODOT_SUBCLASS(GDProcExec, GDProcNode)

	private:
		String default_expression;
		real_t default_a;
		real_t default_b;
		real_t default_c;
		real_t default_d;
		real_t default_e;
		PoolRealArray values;

	public:
		static void _register_methods();

		virtual String get_type_name();
		virtual String get_description() const;

		void _init();

		// properties
		void set_expression(String p_expression);
		String get_expression() const;

		void set_a(real_t a);
		real_t get_a() const;

		void set_b(real_t b);
		real_t get_b() const;

		void set_c(real_t c);
		real_t get_c() const;

		void set_d(real_t d);
		real_t get_d() const;

		void set_e(real_t e);
		real_t get_e() const;

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

#endif /* !GD_PROC_EXEC_H */
