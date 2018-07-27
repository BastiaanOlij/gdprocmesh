tool
extends DirectionalLight

var val = 1

func _get_property_list():
	var arr = Array()
	
	var dict = Dictionary()
	dict['name'] = 'test'
	dict['type'] = TYPE_INT
	arr.push_back(dict)
	
	return arr
	
func _get(prop):
	if (prop == 'test'):
		return val
	return null

func _set(prop, value):
	if (prop == 'test'):
		val = value
		return true
	
	return false


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
