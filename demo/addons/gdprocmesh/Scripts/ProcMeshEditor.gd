tool
extends VBoxContainer

var procmesh = null
var add_popup = null
var place = Vector2()
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
	var node = procmesh.get_node(p_node_id)
	
	# create a graph node for this
	var gn = preload("res://addons/gdprocmesh/Scenes/ProcNodeEditor.tscn").instance()
	gn.set_proc_node(procmesh, p_node_id)
	gn.connect("changed", self, "_update_graph")
	
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
		
		new_node.set_position(place + $GraphEdit.scroll_offset)
		procmesh.add_node(new_node, id_to_use)
		
		_update_graph();

func _add_node_class(p_name, p_class):
	var popup = add_popup
	var names = p_name.split("/")
	for i in range(names.size() - 1):
		var i_name = names[i]
		if not popup.has_node(i_name):
			var new_popup = PopupMenu.new()
			new_popup.name = i_name
			new_popup.connect("id_pressed", self, "_add_node")
			popup.add_child(new_popup)
			popup.add_submenu_item(i_name, i_name)
		popup = popup.get_node(i_name)
	
	node_classes.push_back(p_class)
	popup.add_item(names[names.size() - 1], node_classes.size() - 1)
	

func _ready():
	# create our add node button
	var add_button = MenuButton.new()
	add_button.text = "Add node..."
	add_button.connect("pressed", self, "set", ["place", Vector2(10.0, 50.0)])
	$GraphEdit.get_zoom_hbox().add_child(add_button)
	$GraphEdit.get_zoom_hbox().move_child(add_button, 0)
	
	# add some options (should make this smarter, maybe build an array first)
	add_popup = add_button.get_popup()
	add_popup.connect("id_pressed", self, "_add_node")

	# inputs
	_add_node_class("Inputs/Input Curve", "GDProcInCurve")
	_add_node_class("Inputs/Input Int", "GDProcInInt")
	_add_node_class("Inputs/Input Vectors", "GDProcInPoolVectors")
	_add_node_class("Inputs/Input Real", "GDProcInReal")
	_add_node_class("Inputs/Input Vector", "GDProcInVector")

	# primitives
	_add_node_class("Primitives/Count", "GDProcCount")
	_add_node_class("Primitives/Euler angles", "GDProcEuler")
	_add_node_class("Primitives/Random", "GDProcRandom")
	_add_node_class("Primitives/Vector", "GDProcVector")

	# transforms (work on primitives)
	_add_node_class("Transforms/Add", "GDProcAdd")
	_add_node_class("Transforms/Bevel", "GDProcBevel")
	_add_node_class("Transforms/Division", "GDProcDiv")
	_add_node_class("Transforms/Generate normals", "GDProcGenNormals")
	_add_node_class("Transforms/Redistribute", "GDProcRedist")
	_add_node_class("Transforms/Multiply", "GDProcMult")
	_add_node_class("Transforms/Rotate", "GDProcRotate")
	_add_node_class("Transforms/Scale", "GDProcScale")
	_add_node_class("Transforms/Subtract", "GDProcSub")
	_add_node_class("Transforms/Translate", "GDProcTranslate")

	# shapes
	_add_node_class("Shapes/Box", "GDProcBox")
	_add_node_class("Shapes/Circle", "GDProcCircle")
	_add_node_class("Shapes/Rectangle", "GDProcRect")

	# Surface
	_add_node_class("Surfaces/Extrude Shape", "GDProcExtrudeShape")
	_add_node_class("Surfaces/Surface", "GDProcSurface")

	# modifiers (work on surfaces)
	_add_node_class("Modifiers/Merge", "GDProcMerge")
	_add_node_class("Modifiers/Mirror", "GDProcMirror")
	_add_node_class("Modifiers/Place on path", "GDProcPlaceOnPath")
	_add_node_class("Modifiers/Transform", "GDProcTransform")

	# output
	_add_node_class("Output", "GDProcOutput")

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

func _on_GraphEdit_popup_request(p_position):
	place = get_local_mouse_position()
	add_popup.popup(Rect2(p_position, Vector2(1, 1)))
