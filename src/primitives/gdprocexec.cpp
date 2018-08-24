#include "gdprocexec.h"
#include <Expression.hpp>

using namespace godot;

void GDProcExec::_register_methods() {
	register_property<GDProcExec, String>("expression", &GDProcExec::set_expression, &GDProcExec::get_expression, "");
	register_property<GDProcExec, real_t>("a", &GDProcExec::set_a, &GDProcExec::get_a, 0.0);
	register_property<GDProcExec, real_t>("b", &GDProcExec::set_b, &GDProcExec::get_b, 0.0);
	register_property<GDProcExec, real_t>("c", &GDProcExec::set_c, &GDProcExec::get_c, 0.0);
	register_property<GDProcExec, real_t>("d", &GDProcExec::set_d, &GDProcExec::get_d, 0.0);
	register_property<GDProcExec, real_t>("e", &GDProcExec::set_e, &GDProcExec::get_e, 0.0);
}

String GDProcExec::get_type_name() {
	return "Execute Expression";
}

String GDProcExec::get_description() const {
	return "Executes an expression against multiple reals.\nUse variables 'a', 'b', 'c', 'd' and 'e' to access the specific array.\nUse 'i' to get the index in the array.";
}

void GDProcExec::_init() {
	// first call super class
	GDProcNode::_init();

	default_expression = "";
	default_a = 0.0;
	default_b = 0.0;
	default_c = 0.0;
	default_d = 0.0;
	default_e = 0.0;
}

void GDProcExec::set_expression(String p_expression) {
	if (p_expression != default_expression) {
		default_expression = p_expression;
		must_update = true;
		emit_signal("changed");
	}
}

String GDProcExec::get_expression() const {
	return default_expression;
}

void godot::GDProcExec::set_a(real_t a) {
	if (default_a != a) {
		default_a = a;
		must_update = true;
		emit_signal("changed");
	}
}

real_t godot::GDProcExec::get_a() const {
	return default_a;
}

void godot::GDProcExec::set_b(real_t b) {
	if (default_b != b) {
		default_b = b;
		must_update = true;
		emit_signal("changed");
	}
}

real_t godot::GDProcExec::get_b() const {
	return default_b;
}

void godot::GDProcExec::set_c(real_t c) {
	if (default_c != c) {
		default_c = c;
		must_update = true;
		emit_signal("changed");
	}
}

real_t godot::GDProcExec::get_c() const {
	return default_c;
}

void godot::GDProcExec::set_d(real_t d) {
	if (default_d != d) {
		default_d = d;
		must_update = true;
		emit_signal("changed");
	}
}

real_t godot::GDProcExec::get_d() const {
	return default_d;
}

void godot::GDProcExec::set_e(real_t e) {
	if (default_e != e) {
		default_e = e;
		must_update = true;
		emit_signal("changed");
	}
}

real_t godot::GDProcExec::get_e() const {
	return default_e;
}

