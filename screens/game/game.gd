extends Node2D

onready var player = preload("res://player/player.tscn")
onready var platform = preload("res://platform/platform.tscn")

const player_offset_board = 29.5
const initial_position = Vector2(player_offset_board, player_offset_board)
const platform_size = 48

const total_players = 4
var current_player = 0
const player_instances = []
const COLORS = {
	"RED": "#dc2626",
	"BLUE": "#2563eb",
	"GREEN": "#16a34a",
	"PURPLE": "#c026d3",
}
const players_colors = [COLORS.RED, COLORS.BLUE, COLORS.GREEN, COLORS.PURPLE]
#const _players_initial_positions = [
#	initial_position, 
#	initial_position + Vector2(platform_size, 0) * 6,
#	initial_position + Vector2(0, platform_size) * 6,
#	initial_position + Vector2(platform_size, platform_size) * 6
#]
const players_initial_positions = [
	Vector2(0, 0),
	Vector2(6, 6),
	Vector2(0, 6),
	Vector2(6, 0),
]

const total_platforms = 12
const platforms_initial_positions = [
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

var has_platform_selected = false
var is_moving = false

func _ready():
#	OS.window_maximized = true
	
	for i in range(total_platforms):
		var platform_instance = platform.instance()
		add_child(platform_instance)
		platforms_instances.push_back(platform_instance)
		platform_instance.position = initial_position + platforms_initial_positions[i] * platform_size

	for i in range(total_players):
		var player_instance = player.instance()
		add_child(player_instance)
		player_instances.push_back(player_instance)
		player_instance.self_modulate = players_colors[i]
		player_instance.position = get_screen_player_position(players_initial_positions[i])
		
	player_instances[0].get_node("Crosshair").visible = true

var is_moving_right = false
var is_moving_left = false
var is_moving_up = false
var is_moving_down = false

var is_platform_moving_right = false
var is_platform_moving_left = false
var is_platform_moving_up = false
var is_platform_moving_down = false
#var is_player_moving = false
var destination = Vector2.ZERO
var curr_plat_idx = -1
var base_direction = Vector2.ZERO
var direction = Vector2.ZERO
var inverse_direction = Vector2.ZERO


func _unhandled_input(event: InputEvent):
	var player_board_position = players_initial_positions[current_player]
	var right_direction = player_board_position + Vector2.RIGHT
	var left_direction = player_board_position + Vector2.LEFT
	var up_direction = player_board_position + Vector2.UP
	var down_direction = player_board_position + Vector2.DOWN
	
	var pressed_right = event.is_action_pressed("ui_right")
	var pressed_left = event.is_action_pressed("ui_left")
	var pressed_up = event.is_action_pressed("ui_up")
	var pressed_down = event.is_action_pressed("ui_down")
	var pressed_select = event.is_action_pressed("ui_select")
	
	if !is_moving_right and pressed_right and has_platform(right_direction) and not has_platform_selected:
		is_moving_right = true
		return
	if !is_moving_left and pressed_left and has_platform(left_direction) and not has_platform_selected:
		is_moving_left = true
		return
	if !is_moving_up and pressed_up and has_platform(up_direction) and not has_platform_selected:
		is_moving_up = true
		return
	if !is_moving_down and pressed_down and has_platform(down_direction) and not has_platform_selected:
		is_moving_down = true
		return
		
	
	if pressed_select and not has_platform_selected:
		has_platform_selected = true
		var index = platforms_initial_positions.find(players_initial_positions[current_player])
		platforms_instances[index].get_node("Crosshair").visible = true
		return
	if pressed_select and has_platform_selected:
		has_platform_selected = false
		var index = platforms_initial_positions.find(players_initial_positions[current_player])
		platforms_instances[index].get_node("Crosshair").visible = false
		return
	

	if !is_platform_moving_right and pressed_right and has_platform_selected and not is_moving:
		is_moving = true
		is_platform_moving_right = true
		direction = Vector2.RIGHT
		inverse_direction = Vector2.LEFT
		base_direction = right_direction
		move()
		return

	if !is_platform_moving_left and pressed_left and has_platform_selected and not is_moving:
		is_moving = true
		is_platform_moving_left = true
		direction = Vector2.LEFT
		inverse_direction = Vector2.RIGHT
		base_direction = left_direction
		move()
		return
	
	if !is_platform_moving_up and pressed_up and has_platform_selected and not is_moving:
		is_moving = true
		is_platform_moving_up = true
		direction = Vector2.UP
		inverse_direction = Vector2.DOWN
		base_direction = up_direction
		move()
		return
	if !is_platform_moving_down and pressed_down and has_platform_selected and not is_moving:
		is_moving = true
		is_platform_moving_down = true
		direction = Vector2.DOWN
		inverse_direction = Vector2.UP
		base_direction = down_direction
		move()
		return


func _process(delta: float):
	var player_board_position = players_initial_positions[current_player]
	var right_direction = player_board_position + Vector2.RIGHT
	var left_direction = player_board_position + Vector2.LEFT
	var up_direction = player_board_position + Vector2.UP
	var down_direction = player_board_position + Vector2.DOWN

	var direction = Vector2.ZERO
	if is_moving_left:
		direction = left_direction
	if is_moving_right:
		direction = right_direction
	if is_moving_up:
		direction = up_direction
	if is_moving_down:
		direction = down_direction
		
	if is_moving_left or is_moving_right or is_moving_up or is_moving_down:
		var screen_position = get_screen_player_position(direction)
		player_instances[current_player].position = lerp(player_instances[current_player].position, screen_position, 0.3)
		var curr_pos = player_instances[current_player].position
		if is_equal_approx(curr_pos.x, screen_position.x) and is_equal_approx(curr_pos.y, screen_position.y):
			is_moving_left = false
			is_moving_right = false
			is_moving_up = false
			is_moving_down = false
			players_initial_positions[current_player] = direction
			player_instances[current_player].position = screen_position
	
	if is_platform_moving_right or is_platform_moving_left or is_platform_moving_up or is_platform_moving_down:
		var screen_position = get_screen_player_position(destination)
		player_instances[current_player].position = lerp(player_instances[current_player].position, screen_position, 0.3)
		var curr_pos = player_instances[current_player].position
		
		var screen_position_plat = new_platform_position(destination)
		platforms_instances[curr_plat_idx].position = lerp(platforms_instances[curr_plat_idx].position, screen_position_plat, 0.3)
		var curr_plat_pos = platforms_instances[curr_plat_idx].position
		
		if is_equal_approx(curr_pos.x, screen_position.x) and is_equal_approx(curr_pos.y, screen_position.y) and is_equal_approx(curr_plat_pos.x, screen_position_plat.x) and is_equal_approx(curr_plat_pos.x, screen_position_plat.x):
			is_platform_moving_left = false
			is_platform_moving_right = false
			is_platform_moving_up = false
			is_platform_moving_down = false
			is_moving = false
			
			player_instances[current_player].position = screen_position
			platforms_instances[curr_plat_idx].position = screen_position_plat
			
			current_player = (current_player + 1) % total_players
			player_instances[current_player].get_node("Crosshair").visible = true
			has_platform_selected = false
		


func has_platform(v: Vector2) -> bool:
	return platforms_initial_positions.find(v) != -1


func get_screen_player_position(v: Vector2) -> Vector2:
	return initial_position + v * platform_size


var board_range = range(0, 7)
func is_out_of_bounds(v: Vector2) -> bool:
	return not (v.x in board_range) or not (v.y in board_range)


func new_platform_position(v: Vector2) -> Vector2:
	return v * platform_size + Vector2(player_offset_board, player_offset_board)


func move():
	var next_square = base_direction + direction
	while not has_platform(next_square) or is_out_of_bounds(next_square):
		next_square += direction
			
	destination = next_square + inverse_direction
	players_initial_positions[current_player] = destination
	curr_plat_idx = platforms_initial_positions.find(base_direction + inverse_direction)
	platforms_initial_positions[curr_plat_idx] = destination
	platforms_instances[curr_plat_idx].get_node("Crosshair").visible = false
	player_instances[current_player].get_node("Crosshair").visible = false
