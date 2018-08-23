Todo list:
 - add subdivide node
 - add sphere node
 - make generate normals work on a surface, have it calculate tangents and options like smooth?
 - add a UV unwrap (maybe using the Thekla module in Godot?)
 - add an input where we use another mesh as an input
 - add an extract node that takes a surface as input and has each array as an output
 - add boolean node to do CSG merge
 - add group node where you can combine a number of nodes into one new node to simplify graphs
 - add collision output

Things to contemplate:
 - introduce a data object that keeps far more state about our surfaces such as selected vertices, edges and faces and maybe works on n-polygons with quads as the default type. Triangulation then only happens on final output
 - introduce a layer object between our graph and the mesh. Maybe GDProcGraph? Then we can create a GDProcCollisionShape and have it share the same graph with the accompanying GDProcMesh instance
