extends Control

var qty_selected = 2

onready var p2 = $Players2
onready var p4 = $Players4
onready var click_sfx = $Select
onready var change_sfx = $ChangeSelection

const tween_speed = 0.1
var initial_scale = Vector2(0.49, 0.49)
var final_scale = Vector2(0.63, 0.63)


func _ready():
	VisualServer.set_default_clear_color(Color.gold)
	in_anim(p2)


func _unhandled_input(event: InputEvent):
	if event.is_action_pressed("ui_accept"):
		var check = p2.get_child(0)
		sel_anim(check)
		var s = p2 if qty_selected == 2 else p4
		out_anim(s)
		in_anim(s)
		click_sfx.play()
		return
	if event.is_action_pressed("ui_left") and qty_selected == 4:
		qty_selected = 2
#		p4.get_child(0).play("RESET")
#		p2.get_child(0).play("selected")
		in_anim(p2).play()
		out_anim(p4).play()
		change_sfx.play()
		return
	
	if event.is_action_pressed("ui_right") and qty_selected == 2:
		qty_selected = 4
#		p2.get_child(0).play("RESET")
#		p4.get_child(0).play("selected")
		in_anim(p4).play()
		out_anim(p2).play()
		change_sfx.play()
		return
	
#	current.get_child(0).play("selected")


func in_anim(sprite: Sprite) -> SceneTreeTween:
	var tween = create_tween().set_trans(Tween.TRANS_BACK).set_ease(Tween.EASE_IN_OUT)
	tween.tween_property(sprite, "scale", initial_scale, tween_speed)
	tween.tween_property(sprite, "scale", final_scale, tween_speed)
#	tween.stop()
	return tween


func out_anim(sprite: Sprite) -> SceneTreeTween:
	var tween = create_tween().set_trans(Tween.TRANS_BACK).set_ease(Tween.EASE_IN_OUT)
	tween.tween_property(sprite, "scale", final_scale, tween_speed)
	tween.tween_property(sprite, "scale", initial_scale, tween_speed)
#	tween.stop()
	return tween

func sel_anim(sprite: Sprite) -> SceneTreeTween:
	var tween = create_tween().set_trans(Tween.TRANS_BACK).set_ease(Tween.EASE_IN_OUT)
	var modulate_speed = 0.1
	tween.tween_property(sprite, "modulate", Color(0.090196, 0.698039, 0.215686, 0), modulate_speed)
	tween.tween_property(sprite, "modulate", Color(0.090196, 0.698039, 0.215686, 1), modulate_speed)
#	tween.set_parallel()
	
	var scale_speed = 0.1
	tween.tween_property(sprite, "scale", Vector2(2, 2), scale_speed)
	tween.tween_property(sprite, "scale", Vector2(1, 1), scale_speed)
	

#	tween.stop()
	return tween
