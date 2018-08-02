tool
extends VBoxContainer

var procmesh = null

func edit_mesh(p_procmesh):
	if p_procmesh:
		procmesh = p_procmesh
		
		_update_graph()
	else:
		procmesh = null
		hide()

func _update_graph():
	# clear graph
	$GraphEdit.clear_connections()
	for child in $GraphEdit.get_children():
		child.queue_free()
	
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
		gn.set_name(String(node_id))
		
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
				
				print(String(i) + ", left: " + String(left_is_valid) + ", right: " + String(right_is_valid))
				
				
				gn.set_slot(i, left_is_valid, left_type, left_color, right_is_valid, right_type, right_color)

func _remove_node(p_node_id):
	pass

func _dragged_node(p_node_id):
	pass

func _ready():
	pass # Replace with function body.

func _input(event):
	pass