tool
extends GraphNode

signal changed

var proc_mesh = null
var node_id = -1
var node = null

func set_proc_node(p_proc_mesh, p_node_id):
	var slot_offset = 0
	
	proc_mesh = p_proc_mesh
	node_id = p_node_id
	node = proc_mesh.get_node(p_node_id)
	
	set_offset(node.position)
	set_title(node.get_type_name())
	set_name(String(p_node_id))
	
	$Name_Container/Name_Edit.set_text(node.get_name())
	slot_offset += 1

func _on_ProcNode_close_request():
	if proc_mesh:
		proc_mesh.remove_node(node_id)
		emit_signal("changed")

func _on_ProcNode_dragged(from, to):
	if node:
		node.set_position(to)

func _on_Name_Edit_text_changed(new_text):
	if node:
		node.set_name(new_text)
		# set_name is internal and doesn't trigger an update when changed so workaround..
		node._touch()
		node.emit_signal("changed")

func _on_Name_Edit_text_entered(new_text):
	if node:
		node.set_name(new_text)
		# set_name is internal and doesn't trigger an update when changed so workaround..
		node._touch()
		node.emit_signal("changed")
