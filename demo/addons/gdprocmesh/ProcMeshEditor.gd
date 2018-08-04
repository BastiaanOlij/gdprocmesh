tool
extends VBoxContainer

var procmesh = null
var add_button = null
var add_popup = null
var node_classes = Array()

func edit_mesh(p_procmesh):
	if p_procmesh:
		procmesh = p_procmesh
		
		_update_graph()
	else:
		procmesh = null
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
	
	print('Need a color for type ' + String(p_type))
	return Color(0.0, 0.0, 1.0)

func _update_graph():
	# clear graph
	$GraphEdit.clear_connections()
	for child in $GraphEdit.get_children():
		if child.is_class('GraphNode'):
			print('removing: ' + child.name)
			$GraphEdit.remove_child(child)
	
	# lets build our new graph
	var node_ids = procmesh.get_node_id_list()
	for node_id in node_ids:
		print("Found node with id " + String(node_id))
		var node = procmesh.get_node(node_id)
		
		# create a graph node for this
		var gn = GraphNode.new()
		$GraphEdit.add_child(gn)
		
		gn.set_offset(node.position)
		gn.set_title(node.get_type_name())
		gn.set_name('Node_' + String(node_id))
		
		print("Node name: " + gn.name)
		
		var node_arr = Array()
		node_arr.push_back(node_id)
		
		gn.set_show_close_button(true)
		gn.connect("close_request", self, "_remove_node", node_arr, CONNECT_DEFERRED)
		
		gn.connect("dragged", self, "_dragged_node", node_arr)
		
		var input_connector_count = node.get_input_connector_count()
		var output_connector_count = node.get_output_connector_count()
		
		print("Input: " + String(input_connector_count) + ", output: " + String(output_connector_count))
		
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
						prop_arr.push_back(node_id)
						prop_arr.push_back(prop_name)
						prop_arr.push_back(prop_field)
						prop_field.connect("text_entered", self, "_set_node_property", prop_arr)
						prop_field.connect("focus_exited", self, "_exit_node_property", prop_arr)
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
				gn.set_slot(i, left_is_valid, left_type, left_color, right_is_valid, right_type, right_color)
	
	var connections = procmesh.get_connection_list()
	for c in connections:
		print('Connect ' + String(c[0]) + ', ' + String(c[1]) +' - ' + String(c[2]) + ', ' + String(c[3]))
		$GraphEdit.connect_node('Node_' + String(c[2]), c[3], 'Node_' + String(c[0]), c[1])
	
	print(String($GraphEdit.get_connection_list()))

func _set_node_property(p_value, p_node_id, p_property, p_field):
	print("Test " + String(p_value) + " " + String(p_node_id) + " " + String(p_property))
	
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

func _remove_node(p_node_id):
	# need to implement this
	pass

func _dragged_node(p_from, p_to, p_node_id):
	# maybe add this to an undo/redo thing?
	var drag_name = 'Node_' + String(p_node_id)
	print('Drag: ' + drag_name + ' from ' + String(p_from) + ' to ' + String(p_to))
	
	var node = procmesh.get_node(p_node_id)
	if node:
		node.set_position(p_to)

func _add_node(p_type):
	print('Add: ' + node_classes[p_type])
	
	var new_script = NativeScript.new()
	new_script.library = procmesh.script.library
	new_script.set_class_name(node_classes[p_type])
	
	var new_node = Resource.new()
	new_node.script = new_script
	
	if new_node:
		var id_to_use = procmesh.get_free_id()

		# this crashes atm
		print('Add node to mesh')
		new_node.set_position(Vector2(10.0, 50.0))
		procmesh.add_node(new_node, id_to_use)
		
		print('Update graph')
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
	_add_node_class("Vector", "GDProcVector")
	_add_node_class("Box", "GDProcBox")
	_add_node_class("Surface", "GDProcSurface")
	
	pass # Replace with function body.

func _input(event):
	pass