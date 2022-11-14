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
#var is_player_moving = false

func _unhandled_input(event: InputEvent):
	var player_board_position = players_initial_positions[current_player]
	var right_direction = player_board_position + Vector2.RIGHT
	var left_direction = player_board_position + Vector2.LEFT
	var up_direction = player_board_position + Vector2.UP
	var down_direction = player_board_position + Vector2.DOWN
	
	if !is_moving_right and event.is_action_pressed("ui_right") and has_platform(right_direction) and not has_platform_selected:
		is_moving_right = true
		return
	if !is_moving_left and event.is_action_pressed("ui_left") and has_platform(left_direction) and not has_platform_selected:
		is_moving_left = true
		return
	if !is_moving_up and event.is_action_pressed("ui_up") and has_platform(up_direction) and not has_platform_selected:
		is_moving_up = true
		return
	if !is_moving_down and event.is_action_pressed("ui_down") and has_platform(down_direction) and not has_platform_selected:
		is_moving_down = true
		return
		
	
	if event.is_action_pressed("ui_select") and not has_platform_selected:
		has_platform_selected = true
		var index = platforms_initial_positions.find(players_initial_positions[current_player])
		platforms_instances[index].get_node("Crosshair").visible = true
		return
	if event.is_action_pressed("ui_select") and has_platform_selected:
		has_platform_selected = false
		var index = platforms_initial_positions.find(players_initial_positions[current_player])
		platforms_instances[index].get_node("Crosshair").visible = false
		return
	
	if event.is_action_pressed("ui_right") and has_platform_selected and not is_moving:
		is_moving = true
		var next_square = right_direction + Vector2.RIGHT
		while not has_platform(next_square) or is_out_of_bounds(next_square):
			next_square + Vector2.RIGHT
			pass
			
		var destination = next_square + Vector2.LEFT
#		for i in range(platforms_initial_positions.size()):
#			pass

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


func has_platform(v: Vector2) -> bool:
	return platforms_initial_positions.find(v) != -1


func get_screen_player_position(v: Vector2) -> Vector2:
	return initial_position + v * platform_size


var board_range = range(0, 7)
func is_out_of_bounds(v: Vector2) -> bool:
	return not (v.x in board_range) or not (v.y in board_range)
