tool
extends HBoxContainer

export(Vector3) var vector setget set_vector, get_vector
signal changed_vector(new_vector)

func set_vector(new_vector):
	vector = new_vector
	$X.text = "%0.3f" % vector.x
	$Y.text = "%0.3f" % vector.y
	$Z.text = "%0.3f" % vector.z

func get_vector():
	vector.x = $X.text
	vector.y = $Y.text
	vector.z = $Z.text
	return vector

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.



func _on_X_text_entered(new_text):
	vector.x = float(new_text)
	$X.text = "%0.3f" % vector.x
	emit_signal("changed_vector", vector)

func _on_X_focus_exited():
	_on_X_text_entered($X.text)


func _on_Y_text_entered(new_text):
	vector.y = float(new_text)
	$Y.text = "%0.3f" % vector.y
	emit_signal("changed_vector", vector)

func _on_Y_focus_exited():
	_on_Y_text_entered($Y.text)


func _on_Z_text_entered(new_text):
	vector.z = float(new_text)
	$Z.text = "%0.3f" % vector.z
	emit_signal("changed_vector", vector)

func _on_Z_focus_exited():
	_on_Z_text_entered($Z.text)