bool GDProcExec::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		String expression = default_expression;
		int num_a;
		PoolRealArray a;
		int num_b;
		PoolRealArray b;
		int num_c;
		PoolRealArray c;
		int num_d;
		PoolRealArray d;
		int num_e;
		PoolRealArray e;

		int input_count = p_inputs.size();
		if (input_count > 0 && p_inputs[0].get_type() == Variant::STRING) {
			expression = p_inputs[0];
		}

		if (expression == String()) {
			// No expression. Leave output unmodified
			Godot::print_warning(String("Node '{0}' failed to parse expression. Output will be unmodified.\nError Message: No expression found.")
				.format(Array::make(get_node_name())), __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		if (input_count > 1 && p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY && ((PoolRealArray)p_inputs[1]).size() > 0) {
			a = p_inputs[1];
			num_a = a.size();
		} else {
			a.push_back(default_a);
			num_a = 1;
		}

		if (input_count > 2 && p_inputs[2].get_type() == Variant::POOL_REAL_ARRAY && ((PoolRealArray)p_inputs[2]).size() > 0) {
			b = p_inputs[2];
			num_b = b.size();
		} else {
			b.push_back(default_b);
			num_b = 1;
		}

		if (input_count > 3 && p_inputs[3].get_type() == Variant::POOL_REAL_ARRAY && ((PoolRealArray)p_inputs[3]).size() > 0) {
			c = p_inputs[3];
			num_c = c.size();
		} else {
			c.push_back(default_c);
			num_c = 1;
		}

		if (input_count > 4 && p_inputs[4].get_type() == Variant::POOL_REAL_ARRAY && ((PoolRealArray)p_inputs[4]).size() > 0) {
			d = p_inputs[4];
			num_d = d.size();
		} else {
			d.push_back(default_d);
			num_d = 1;
		}

		if (input_count > 5 && p_inputs[5].get_type() == Variant::POOL_REAL_ARRAY && ((PoolRealArray)p_inputs[5]).size() > 0) {
			e = p_inputs[5];
			num_e = e.size();
		} else {
			e.push_back(default_e);
			num_e = 1;
		}

		// Parse expression
		Ref<Expression> executor;
		executor.instance();
		Array input_names;
		input_names.push_back("i");
		input_names.push_back("a");
		input_names.push_back("b");
		input_names.push_back("c");
		input_names.push_back("d");
		input_names.push_back("e");
		Error err = executor->parse(expression, PoolStringArray(input_names));
		if (err != Error::OK) {
			// We error'd out. output is unmodified.
			Godot::print_warning(String("Node '{0}' failed to parse expression. Output will be unmodified.\nError Message: {1}")
					.format(Array::make(get_node_name(), executor->get_error_text())), __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		// How many reals will we output?
		int max = num_a > num_b ? num_a : num_b;
		max = max > num_c ? max : num_c;
		max = max > num_d ? max : num_d;
		max = max > num_e ? max : num_e;

		// Process
		PoolRealArray::Read ra = a.read();
		PoolRealArray::Read rb = b.read();
		PoolRealArray::Read rc = c.read();
		PoolRealArray::Read rd = d.read();
		PoolRealArray::Read re = e.read();

		values.resize(max);
		PoolRealArray::Write vw = values.write();
		Array inputs;
		for (int i = 0; i < max; i++) {
			inputs.push_back((float)i);
			inputs.push_back(ra[i % num_a]);
			inputs.push_back(rb[i % num_b]);
			inputs.push_back(rc[i % num_c]);
			inputs.push_back(rd[i % num_d]);
			inputs.push_back(re[i % num_e]);
			// Ideally '*executor' should be replaced with NULL but passing NULL crashes Godot so this is a workaround
			Variant v = executor->execute(inputs, *executor, false);
			if (executor->has_execute_failed()) {
				// If first execution fails, all of them will fail so exit early
				Godot::print_warning(String("Node '{0}' failed to execute expression. Output will be unmodified.\nError Message: {1}")
						.format(Array::make(get_node_name(), executor->get_error_text())), __FUNCTION__, __FILE__, __LINE__);
				return false;
			} else {
				vw[i] = v;
			}
			inputs.clear();
		}
	}

	return updated;
}

int GDProcExec::get_input_connector_count() const {
	return 6;
}

Variant::Type GDProcExec::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::STRING;
	} else {
		return Variant::POOL_REAL_ARRAY;
	}
}

const String GDProcExec::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "expression";
	} else if (p_slot == 1) {
		return "a";
	} else if (p_slot == 2) {
		return "b";
	} else if (p_slot == 3) {
		return "c";
	} else if (p_slot == 4) {
		return "d";
	} else if (p_slot == 5) {
		return "e";
	}

	return "";
}

const String GDProcExec::get_connector_property_name(int p_slot) const {
	if (p_slot == 0) {
		return "expression";
	} else if (p_slot == 1) {
		return "a";
	} else if (p_slot == 2) {
		return "b";
	} else if (p_slot == 3) {
		return "c";
	} else if (p_slot == 4) {
		return "d";
	} else if (p_slot == 5) {
		return "e";
	}

	return "";
}

int GDProcExec::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcExec::get_output_connector_type(int p_slot) const {
	return Variant::POOL_REAL_ARRAY;
}

const String GDProcExec::get_output_connector_name(int p_slot) const {
	return "values";
}

const Variant GDProcExec::get_output(int p_slot) const {
	return values;
}
