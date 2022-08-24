extends Node2D

const player_size = 22
const platform_size = 48

const board_padding = 5

const half_platform_size = platform_size / 2
const platform_center = half_platform_size + board_padding
const player_center = platform_center - player_size / 2

onready var PlatformScene: PackedScene = preload("res://scenes/platform.tscn")
onready var PlayerScene: PackedScene = preload("res://scenes/player.tscn")

onready var player_one = PlayerScene.instance()
onready var player_two = PlayerScene.instance()
onready var player_three = PlayerScene.instance()
onready var player_four = PlayerScene.instance()

onready var player = player_one
var turn = 0

const players_last_positions = []

const platforms_positions = [
	# Vector2(2, 3),
	# Vector2(4, 3),

	# Top Left
	Vector2(0, 0),
	Vector2(1, 0),
	Vector2(0, 1),
	
	# Top Right
	Vector2(5, 0),
	Vector2(6, 0),
	Vector2(6, 1),

	# Bottom Left
	Vector2(0, 5),
	Vector2(1, 6),
	Vector2(0, 6),

	# Bottom Right
	Vector2(5, 6),
	Vector2(6, 5),
	Vector2(6, 6),
]

const platforms_instances = []

var is_platform_selected = false
var selected_platform_position = Vector2.ZERO
var is_moving = false
var has_won = false
var is_doubles = true
var started = false
var debug = false

var initial_position = Vector2(player_center, player_center)
var initial_position_test = Vector2(player_center + platform_size * 2, player_center + platform_size * 3)
		
func init_platforms():
	for position in platforms_positions:
		var platform: Sprite = PlatformScene.instance()
		platform.position = new_platform_position(position)
		platforms_instances.push_back(platform)
		add_child(platform)
	
func init_players():
	add_child(player_one)
	player_one.position = initial_position_test if debug else initial_position
	player_one.modulate = "#dc2626"
	players_last_positions.push_back(Vector2(2, 3) if debug else Vector2.ZERO)
	
	add_child(player_two)
	player_two.position = Vector2(initial_position.x + platform_size * 6, initial_position.y + platform_size * 6)
	player_two.modulate = "#2563eb"
	players_last_positions.push_back(Vector2(6, 6))
	
	player.select()

func add_players():
	add_child(player_three)
	player_three.position = Vector2(initial_position.x + platform_size * 0, initial_position.y + platform_size * 6)
	player_three.modulate = "#16a34a"
	players_last_positions.push_back(Vector2(0, 6))
	
	add_child(player_four)
	player_four.position = Vector2(initial_position.x + platform_size * 6, initial_position.y + platform_size * 0)
	player_four.modulate = "#c026d3"
	players_last_positions.push_back(Vector2(6, 0))

func _unhandled_input(_event: InputEvent):
	if not started:
		return
	var left_position = players_last_positions[turn] + Vector2.LEFT
	var right_position = players_last_positions[turn] + Vector2.RIGHT
	var up_position = players_last_positions[turn] + Vector2.UP
	var down_position = players_last_positions[turn] + Vector2.DOWN
	
	var pressed_left = Input.is_action_just_pressed("ui_left")
	var pressed_right = Input.is_action_just_pressed("ui_right")
	var pressed_up = Input.is_action_just_pressed("ui_up")
	var pressed_down = Input.is_action_just_pressed("ui_down")
	var pressed_enter = Input.is_action_just_pressed("ui_select")
	
	if has_won and pressed_enter:
		$CanvasLayer/MainPanel.visible = true
		$CanvasLayer/Panel.visible = false
		started = false
		has_won = false
		player_one.position = initial_position_test
		player_two.position = Vector2(initial_position.x + platform_size * 6, initial_position.y + platform_size * 6)
		players_last_positions.clear()
		players_last_positions.push_back(Vector2(2, 3))
		players_last_positions.push_back(Vector2(6, 6))
		remove_child(player_one)
		remove_child(player_two)
		remove_child(player_three)
		remove_child(player_four)
		for platform_instance in platforms_instances:
			remove_child(platform_instance)
