#include "input/gdprocincurve.h"

using namespace godot;

void GDProcInCurve::_register_methods() {
	register_method("_curve_changed", &GDProcInCurve::_curve_changed);
}

String GDProcInCurve::get_type_name() {
	return "Input Curve";
}

String GDProcInCurve::get_description() const {
	return "Allows you to specify a Curve3D as input for the procedural mesh.\nCan be set from the editor.\nOutputs vertices at the baked interval.";
}

void GDProcInCurve::_init() {
	// first call super class
	GDProcNode::_init();
}

GDProcInCurve::~GDProcInCurve() {
	if (curve.is_valid()) {
		curve->disconnect("changed", this, "_curve_changed");
	}
}

bool GDProcInCurve::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		// parse our curve into vertices, couldn't be simpler...
		if (curve.is_valid()) {
			value = curve->get_baked_points();
		} else {
			value.resize(0);
		}
	}

	return updated;
}

Variant::Type GDProcInCurve::get_input_property_type() const {
	return Variant::OBJECT;
}

String GDProcInCurve::get_input_property_hint() const {
	return "Curve3D";
}

void GDProcInCurve::set_input(Variant p_input) {
	if (p_input.get_type() != Variant::OBJECT) {
		printf("Not an object\n");
		return;
	}

	Ref<Curve3D> new_curve = p_input;
	if (new_curve.is_null()) {
		printf("Not a valid object\n");
		return;
	} else if (new_curve->get_class() != "Curve3D") {
		printf("Not a valid object\n");
		return;
	}

	// disconnect from the old...
	if (curve.is_valid()) {
		curve->disconnect("changed", this, "_curve_changed");
	}

	// and update
	curve = new_curve;

	// connect to the new
	if (curve.is_valid()) {
		curve->connect("changed", this, "_curve_changed");
	}

	must_update = true;
	emit_signal("changed");
}

Variant GDProcInCurve::get_input() {
	if (curve.is_valid()) {
		return Variant(curve);
	}
	return Variant();
}

void GDProcInCurve::_curve_changed() {
	must_update = true;
	emit_signal("changed");
}

int GDProcInCurve::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcInCurve::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcInCurve::get_output_connector_name(int p_slot) const {
	return "vectors";
}

const Variant GDProcInCurve::get_output(int p_slot) const {
	return Variant(value);
}