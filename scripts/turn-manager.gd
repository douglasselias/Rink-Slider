extends Resource

class_name TurnManager

enum {PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4}

var turn setget set_turn

signal player_one_turn_started()
signal player_two_turn_started()
signal player_three_turn_started()
signal player_four_turn_started()

func set_turn(value):
	turn = value
	match turn:
		PLAYER_1: emit_signal("player_one_turn_started")
		PLAYER_2: emit_signal("player_two_turn_started")
		PLAYER_3: emit_signal("player_three_turn_started")
		PLAYER_4: emit_signal("player_four_turn_started")