#		player_three.position = Vector2(initial_position.x + platform_size * 0, initial_position.y + platform_size * 6)
#		player_four.position = Vector2(initial_position.x + platform_size * 6, initial_position.y + platform_size * 0)
		
		player = player_one
		var i = 0
		for platform in platforms_instances:
			platform.position = new_platform_position(platforms_positions[i])
			i += 1

	if has_won:
		return

	if not is_platform_selected:
		if pressed_left and left_position in platforms_positions and not (left_position in players_last_positions):
			player.position.x -= platform_size
			players_last_positions[turn].x -= 1
		if pressed_right and right_position in platforms_positions and not (right_position in players_last_positions):
			player.position.x += platform_size
			players_last_positions[turn].x += 1
		if pressed_up and up_position in platforms_positions and not (up_position in players_last_positions):
			player.position.y -= platform_size
			players_last_positions[turn].y -= 1
		if pressed_down and down_position in platforms_positions and not (down_position in players_last_positions):
			player.position.y += platform_size
			players_last_positions[turn].y += 1
		if pressed_enter:
			is_platform_selected = true
			var current_position_index = platforms_positions.find(players_last_positions[turn])
			platforms_instances[current_position_index].select()

	elif is_platform_selected and not is_moving:
		if pressed_left and not left_position in platforms_positions and not is_out_of_bounds(left_position):
			move_platform(Vector2.LEFT)
		if pressed_right and not right_position in platforms_positions and not is_out_of_bounds(right_position):
			move_platform(Vector2.RIGHT)
		if pressed_up and not up_position in platforms_positions and not is_out_of_bounds(up_position):
			move_platform(Vector2.UP)
		if pressed_down and not down_position in platforms_positions and not is_out_of_bounds(down_position):
			move_platform(Vector2.DOWN)
		if pressed_enter:
			is_platform_selected = false
			var current_position_index = platforms_positions.find(players_last_positions[turn])
			platforms_instances[current_position_index].deselect()


func move_platform(direction: Vector2) -> void:
	is_moving = true
	var selected_platform_final_position = players_last_positions[turn] + direction
	while(not (selected_platform_final_position in platforms_positions) and not is_out_of_bounds(selected_platform_final_position)):
		selected_platform_final_position += direction
	
	# move platform
	var corrected_position = selected_platform_final_position - direction
	var current_position_index = platforms_positions.find(players_last_positions[turn])
	platforms_positions[current_position_index] = corrected_position
	platforms_instances[current_position_index].position = new_platform_position(corrected_position)
	platforms_instances[current_position_index].deselect()
	
	# move player
	var result_squares = corrected_position - players_last_positions[turn]
	player.position += result_squares * platform_size
	players_last_positions[turn] += result_squares

	player.deselect()

	if platforms_positions[current_position_index] == Vector2(3, 3):
		has_won = true
		is_platform_selected = false
		is_moving = false
		player = player_one
		$CanvasLayer/Panel.visible = true
		var winner_double = 0 if turn < 2 else 2
		var doubles_text = "Player %d and Player %d won!" % [winner_double + 1, winner_double + 2]
		var singles_text = "Player %d won!" % (turn + 1)
		$CanvasLayer/Panel/Won.text = doubles_text if is_doubles else singles_text
		return
	
	turn = (turn + 1) % 4 if is_doubles else 2
	match turn:
		0: player = player_one
		1: player = player_two
		2: player = player_three
		3: player = player_four

	player.select()
		
	is_platform_selected = false
	is_moving = false

func new_platform_position(vector: Vector2) -> Vector2:
	return vector * platform_size + Vector2(platform_center, platform_center)

var board_range = range(0, 7)
func is_out_of_bounds(v: Vector2) -> bool:
	return not (v.x in board_range) or not (v.y in board_range)
