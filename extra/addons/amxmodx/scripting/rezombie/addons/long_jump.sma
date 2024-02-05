#include <amxmodx>
#include <fakemeta>
#include <reapi>
#include <rezombie>

new const ITEM_LONG_JUMP_SPRITE[] = "item_longjump"
new const STATUS_ICON_SPRITE[] = "item_longjump"
new const ACTIVATE_SOUND[] = "zombie_plague/boss_shokwave.wav"

new ModelIndex_Trail

public plugin_precache() {
	register_plugin("Long Jump", "1.0", "fl0wer")
	precache_sound(ACTIVATE_SOUND)
	ModelIndex_Trail = precache_model("sprites/laserbeam.spr")
}

@give_long_jump(player) {
	send_item_pickup(player, ITEM_LONG_JUMP_SPRITE)
}

@long_jump_state(player, LongJumpState:longJumpState) {
	switch (longJumpState) {
		case long_jump_none: {
			send_status_icon(player, 0, STATUS_ICON_SPRITE)
		}
		case long_jump_cooldown: {
			send_status_icon(player, 1, STATUS_ICON_SPRITE, "grey")
		}
		case long_jump_ready: {
			send_status_icon(player, 1, STATUS_ICON_SPRITE, "rgb(255, 160, 0)")
		}
	}
}

@long_jump_activated(player) {
	new Float:force = float(get_long_jump_var(player, "force"))
	new Float:viewAngle[3]
	new Float:velocity[3]
	new Float:viewForward[3]
	get_entvar(player, var_v_angle, viewAngle)
	engfunc(EngFunc_MakeVectors, viewAngle)
	global_get(glb_v_forward, viewForward)
	velocity[0] = viewForward[0] * force
	velocity[1] = viewForward[1] * force
	velocity[2] = viewForward[2] * float(get_long_jump_var(player, "height"))
	set_entvar(player, var_velocity, velocity)
	new cooldown = get_long_jump_var(player, "cooldown")
	if (cooldown != 0) {
		set_long_jump_var(player, "cooldown_timer", cooldown)
		set_long_jump_var(player, "next_state_time", get_gametime() + 1.0)
		set_long_jump_var(player, "state", long_jump_cooldown)
	}
	longJumpEffects(player)
}

@long_jump_cooldown_timer(player, timer) {
	update_player_info(player)
}

longJumpEffects(player) {
	//new Float:origin[3]
	new Float:punchAngle[3]
	//get_entvar(player, var_origin, origin)
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] = -5.0
	set_entvar(player, var_punchangle, punchAngle)
	rg_set_animation(player, PLAYER_SUPERJUMP)
	rh_emit_sound2(player, 0, CHAN_ITEM, ACTIVATE_SOUND)
	message_begin_f(MSG_PVS, SVC_TEMPENTITY, entity_origin(player))
	message_beam_follow(player, ModelIndex_Trail, 1, 5, "yellow")
}

Float:entity_origin(entity) {
	new Float:origin[3]
	get_entvar(entity, var_origin, origin)
	return origin
}
