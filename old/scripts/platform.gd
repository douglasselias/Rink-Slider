extends Sprite

onready var border = $"SelectedBorder"

func _ready():
	border.visible = false

func select():
	border.visible = true

func deselect():
	border.visible = false
