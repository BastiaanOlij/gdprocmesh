#include "transforms/gdprocraycast.h"

#include "PhysicsServer.hpp"
#include "PhysicsDirectBodyState.hpp"
#include "PhysicsDirectSpaceState.hpp"
#include "Viewport.hpp"
#include "World.hpp"

using namespace godot;

void GDProcRaycast::_register_methods() {
	register_property<GDProcRaycast, Vector3>("ray", &GDProcRaycast::set_ray, &GDProcRaycast::get_ray, Vector3(0.0f, 0.0f, 0.0f));
	register_property<GDProcRaycast, float>("offset", &GDProcRaycast::set_offset, &GDProcRaycast::get_offset, 0.1f);
}

String GDProcRaycast::get_type_name() {
	return "Raycast";
}

String GDProcRaycast::get_description() const {
	return "Moves the vertices in an array along a raycast until they hit a surface.\nVertices are not moved if the ray doesn't hit anything.\nYour mesh must be at the origin of the scene and your resource must be local to your scene!";
}

void GDProcRaycast::_init() {
	// first call super class
	GDProcNode::_init();

	// default values
	values.resize(1);
	values.set(0, Vector3(0.0f, 0.0f, 0.0f));
	default_ray = Vector3(0.0f, -1.0f, 0.0f);
	default_offset = 0.1f;
}

void GDProcRaycast::set_ray(Vector3 p_ray) {
	if (default_ray != p_ray) {
		default_ray = p_ray;
		must_update = true;
		emit_signal("changed");
	}
}

Vector3 GDProcRaycast::get_ray() {
	return default_ray;
}

void GDProcRaycast::set_offset(float p_offset) {
	if (default_offset != p_offset) {
		default_offset = p_offset;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcRaycast::get_offset() {
	return default_offset;
}

bool GDProcRaycast::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		int num_values = 0;
		PoolVector3Array input_values;
		int num_rays = 0;
		PoolVector3Array rays;
		int num_offsets = 0;
		PoolRealArray offsets;

		printf("Updating raycast\n");

		int input_count = p_inputs.size();
		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				input_values = p_inputs[0];
				num_values = input_values.size();
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_VECTOR3_ARRAY) {
				rays = p_inputs[1];
				num_rays = rays.size();
			}
		}
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::POOL_REAL_ARRAY) {
				offsets = p_inputs[1];
				num_offsets = offsets.size();
			}
		}

		if (num_rays == 0) {
			rays.push_back(default_ray);
			num_rays++;
		}

		if (num_offsets == 0) {
			offsets.push_back(default_offset);
			num_offsets++;
		}

		if (num_values > 0) {
			values.resize(num_values);

			PoolVector3Array::Write w = values.write();
			PoolVector3Array::Read r = input_values.read();
			PoolVector3Array::Read d = rays.read();
			PoolRealArray::Read o = offsets.read();

			PhysicsDirectSpaceState * state = NULL;

/*
			printf("Procedural mesh raycast, obtaining local scene\n");
			Node * localscene = get_local_scene();
			if (localscene == NULL) {
				printf("Procedural mesh raycast, no local scene found, is this a local resource?\n");
			} else {
				printf("Procedural mesh raycast, obtaining viewport\n");
				Viewport * vp = localscene->get_viewport();
				if (vp == NULL) {
					printf("Procedural mesh raycast, Couldn't obtain local viewport\n");
				} else {
					printf("Procedural mesh raycast, obtaining world\n");
					Ref<World> world = vp->find_world();
					if (world.is_null()) {
						printf("Procedural mesh raycast, Couldn't obtain world\n");
					} else {
						printf("Procedural mesh raycast, obtaining state\n");
						state = world->get_direct_space_state(); // <-- this crashes..
						if (state == NULL) {
							printf("Procedural mesh raycast, Couldn't obtain state\n");
						}
					}
				}
			}
*/

			if (state == NULL) {
				// no state? just copy them
				for (int i = 0; i < num_values; i++) {
					w[i] = r[i];
				}
			} else {
				for (int i = 0; i < num_values; i++) {
					Vector3 v = r[i];
					Vector3 cast_ray = d[i % num_rays];
					float off = o[i % num_offsets];

					printf("Performing raycast\n");

					Dictionary col = state->intersect_ray(v, v + (cast_ray * 1000.0f));
					if (col.empty()) {
						w[i] = v;
					} else {
						printf("Found collision: ");
						Vector3 pos = col["position"];
						Vector3 norm = col["normal"];
						printf("pos = %0.2f, %0.2f, %0.2f", pos.x, pos.y, pos.z);
						printf("norm = %0.2f, %0.2f, %0.2f", norm.x, norm.y, norm.z);
						printf("\n");
						w[i] = pos + (norm * off);
					}
				}
			}
		} else {
			printf("No inputs for raycast\n");
			values.resize(0);
		}
	}

	return updated;
}

int GDProcRaycast::get_input_connector_count() const {
	return 3;
}

Variant::Type GDProcRaycast::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 1) {
		return Variant::POOL_VECTOR3_ARRAY;
	} else if (p_slot == 2) {
		return Variant::POOL_REAL_ARRAY;
	}

	return Variant::NIL;
}

const String GDProcRaycast::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "vectors";
	} else if (p_slot == 1) {
		return "ray";
	} else if (p_slot == 2) {
		return "offset";
	}

	return "";
}

const String GDProcRaycast::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "ray";
	} else if (p_slot == 2) {
		return "offset";
	}

	return "";
}

int GDProcRaycast::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcRaycast::get_output_connector_type(int p_slot) const {
	return Variant::POOL_VECTOR3_ARRAY;
}

const String GDProcRaycast::get_output_connector_name(int p_slot) const {
	return "vectors";
}

const Variant GDProcRaycast::get_output(int p_slot) const {
	return Variant(values);
}

