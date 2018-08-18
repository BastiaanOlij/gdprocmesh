#include "surfaces/gdprocextrudeshape.h"

using namespace godot;

void GDProcExtrudeShape::_register_methods() {
	register_property<GDProcExtrudeShape, bool>("shape_is_closed", &GDProcExtrudeShape::set_shape_is_closed, &GDProcExtrudeShape::get_shape_is_closed, true);
	register_property<GDProcExtrudeShape, bool>("path_is_closed", &GDProcExtrudeShape::set_path_is_closed, &GDProcExtrudeShape::get_path_is_closed, false);
}

String GDProcExtrudeShape::get_type_name() {
	return String("Extrude Shape");
}

void GDProcExtrudeShape::_init() {
	// first call super class
	GDProcNode::_init();

	// defaults
	shape_is_closed = true;
	path_is_closed = false;
}

void GDProcExtrudeShape::set_shape_is_closed(bool p_is_closed) {
	if (shape_is_closed != p_is_closed) {
		shape_is_closed = p_is_closed;
		must_update = true;
		emit_signal("changed");
	}
}

bool GDProcExtrudeShape::get_shape_is_closed() const {
	return shape_is_closed;
}

void GDProcExtrudeShape::set_path_is_closed(bool p_is_closed) {
	if (path_is_closed != p_is_closed) {
		path_is_closed = p_is_closed;
		must_update = true;
		emit_signal("changed");
	}
}

bool GDProcExtrudeShape::get_path_is_closed() const {
	return path_is_closed;
}

