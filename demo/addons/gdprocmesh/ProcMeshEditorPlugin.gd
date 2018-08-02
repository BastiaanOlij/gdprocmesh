tool
extends EditorPlugin

var control = null
var button = null

func _enter_tree():
	# Initialization of the plugin goes here
	add_custom_type("GDProcMesh", "ArrayMesh", preload("res://addons/gdprocmesh/gdprocmesh.gdns"), preload("icon_array_mesh.svg"))
	
	control = preload("res://addons/gdprocmesh/ProcMeshEditor.tscn").instance()
	
	button = add_control_to_bottom_panel(control, "Procedural Mesh")
	button.hide()

func _exit_tree():
	# Clean-up of the plugin goes here
	remove_control_from_bottom_panel(control)
	remove_custom_type("GDProcMesh")
	
	control = null
	button = null

func handles(object):
	if object.is_class('ArrayMesh'):
		var script = object.get_script()
		if script and script.resource_path.find('gdprocmesh.gdns') >=0 :
			return true
	return false

func edit(object):
	control.edit_mesh(object)

func make_visible(visible):
	# hide or show our editor
	if visible:
		button.show()
		make_bottom_panel_item_visible(control)
		control.set_process_input(true)
	else:
		if control.is_visible_in_tree():
			hide_bottom_panel()
		button.hide()
		control.set_process_input(false)
		
		# unload object?
