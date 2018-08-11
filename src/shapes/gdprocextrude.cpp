#include "shapes/gdprocextrude.h"

using namespace godot;

void GDProcExtrude::_register_methods() {
	// add end caps property
}

String GDProcExtrude::get_type_name() {
	return String("Extrude");
}

void GDProcExtrude::_init() {
	// first call super class
	GDProcNode::_init();

	// is this shape closed?
	is_closed = false;
}

bool GDProcExtrude::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_shape = 0;
		PoolVector2Array shape;
		int num_path = 0;
		PoolVector3Array path;

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_VECTOR2_ARRAY) {
				shape = p_inputs[0];
				num_shape = shape.size();
			}
		}

		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				path = p_inputs[1];
				num_path = path.size();
			}
		}

		printf("Shape points: %i, path points: %i\n", num_shape, num_path);

		// in order to extrude we need a shape with atleast 3 points and a path with atleast 2 points 
		if ((num_shape >= 3) && (num_path >= 2)) {
			PoolVector2Array::Read sr = shape.read();
			PoolVector3Array::Read pr = path.read();

			int last_start_vertice = 0;
			int curr_vertice = 0;
			int curr_index = 0;

			// prepare our data
			if (is_closed) {
				vertices.resize(num_shape * (num_path + 1)) ;
				indices.resize(num_shape * num_path * 2 * 3);
			} else {
				// need to add indices for start and end caps..

				vertices.resize(num_shape * num_path);
				indices.resize(num_shape * (num_path - 1) * 2 * 3);
			}

			printf("Vertices: %i, indices: %i\n", vertices.size(), indices.size());

			PoolVector3Array::Write vw = vertices.write();
			PoolIntArray::Write iw = indices.write();

			Vector3 up = Vector3(0.0, 1.0, 0.0);
			for (int p = 0; p < num_path + (is_closed ? 1 : 0); p++) {
				// calculate our transform
				Transform xf;
				Vector3 lookat_dir;
				if (is_closed) {
					Vector3 p1 = pr[(p - 1) % num_path];
					Vector3 p2 = pr[(p + 1) % num_path];
					lookat_dir = (p2 - p1).normalized();
				} else {
					Vector3 p1 = pr[p == 0 ? 0 : (p - 1)];
					Vector3 p2 = pr[(p + 1) < num_path ? (p + 1) : (num_path - 1)];
					lookat_dir = (p2 - p1).normalized();
				}

				xf.origin = pr[p % num_path];
				xf = xf.looking_at(xf.origin + lookat_dir, up);

				// use our new up as our next up
				up = xf.basis.get_axis(1);

				// add some points
				int this_start_vertice = curr_vertice;
				for (int j = 0; j < num_shape; j++) {
					Vector2 point = sr[j];
					Vector3 xf_point = xf.xform(Vector3(point.x, point.y, 0.0));
					vw[curr_vertice++] = xf_point;
				}

				if (p > 0) {
					// add our section
					for (int j = 0; j < num_shape; j++) {
						iw[curr_index++] = last_start_vertice + j;
						iw[curr_index++] = this_start_vertice + ((j + 1) % num_shape);
						iw[curr_index++] = this_start_vertice + j;

						iw[curr_index++] = last_start_vertice + j;
						iw[curr_index++] = last_start_vertice + ((j + 1) % num_shape);
						iw[curr_index++] = this_start_vertice + ((j + 1) % num_shape);
					}
				}

				last_start_vertice = this_start_vertice;
			}

			if (!is_closed) {
				// add start and end caps

			}

			printf("Check vertices: %i, indices: %i\n", curr_vertice, curr_index);
		} else {
			vertices.resize(0);
			indices.resize(0);
		}
	}

	return updated;
}

int GDProcExtrude::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcExtrude::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR2_ARRAY;
	} else if (p_slot == 1) {
		return Variant::POOL_VECTOR3_ARRAY;
	}
	return Variant::NIL;
}

const String GDProcExtrude::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "shape";
	} else if (p_slot == 1) {
		return "path";
	}

	return "";
}

int GDProcExtrude::get_output_connector_count() const {
	return 2;
}

Variant::Type GDProcExtrude::get_output_connector_type(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant::POOL_VECTOR3_ARRAY;
		case 1:
			return Variant::POOL_INT_ARRAY;
		default:
			return Variant::NIL;		
	}
}

const String GDProcExtrude::get_output_connector_name(int p_slot) const {
	switch (p_slot) {
		case 0:
			return "Vertices";
		case 1:
			return "Indices";
		default:
			return "???";
	}
}

const Variant GDProcExtrude::get_output(int p_slot) const {
	switch (p_slot) {
		case 0:
			return Variant(vertices);
		case 1:
			return Variant(indices);
		default:
			return Variant();
	}
}
