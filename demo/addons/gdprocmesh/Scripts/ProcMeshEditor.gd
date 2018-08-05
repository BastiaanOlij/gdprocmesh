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

func _get_type_color(p_type):
	if p_type == TYPE_REAL:
		return Color("#f34d4d")
	elif p_type == TYPE_VECTOR3:
		return Color("#1e8a76")
	elif p_type == TYPE_VECTOR3_ARRAY:
		return Color("#d67dee")
	elif p_type == TYPE_VECTOR2_ARRAY:
		return Color("#61daf4")
	elif p_type == TYPE_INT_ARRAY:
		return Color("#f6a86e")
	elif p_type == TYPE_REAL_ARRAY:
		return Color("#8ad82b")
	elif p_type == TYPE_COLOR_ARRAY:
		return Color("#d82ba5")
	elif p_type == TYPE_ARRAY:
		# array is assumed to be a surface
		return Color("#a8fbde")
	
	print('Need a color for type ' + String(p_type))
	return Color(0.0, 0.0, 1.0)

func _create_graph_node(p_node_id):
	# should turn this into a scene to (pre)load...
	
	# print("Found node with id " + String(p_node_id))
	var node = procmesh.get_node(p_node_id)
	var slot_offset = 0
	
	# create a graph node for this
	var gn = preload("res://addons/gdprocmesh/Scenes/ProcNodeEditor.tscn").instance()
	gn.set_proc_node(procmesh, p_node_id)
	gn.connect("changed", self, "_update_graph")
	
	var node_arr = Array()
	node_arr.push_back(p_node_id)
	
	slot_offset += 1
	
	# add input and output connectors
	var input_connector_count = node.get_input_connector_count()
	var output_connector_count = node.get_output_connector_count()
	
	if (input_connector_count > 0 or output_connector_count > 0):
		for i in range(0, max(input_connector_count, output_connector_count)):
			var left_is_valid = i < input_connector_count
			var left_type = 0
			var left_color = Color(0.0, 0.0, 1.0, 1.0)
			var right_is_valid = i < output_connector_count
			var right_type = 0
			var right_color = Color(0.0, 0.0, 1.0, 1.0)
			
			var hb = HBoxContainer.new()
			
			if left_is_valid:
				var label = Label.new()
				label.set_text(node.get_input_connector_name(i))
				label.rect_min_size = Vector2(75.0, 0.0)
				hb.add_child(label)
				left_type = node.get_input_connector_type(i)
				left_color = _get_type_color(left_type)
				
			var prop_name = node.get_connector_property_name(i)
			if prop_name != '':
				var prop_field = null
				var prop_value = node.get(prop_name)
				var prop_type = typeof(prop_value)
				if prop_type == TYPE_INT:
					prop_field = LineEdit.new()
					prop_field.align = LineEdit.ALIGN_RIGHT
					prop_field.set_text(String(prop_value))
				elif prop_type == TYPE_REAL:
					prop_field = LineEdit.new()
					prop_field.align = LineEdit.ALIGN_RIGHT
					prop_field.set_text("%0.3f" % prop_value)
					
				if prop_field:
					var prop_arr = Array()
					prop_arr.push_back(p_node_id)
					prop_arr.push_back(prop_name)
					prop_arr.push_back(prop_field)
					prop_field.connect("text_entered", self, "_set_node_property", prop_arr)
					prop_field.connect("focus_exited", self, "_exit_node_property", prop_arr)
					prop_field.rect_min_size = Vector2(75.0, 0.0)
					hb.add_child(prop_field)
			
			hb.add_spacer(false)
			
			if right_is_valid:
				var label = Label.new()
				label.set_text(node.get_output_connector_name(i))
				label.set_align(Label.ALIGN_RIGHT)
				hb.add_child(label)
				right_type = node.get_output_connector_type(i)
				right_color = _get_type_color(right_type)
			
			gn.add_child(hb)
			gn.set_slot(i + slot_offset, left_is_valid, left_type, left_color, right_is_valid, right_type, right_color)
	
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

func _set_node_name(p_value, p_node_id):
	pass

func _exit_node_name(p_node_id):
	pass

func _set_node_property(p_value, p_node_id, p_property, p_field):
	var node = procmesh.get_node(p_node_id)
	if node:
		node.set(p_property, p_value)
		
		# see what it changed to and update our display, need to figure out how...
		var prop_value = node.get(p_property)
		var prop_type = typeof(prop_value)
		if prop_type == TYPE_INT:
			p_field.set_text(String(prop_value))
		elif prop_type == TYPE_REAL:
			p_field.set_text("%0.3f" % prop_value)

func _exit_node_property(p_node_id, p_property, p_field):
	var value = p_field.get_text()
	_set_node_property(value, p_node_id, p_property, p_field)

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

	# primitives
	_add_node_class("Vec3", "GDProcVec3")

	# transforms
	_add_node_class("Vec3 Translate", "GDProcVec3Translate")
	_add_node_class("Generate normals", "GDProcGenNormals")

	# shapes
	_add_node_class("Box", "GDProcBox")

	# modifiers

	# output
	_add_node_class("Surface", "GDProcSurface")
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
