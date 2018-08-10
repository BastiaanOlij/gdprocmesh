tool
extends VBoxContainer

var procmesh = null
var add_button = null
var add_popup = null
var node_classes = Array()

func _clear_graph():
	$GraphEdit.clear_connections()
	for child in $GraphEdit.get_children():
		if child.is_class('GraphNode'):
			# print('removing: ' + child.name)
			$GraphEdit.remove_child(child)

func edit_mesh(p_procmesh):
	if p_procmesh:
		procmesh = p_procmesh
		
		_update_graph()
	else:
		procmesh = null
		_clear_graph()
		hide()

func _create_graph_node(p_node_id):
	# should turn this into a scene to (pre)load...
	
	# print("Found node with id " + String(p_node_id))
	var node = procmesh.get_node(p_node_id)
	var slot_offset = 0
	
	# create a graph node for this
	var gn = preload("res://addons/gdprocmesh/Scenes/ProcNodeEditor.tscn").instance()
	gn.set_proc_node(procmesh, p_node_id)
	gn.connect("changed", self, "_update_graph")
	
	slot_offset += 1
	
	return gn

func _update_graph():
	# clear graph
	_clear_graph()
	
	# lets build our new graph
	var node_ids = procmesh.get_node_id_list()
	for node_id in node_ids:
		var gn = _create_graph_node(node_id)
		$GraphEdit.add_child(gn)
	
	var connections = procmesh.get_connection_list()
	for c in connections:
		$GraphEdit.connect_node(String(c[2]), c[3], String(c[0]), c[1])

func _add_node(p_type):
	var new_script = NativeScript.new()
	new_script.library = procmesh.script.library
	new_script.set_class_name(node_classes[p_type])
	
	var new_node = Resource.new()
	new_node.script = new_script
	
	if new_node:
		var id_to_use = procmesh.get_free_id()
		
		new_node.set_position(Vector2(10.0, 50.0) + $GraphEdit.scroll_offset)
		procmesh.add_node(new_node, id_to_use)
		
		_update_graph();

func _add_node_class(p_name, p_class):
	add_popup.add_item(p_name)
	node_classes.push_back(p_class)

func _ready():
	# create our add node button
	add_button = MenuButton.new()
	add_button.text = "Add node..."
	$GraphEdit.get_zoom_hbox().add_child(add_button)
	$GraphEdit.get_zoom_hbox().move_child(add_button, 0)
	
	# add some options (should make this smarter, maybe build an array first)
	add_popup = add_button.get_popup()
	add_popup.connect("id_pressed", self, "_add_node")

	# inputs
	_add_node_class("Input Real", "GDProcInReal")
	_add_node_class("Input Vec3", "GDProcInVec3")

	# primitives
	_add_node_class("Vec3", "GDProcVec3")

	# transforms (work on primitives)
	_add_node_class("Vec3 Translate", "GDProcVec3Translate")
	_add_node_class("Generate normals", "GDProcGenNormals")

	# shapes
	_add_node_class("Box", "GDProcBox")

	# output
	_add_node_class("Surface", "GDProcSurface")
	_add_node_class("Output", "GDProcOutput")

	# modifiers (work on surfaces)

func _input(event):
	pass

func _on_GraphEdit_connection_request(from, from_slot, to, to_slot):
	var output_node = from.to_int()
	var input_node = to.to_int()
	procmesh.add_connection(input_node, to_slot, output_node, from_slot)
	
	_update_graph();

func _on_GraphEdit_disconnection_request(from, from_slot, to, to_slot):
	# ignore from
	var input_node = to.to_int()
	procmesh.remove_connection(input_node, to_slot)

	_update_graph();
