extends Node

var turn_manager = load("res://turn-manager.tres")
var turn = 0

func _ready():
	turn_manager.connect("player_one_turn_started", self, "one")
	turn_manager.connect("player_two_turn_started", self, "two")
	turn_manager.connect("player_three_turn_started", self, "three")
	turn_manager.connect("player_four_turn_started", self, "four")

func _process(delta):
	if (Input.is_action_just_pressed("ui_accept")):
		match turn:
			0: turn_manager.turn = turn_manager.PLAYER_1
			1: turn_manager.turn = turn_manager.PLAYER_2
			2: turn_manager.turn = turn_manager.PLAYER_3
			3: turn_manager.turn = turn_manager.PLAYER_4
		turn = (turn + 1) % 4

func one():
	print("one")

func two():
	print("two")

func three():
	print("three")

func four():
	print("four")
