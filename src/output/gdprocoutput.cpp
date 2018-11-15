#include "output/gdprocoutput.h"

using namespace godot;

void GDProcOutput::_register_methods() {
}

String GDProcOutput::get_type_name() {
	return String("Output");
}

String GDProcOutput::get_description() const {
	return String("Used to output a surface to the final mesh.");
}

void GDProcOutput::_init() {
	// first call super class
	GDProcNode::_init();

	// nothing to do yet for ourselves.
}

bool GDProcOutput::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		// Really should change this so we keep a reference of sorts, this is a bit of a wasteful copy.
		// Reduz was talking the other day about adding reference counting to arrays, that would be pretty cool.

		output_arr.resize(ArrayMesh::ARRAY_MAX);

		// and copy from our input if we have an input
		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::ARRAY) {
				// For some reason just copying the array doesn't work, so copy each member.
				// Will prove useful later on because I might add additional arrays to our source array.
				Array surface = p_inputs[0];

				output_arr[ArrayMesh::ARRAY_VERTEX] = surface[ArrayMesh::ARRAY_VERTEX];
				output_arr[ArrayMesh::ARRAY_NORMAL] = surface[ArrayMesh::ARRAY_NORMAL];
				output_arr[ArrayMesh::ARRAY_TANGENT] = surface[ArrayMesh::ARRAY_TANGENT];
				output_arr[ArrayMesh::ARRAY_COLOR] = surface[ArrayMesh::ARRAY_COLOR];
				output_arr[ArrayMesh::ARRAY_TEX_UV] = surface[ArrayMesh::ARRAY_TEX_UV];
				output_arr[ArrayMesh::ARRAY_TEX_UV2] = surface[ArrayMesh::ARRAY_TEX_UV2];
				output_arr[ArrayMesh::ARRAY_BONES] = surface[ArrayMesh::ARRAY_BONES];
				output_arr[ArrayMesh::ARRAY_WEIGHTS] = surface[ArrayMesh::ARRAY_WEIGHTS];
				output_arr[ArrayMesh::ARRAY_INDEX] = surface[ArrayMesh::ARRAY_INDEX];
			}
		}
	}

	return updated;
}

int GDProcOutput::get_input_connector_count() const {
	return 1;
}

Variant::Type GDProcOutput::get_input_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcOutput::get_input_connector_name(int p_slot) const {
	return "Surface";
}

int GDProcOutput::get_output_connector_count() const {
	// We set this to zero even though we output our final array, we don't want this connector to show
	// and it means this will be treated as a final output node
	return 0;
}

const Variant GDProcOutput::get_output(int p_slot) const {
	// this should only be called after update is called.
	if (p_slot == 0) {
		return Variant(output_arr);
	}

	return Variant();
}
