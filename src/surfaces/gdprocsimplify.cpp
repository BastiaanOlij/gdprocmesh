#include "surfaces/gdprocsimplify.h"
#include "3rdparty/Simplify.h"

using namespace godot;

void GDProcSimplify::_register_methods() {
	register_property<GDProcSimplify, float>("reduction_factor",
			&GDProcSimplify::set_reduction,
			&GDProcSimplify::get_reduction, 0.5);
}

String GDProcSimplify::get_type_name() {
	return String("Simplify");
}

String GDProcSimplify::get_description() const {
	return "Takes a surface a runs a Fast Quadric Mesh Simplification routine on "
		   "it.";
}

void GDProcSimplify::_init() {
	// first call super class
	GDProcNode::_init();

	reduction = 0.5;
}

void GDProcSimplify::set_reduction(float p_reduction) {
	if (reduction != p_reduction && p_reduction > 0.1 && p_reduction < 0.9) {
		reduction = p_reduction;
		must_update = true;
		emit_signal("changed");
	}
}

float GDProcSimplify::get_reduction() const {
	return reduction;
}

int GDProcSimplify::find_vertice(float p_x, float p_y, float p_z) {
	// how much distance can there be between vertices to consider them the same?
	float precision = 0.0001;

	// SLOOOOOOOW, need to optimise this
	for (int i = 0; i < Simplify::vertices.size(); i++) {
		if ((abs(Simplify::vertices[i].p.x - p_x) < precision) &&
				(abs(Simplify::vertices[i].p.y - p_y) < precision) &&
				(abs(Simplify::vertices[i].p.z - p_z) < precision)) {
			// printf("Found duplicate vertice %i\n", i);
			return i;
		}
	}

	return -1;
}

