extends Node

onready var main_menu = preload("res://screens/MainMenu.tscn").instance()
onready var game = preload("res://screens/game/Game.tscn").instance()
onready var canvas = $CanvasLayer
onready var animation_transition = $CanvasLayer/ColorRect/AnimationPlayer

func _ready():
	OS.set_window_title("Rink Slider")
	OS.window_maximized = true
	canvas.visible = false
	add_child(main_menu)
	

func _unhandled_input(event: InputEvent):
	if event.is_action_pressed("ui_accept"):
		yield(get_tree().create_timer(0.4), "timeout")
		canvas.visible = true
		animation_transition.play_backwards("transition")
		yield(get_tree().create_timer(1.5), "timeout")
		remove_child(main_menu)
		add_child(game)
		animation_transition.play("transition")
		yield(get_tree().create_timer(1.5), "timeout")
		pass
