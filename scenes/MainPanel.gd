extends Panel

enum OPTIONS { START, HOW_TO_PLAY, NUMBER_OF_PLAYERS }
var selected_option = OPTIONS.START
onready var current_animation = $Start/Blink

func _ready():
	$Radio4.set_pressed_no_signal(true)
	$Check4.visible = true
	current_animation.play("Blink")
	
func _unhandled_input(_e):
	if get_node("/root/Main").started:
		return
	if Input.is_action_just_pressed("ui_accept"):
		visible = false
		get_node("/root/Main").started = true
		get_node("/root/Main").is_doubles = $Radio4.pressed
		get_node("/root/Main").init_platforms()
		get_node("/root/Main").init_players()
		if $Radio4.pressed:
			get_node("/root/Main").add_players()
	if Input.is_action_just_pressed("ui_up"):
		match selected_option:
#			OPTIONS.HOW_TO_PLAY: selected_option = OPTIONS.START
			OPTIONS.NUMBER_OF_PLAYERS: selected_option = OPTIONS.START
	if Input.is_action_just_pressed("ui_down"):
		match selected_option:
			OPTIONS.START: selected_option = OPTIONS.NUMBER_OF_PLAYERS
#			OPTIONS.HOW_TO_PLAY: selected_option = OPTIONS.NUMBER_OF_PLAYERS
	
	current_animation.stop()

	match selected_option:
		OPTIONS.START: current_animation = $Start/Blink
#		OPTIONS.HOW_TO_PLAY: current_animation = $HowToPlay/Blink
		OPTIONS.NUMBER_OF_PLAYERS: current_animation = $NumberOfPlayers/Blink
		
	current_animation.play("Blink")
	
	if selected_option != OPTIONS.NUMBER_OF_PLAYERS:
		return
	if Input.is_action_just_pressed("ui_left"):
		$Radio2.set_pressed_no_signal(true)
		$Radio4.set_pressed_no_signal(false)
		$Check2.visible = true
		$Check4.visible = false
	if Input.is_action_just_pressed("ui_right"):
		$Radio2.set_pressed_no_signal(false)
		$Radio4.set_pressed_no_signal(true)
		$Check2.visible = false
		$Check4.visible = true
		