bool GDProcSimplify::update(bool p_inputs_updated, const Array &p_inputs) {
	bool updated = must_update || p_inputs_updated;
	must_update = false;

	if (updated) {
		float _reduction = reduction;

		// resize our array just in case
		surface_arr.resize(ArrayMesh::ARRAY_MAX);

		// clear our array
		for (int i = 0; i < ArrayMesh::ARRAY_MAX; i++) {
			surface_arr[i] = Variant();
		}

		int input_count = p_inputs.size();
		if (input_count > 1) {
			if (p_inputs[1].get_type() == Variant::REAL) {
				_reduction = p_inputs[1];
			}
		}

		// Just in case, constrain this
		if (_reduction < 0.1) {
			_reduction = 0.1;
		} else if (_reduction > 0.9) {
			_reduction = 0.9;
		}

		if (input_count > 0) {
			if (p_inputs[0].get_type() == Variant::ARRAY) {
				Array source_array = p_inputs[0];

				// this is just a first go, we'll loose some stuff, but it'll be cool :)
				Simplify::vertices.clear();
				Simplify::triangles.clear();

				// load our vertices as is, note that there is a lot of extra data that
				// we keep default
				PoolVector3Array source_v = source_array[ArrayMesh::ARRAY_VERTEX];
				int num_vertices = source_v.size();
				if (num_vertices > 0) {
					PoolVector3Array::Read sv = source_v.read();
					Simplify::Vertex v;
					std::vector<int> vmap;

					printf("Source vertice count: %i\n", num_vertices);

					for (int i = 0; i < num_vertices; i++) {
						v.p.x = sv[i].x;
						v.p.y = sv[i].y;
						v.p.z = sv[i].z;
						int j = find_vertice(v.p.x, v.p.y, v.p.z);
						if (j == -1) {
							Simplify::vertices.push_back(v);

							vmap.push_back(Simplify::vertices.size() - 1);
						} else {
							vmap.push_back(j);
						}
					}

					// load our triangles, here it gets funky, UVs and normals are loaded
					// into the triangle
					PoolIntArray source_i = source_array[ArrayMesh::ARRAY_INDEX];
					int num_indices = source_i.size();
					if (num_indices) {
						PoolIntArray::Read si = source_i.read();
						Simplify::Triangle t;

						PoolVector2Array source_uv = source_array[ArrayMesh::ARRAY_TEX_UV];
						int num_uvs = source_uv.size();
						PoolVector2Array::Read suv = source_uv.read();

						printf("Source indice count: %i, uv count: %i\n", num_indices,
								num_uvs);

						for (int i = 0; i < num_indices; i += 3) {
							t.v[0] = vmap[si[i]];
							t.v[1] = vmap[si[i + 1]];
							t.v[2] = vmap[si[i + 2]];
							t.attr = 0;
							t.material = 0;

							// need to add uvs
							if (num_uvs > 0) {
								for (int n = 0; n < 3; n++) {
									vec3f uv;
									int a = si[i + n];

									uv.x = suv[a].x;
									uv.y = suv[a].y;
									uv.z = 0.0;

									t.uvs[n] = uv;
								}
								t.attr |= Simplify::TEXCOORD;
							}

							// need to add normals?

							Simplify::triangles.push_back(t);
						}

						printf("Staring vertice count: %zi and face count: %zi\n",
								Simplify::vertices.size(), Simplify::triangles.size());

						// apply our simplify routine
						int face_count = Simplify::triangles.size();
						int target_count = (int)round((float)face_count * _reduction);
						if (target_count < 4) {
							target_count = 4;
						}
						double agressiveness = 7.0;

						printf("Attemping simplify to %i faces with agressiveness %0.2f\n",
								target_count, agressiveness);

						Simplify::simplify_mesh(target_count, agressiveness, true);

						printf("Resulting vertice count: %zi and face count: %zi\n",
								Simplify::vertices.size(), Simplify::triangles.size());

						// and build our resulting arrays
						PoolVector3Array vs;
						PoolVector3Array ns;
						PoolRealArray ts;
						PoolVector2Array uvs;
						PoolIntArray is;
						std::vector<bool> processed_v;

						// process our vertices
						{
							std::vector<vec3f> tan;
							int new_v_count = Simplify::vertices.size();
							vs.resize(new_v_count);
							ns.resize(new_v_count);
							ts.resize(new_v_count * 4);
							processed_v.resize(new_v_count);
							tan.resize(new_v_count * 2);

							PoolVector3Array::Write vw = vs.write();
							PoolVector3Array::Write nw = ns.write();
							PoolRealArray::Write tw = ts.write();

							for (int i = 0; i < new_v_count; i++) {
								vw[i].x = Simplify::vertices[i].p.x;
								vw[i].y = Simplify::vertices[i].p.y;
								vw[i].z = Simplify::vertices[i].p.z;

								// reset all these
								processed_v[i] = false;
								nw[i].x = 0.0;
								nw[i].y = 0.0;
								nw[i].z = 0.0;
								tan[i].x = 0.0;
								tan[i].y = 0.0;
								tan[i].z = 0.0;
								tan[i + new_v_count].x = 0.0;
								tan[i + new_v_count].y = 0.0;
								tan[i + new_v_count].z = 0.0;
								tw[i + 0] = 0.0;
								tw[i + 1] = 0.0;
								tw[i + 2] = 0.0;
								tw[i + 3] = 0.0;
							}

							// calculate our normals and tangents
							// (http://www.terathon.com/code/tangent.html)
							for (int i = 0; i < Simplify::triangles.size(); i++) {
								int a = Simplify::triangles[i].v[0];
								int b = Simplify::triangles[i].v[1];
								int c = Simplify::triangles[i].v[2];

								// get our source vectors
								Vector3 v1 = vw[a];
								Vector3 v2 = vw[b];
								Vector3 v3 = vw[c];

								////////////////////////////////////////////////////////////////////
								// normals first
								// calculate our edges
								v2 -= v1;
								v3 -= v1;

								// cross product (not normalized, bigger faces play a bigger
								// role)
								Vector3 n = v3.cross(v2);

								// and add to the normals for these vectors
								nw[a] += n;
								nw[b] += n;
								nw[c] += n;

								////////////////////////////////////////////////////////////////////
								// now tangents
								// get our source texture coordinates
								vec3f w1 = Simplify::triangles[i].uvs[0];
								vec3f w2 = Simplify::triangles[i].uvs[1];
								vec3f w3 = Simplify::triangles[i].uvs[2];

								// we already calculated this up above for our normals
								// float x1 = v2.x - v1.x;
								// float x2 = v3.x - v1.x;
								// float y1 = v2.y - v1.y;
								// float y2 = v3.y - v1.y;
								// float z1 = v2.z - v1.z;
								// float z2 = v3.z - v1.z;

								float s1 = w2.x - w1.x;
								float s2 = w3.x - w1.x;
								float t1 = w2.y - w1.y;
								float t2 = w3.y - w1.y;

								float r = 1.0F / (s1 * t2 - s2 * t1);
								vec3f sdir((t2 * v2.x - t1 * v3.x) * r, (t2 * v2.y - t1 * v3.y) * r,
										(t2 * v2.z - t1 * v3.z) * r);
								vec3f tdir((s1 * v3.x - s2 * v2.x) * r, (s1 * v3.y - s2 * v2.y) * r,
										(s1 * v3.z - s2 * v2.z) * r);

								// += seems broken on vec3f...
								tan[a] = tan[a] + sdir;
								tan[b] = tan[b] + sdir;
								tan[c] = tan[c] + sdir;

								tan[a + new_v_count] = tan[a + new_v_count] + tdir;
								tan[b + new_v_count] = tan[b + new_v_count] + tdir;
								tan[c + new_v_count] = tan[c + new_v_count] + tdir;
							}

							// and normalize our normals and update our tangents
							int ti = 0;
							for (int i = 0; i < new_v_count; i++) {
								nw[i].normalize();

								vec3f n(nw[i].x, nw[i].y, nw[i].z);
								vec3f t1 = tan[i];
								vec3f t2 = tan[i + new_v_count];

								// Gram-Schmidt orthogonalize
								vec3f tn = (t1 - n * n.dot(t1)).normalize();

								// And copy into our tangent buffer (note that we negate the tangent due to Godot handedness)
								tw[ti++] = (float)-tn.x;
								tw[ti++] = (float)-tn.y;
								tw[ti++] = (float)-tn.z;

								// Calculate handedness
								vec3f nc; // cross also assigns, bit weird..
								tw[ti++] = (nc.cross(n, t1).dot(t2) < 0.0F) ? -1.0F : 1.0F;
							}
						}

						// and our faces
						{
							int new_i_count = Simplify::triangles.size();
							int new_i3_count = new_i_count * 3;
							is.resize(new_i3_count);
							PoolIntArray::Write iw = is.write();
							int di = 0;

							// we won't need this many but we'll resize later on
							uvs.resize(new_i3_count);
							PoolVector2Array::Write uvw = uvs.write();
							for (int i = 0; i < new_i3_count; i++) {
								uvw[i].x = 0.0;
								uvw[i].y = 0.0;
							}

							for (int i = 0; i < new_i_count; i++) {
								for (int n = 0; n < 3; n++) {
									int a = Simplify::triangles[i].v[n];

									if (processed_v[a]) {
										Vector3 dup_v = vs[a];
										Vector3 dup_n = ns[a];
										int ta = a * 4;
										float dup_tx = ts[ta++];
										float dup_ty = ts[ta++];
										float dup_tz = ts[ta++];
										float dup_tw = ts[ta++];

										a = vs.size();

										vs.push_back(dup_v);
										ns.push_back(dup_n);
										ts.push_back(dup_tx);
										ts.push_back(dup_ty);
										ts.push_back(dup_tz);
										ts.push_back(dup_tw);
									} else {
										processed_v[a] = true;
									}

									// set our UV
									uvw[a].x = Simplify::triangles[i].uvs[n].x;
									uvw[a].y = Simplify::triangles[i].uvs[n].y;

									iw[di++] = a;
								}
							}
						}

						// and trim to the size of our vertices
						uvs.resize(vs.size());

						// copy our end result into our array
						surface_arr[ArrayMesh::ARRAY_VERTEX] = vs;
						surface_arr[ArrayMesh::ARRAY_NORMAL] = ns;
						surface_arr[ArrayMesh::ARRAY_TANGENT] = ts;
						surface_arr[ArrayMesh::ARRAY_TEX_UV] = uvs;
						surface_arr[ArrayMesh::ARRAY_INDEX] = is;
					}
				}
			}
		}
	}

	return updated;
}

int GDProcSimplify::get_input_connector_count() const {
	return 2;
}

Variant::Type GDProcSimplify::get_input_connector_type(int p_slot) const {
	if (p_slot == 0) {
		return Variant::ARRAY;
	} else if (p_slot == 1) {
		return Variant::REAL;
	}

	return Variant::NIL;
}

const String GDProcSimplify::get_input_connector_name(int p_slot) const {
	if (p_slot == 0) {
		return "surface";
	} else if (p_slot == 1) {
		return "reduction";
	}

	return "";
}

const String GDProcSimplify::get_connector_property_name(int p_slot) const {
	if (p_slot == 1) {
		return "reduction_factor";
	}

	return "";
}

int GDProcSimplify::get_output_connector_count() const {
	return 1;
}

Variant::Type GDProcSimplify::get_output_connector_type(int p_slot) const {
	return Variant::ARRAY;
}

const String GDProcSimplify::get_output_connector_name(int p_slot) const {
	return "surface";
}

const Variant GDProcSimplify::get_output(int p_slot) const {
	if (p_slot == 0) {
		return Variant(surface_arr);
	}

	return Variant();
}