bool GDProcExtrudeShape::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		// define our destination buffers
		PoolVector3Array vertices;
		PoolVector3Array normals;
		PoolRealArray tangents;
		PoolVector2Array uvs;
		PoolIntArray indices;

		// obtain some defaults
		bool sic = shape_is_closed;
		int num_shape = 0;
		PoolVector3Array shape;
		bool pic = path_is_closed;
		int num_path = 0;
		PoolVector3Array path;

		// resize our array just in case
		surface_arr.resize(ArrayMesh::ARRAY_MAX);

		// parse our inputs
		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				shape = p_inputs[0];
				num_shape = shape.size();
			}
		}

		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::BOOL) {
				sic = p_inputs[1];
			}
		}

		if (input_count > 2) {
			if (p_inputs[2].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				path = p_inputs[2];
				num_path = path.size();
			}
		}

		if (input_count > 3) {
			if (p_inputs[3].get_type() == Variant::BOOL) {
				pic = p_inputs[3];
			}
		}

		// make sure we have enough definition for extrusion 
		if ((num_shape >= (sic ? 3 : 2)) && (num_path >= (pic ? 3 : 2))) {
			// lock our source buffers for reading
			PoolVector3Array::Read sr = shape.read();
			PoolVector3Array::Read pr = path.read();

			// define some stuff
			int last_start_vertice = 0;
			int curr_vertice = 0;
			int curr_tangent = 0;
			int curr_index = 0;
			int num_vertices = (sic ? num_shape + 1 : num_shape); // for closed shape, we add a vertice
			float shape_total_len = 0.0;

			// prepare our data
			int total_vertices = 0;
			int total_indices = 0;
			if (pic) {
				total_vertices = num_vertices * (num_path + 1);
				total_indices = (num_vertices - 1) * num_path * 2 * 3;
			} else {
				total_vertices = num_vertices * num_path;
				total_indices = (((num_vertices - 1) * (num_path - 1)) + (num_vertices - 2)) * 2 * 3;
			}
			vertices.resize(total_vertices);
			normals.resize(total_vertices);
			tangents.resize(total_vertices * 4);
			uvs.resize(total_vertices);
			indices.resize(total_indices);

			// figure out the total length of our shape
			Vector3 last_point = sr[0];
			for (int j = 1; j < (sic ? num_shape + 1 : num_shape); j++) {
				Vector3 point = sr[j % num_shape];
				shape_total_len += (point - last_point).length();
				last_point = point;
			}

			// lock our destination buffers for writing
			PoolVector3Array::Write vw = vertices.write();
			PoolVector3Array::Write nw = normals.write();
			PoolRealArray::Write tw = tangents.write();
			PoolVector2Array::Write uvw = uvs.write();
			PoolIntArray::Write iw = indices.write();

			// let's extrude
			Transform first_xf;
			Vector3 last_pos = pr[0];
			Vector3 up = Vector3(0.0, 1.0, 0.0);
			Vector2 uv = Vector2(0.0, 0.0);

			for (int p = 0; p < num_path + (pic ? 1 : 0); p++) {
				// calculate our transform
				Transform xf;
				if (p < num_path) {
					Vector3 dir;
					if (pic) {
						Vector3 p1 = pr[p == 0 ? (num_path - 1) : ((p - 1) % num_path)];
						Vector3 p2 = pr[(p + 1) % num_path];
						dir = (p2 - p1).normalized();
					} else {
						Vector3 p1 = pr[p == 0 ? 0 : (p - 1)];
						Vector3 p2 = pr[(p + 1) < num_path ? (p + 1) : (num_path - 1)];
						dir = (p2 - p1).normalized();
					}

					// our first entry is straight up or down?
					if ((p == 0) && (fabs(dir.dot(up)) > 0.999f)) {
						// let's start with a different value
						up = Vector3(1.0, 0.0, 0.0);
					}

					// calculate our new transform
					xf.origin = pr[p % num_path];
					xf = xf.looking_at(xf.origin + dir, up);

					// use our new up for our last up
					up = xf.basis.get_axis(1);

					///@TODO need to find a way to start nudging our up back to straight up whenever we can (i.e. dir is not upwards or downwards)

					if (p == 0) {
						// keep a copy, we may need this..
						first_xf = xf;
					}
				} else {
					// reuse our first transform for our last..
					xf = first_xf;
				}

				// add our distance to our uv
				uv.y += (xf.origin - last_pos).length();
				last_pos = xf.origin;

				// add some points
				int this_start_vertice = curr_vertice;
				Vector3 last_point = sr[0];
				float shape_len = 0.0;
				for (int j = 0; j < (sic ? num_shape + 1 : num_shape); j++) {
					// get our point
					Vector3 point = sr[j % num_shape]; // we assume z to be 0 but...

					// calculate our vertice
					Vector3 xf_point = xf.xform(Vector3(point.x, point.y, point.z));
					vw[curr_vertice] = xf_point;

					// calculate our normal (we cheat for now)
					// we should calculate our normals based on our shape and cache them
					nw[curr_vertice] = xf.basis.xform(Vector3(point.x, point.y, point.z)).normalized();

					// we should calculate our tangent, again do once and cache
					Vector3 tangent = xf.basis.get_axis(2).cross(nw[curr_vertice]).normalized();
					tw[curr_tangent++] = tangent.x;
					tw[curr_tangent++] = tangent.y;
					tw[curr_tangent++] = tangent.z;
					tw[curr_tangent++] = -1.0;

					// calculate our uv
					if (j > 0) {
						shape_len += (point - last_point).length();
						last_point = point;
					}
					uv.x = shape_len / shape_total_len; // we should calculate these lengths once and cache them.
					uvw[curr_vertice] = uv;

					// advance..
					curr_vertice++;
				}

				if (p > 0) {
					// add our section
					for (int j = 0; j < num_vertices - 1; j++) {
						iw[curr_index++] = last_start_vertice + j;
						iw[curr_index++] = this_start_vertice + ((j + 1));
						iw[curr_index++] = this_start_vertice + j;

						iw[curr_index++] = last_start_vertice + j;
						iw[curr_index++] = last_start_vertice + ((j + 1));
						iw[curr_index++] = this_start_vertice + ((j + 1));
					}
				}

				last_start_vertice = this_start_vertice;
			}

			if (!pic) {
				// add start and end caps
				for (int j = 2; j < num_vertices; j++) {
					// front cap
					iw[curr_index++] = 0;
					iw[curr_index++] = j;
					iw[curr_index++] = j - 1;

					iw[curr_index++] = last_start_vertice;
					iw[curr_index++] = last_start_vertice + (j - 1);
					iw[curr_index++] = last_start_vertice + j;
				}

			}
		}

		surface_arr[ArrayMesh::ARRAY_VERTEX] = vertices;
		surface_arr[ArrayMesh::ARRAY_NORMAL] = normals;
		surface_arr[ArrayMesh::ARRAY_TANGENT] = tangents;
		surface_arr[ArrayMesh::ARRAY_TEX_UV] = uvs;
		surface_arr[ArrayMesh::ARRAY_INDEX] = indices;
	}

	return updated;
}

int GDProcExtrudeShape::get_input_connector_count() const {
	return 4;
}

Variant::Type GDProcExtrudeShape::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 1) {
		return Variant::BOOL;
	} else if (p_slot == 2) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 3) {
		return Variant::BOOL;
	}
	return Variant::NIL;
}

const String GDProcExtrudeShape::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "shape";
	} else if (p_slot == 1) {
		return "shape_is_closed";
	} else if (p_slot == 2) {
		return "path";
	} else if (p_slot == 3) {
		return "path_is_closed";
	}

	return "";
}

const String GDProcExtrudeShape::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "shape_is_closed";
	} else if (p_slot == 3) {
		return "path_is_closed";
	}
	return "";
}

int GDProcExtrudeShape::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcExtrudeShape::get_output_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcExtrudeShape::get_output_connector_name(int p_slot) const {
	return "Surface";
}

const Variant GDProcExtrudeShape::get_output(int p_slot) const {
	if (p_slot == 0) {
		return Variant(surface_arr);
	}

	return Variant();
}
