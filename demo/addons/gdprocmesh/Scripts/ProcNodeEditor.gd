tool
extends GraphNode

signal changed

var proc_mesh = null
var node_id = -1
var node = null

func _get_type_color(p_type):
	if p_type == TYPE_REAL:
		return Color("#f34d4d")
	elif p_type == TYPE_INT:
		return Color("#0d6498")
	elif p_type == TYPE_BOOL:
		return Color("#c4780c")
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

func set_proc_node(p_proc_mesh, p_node_id):
	var slot_offset = 0
	
	# copy some basic data
	proc_mesh = p_proc_mesh
	node_id = p_node_id
	node = proc_mesh.get_node(p_node_id)
	
	# init our graph node component
	set_offset(node.position)
	set_title(node.get_type_name())
	set_name(String(p_node_id))
	
	# copy our name
	$Name_Container/Name_Edit.set_text(node.get_node_name())
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
				var prop_arr = Array()
				prop_arr.push_back(prop_name)
				
				if prop_type == TYPE_INT:
					prop_field = LineEdit.new()
					prop_arr.push_back(prop_field)
					prop_field.align = LineEdit.ALIGN_RIGHT
					prop_field.set_text(String(prop_value))
					prop_field.connect("text_entered", self, "_set_node_property", prop_arr)
					prop_field.connect("focus_exited", self, "_exit_node_property", prop_arr)
					prop_field.rect_min_size = Vector2(75.0, 0.0)
				elif prop_type == TYPE_REAL:
					prop_field = LineEdit.new()
					prop_arr.push_back(prop_field)
					prop_field.align = LineEdit.ALIGN_RIGHT
					prop_field.set_text("%0.3f" % prop_value)
					prop_field.connect("text_entered", self, "_set_node_property", prop_arr)
					prop_field.connect("focus_exited", self, "_exit_node_property", prop_arr)
					prop_field.rect_min_size = Vector2(75.0, 0.0)
				elif prop_type == TYPE_BOOL:
					prop_field = CheckBox.new()
					prop_arr.push_back(prop_field)
					prop_field.set_text("on")
					prop_field.pressed = prop_value
					prop_field.connect("toggled", self, "_set_node_property", prop_arr)
					prop_field.rect_min_size = Vector2(75.0, 0.0)
					
				if prop_field:
					hb.add_child(prop_field)
			
			hb.add_spacer(false)
			
			if right_is_valid:
				var label = Label.new()
				label.set_text(node.get_output_connector_name(i))
				label.set_align(Label.ALIGN_RIGHT)
				hb.add_child(label)
				right_type = node.get_output_connector_type(i)
				right_color = _get_type_color(right_type)
			
			add_child(hb)
			set_slot(i + slot_offset, left_is_valid, left_type, left_color, right_is_valid, right_type, right_color)

func _set_node_property(p_value, p_property, p_field):
	if node:
		node.set(p_property, p_value)
		
		# see what it changed to and update our display, need to figure out how...
		var prop_value = node.get(p_property)
		var prop_type = typeof(prop_value)
		if prop_type == TYPE_INT:
			p_field.set_text(String(prop_value))
		elif prop_type == TYPE_REAL:
			p_field.set_text("%0.3f" % prop_value)

func _exit_node_property(p_property, p_field):
	var value = p_field.text
	_set_node_property(value, p_property, p_field)

func _on_ProcNode_close_request():
	if proc_mesh:
		proc_mesh.remove_node(node_id)
		emit_signal("changed")

func _on_ProcNode_dragged(from, to):
	if node:
		node.set_position(to)

func _on_Name_Edit_text_entered(new_text):
	if node:
		# need to change this logic to check if our node name has changed and if so, if the node name isn't already in use
		node.set_node_name(new_text)

func _on_Name_Edit_focus_exited():
	_on_Name_Edit_text_entered($Name_Container/Name_Edit.text)
