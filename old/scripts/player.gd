extends Sprite

const player_selected_shader: Shader = preload("res://old/shaders/player.gdshader")
var shader_material = ShaderMaterial.new()

func _ready():
	shader_material.set_shader_param("outline_width", 0)
	shader_material.set_shader_param("outline_color", Color.black)
	
	shader_material.set_shader(player_selected_shader)
	set_material(shader_material)

func select():
	shader_material.set_shader_param("outline_width", 4)

func deselect():
	shader_material.set_shader_param("outline_width", 0)
