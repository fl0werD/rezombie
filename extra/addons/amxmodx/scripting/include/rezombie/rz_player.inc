#if defined _rz_player_included
	#endinput
#endif

#define _rz_player_included

const MAX_EXTRA_VAR_LENGTH = 32;

enum ExtraType {
	extra_int,
	extra_float,
	extra_string,
};

native update_player_info(player);

forward @player_joining(player);
forward @player_joined(player);


native any:get_preview_var(player, const var[], ...);
native set_preview_var(player, const var[], any:...);


native any:get_player_var(player, const var[], any:...);
native set_player_var(player, const var[], any:...);

native bool:is_player_extra_exists(player, ExtraType:type, const key[]);

native any:get_player_extra(player, ExtraType:type, const key[], any:...);
native set_player_extra(player, ExtraType:type, const key[], any:...);

stock get_player_extra_string(player, const key[]) {
	new value[MAX_EXTRA_VAR_LENGTH];
	get_player_extra(player, extra_string, key, value, charsmax(value));
	return value;
}

native bool:is_valid_attacker(attacker, player, bool:self = false);

native change_player_class(player, Class:class, attacker = 0);
native change_player_subclass(player, Subclass:subclass);
native change_player_flashlight(player, Flashlight:flashlight);
native change_player_night_vision(player, NightVision:nightVision);

native player_joined(player);

native respawn_player(player, timer = 0);

native bool:freeze_player(player, Float:freezeTime, bool:inAir = true);
native bool:is_player_freeze(player);

enum LongJumpState {
	long_jump_none,
	long_jump_cooldown,
	long_jump_ready,
};

native bool:give_long_jump(player, force = 560, height = 300, cooldown = 0);
native bool:remove_long_jump(player);

native any:get_long_jump_var(player, const var[], any:...);
native set_long_jump_var(player, const var[], any:...);

forward @give_long_jump(player);
forward @long_jump_state(player, LongJumpState:longJumpState);
forward @long_jump_activated(player);
forward @long_jump_cooldown_timer(player, timer);

native any:get_jumps_var(player, const var[], any:...);
native set_jumps_var(player, const var[], any:...);

forward @player_jump(player, count);

forward @player_give_default_items(player, Class:class);


native Class:create_class(const handle[], const Team:team);

native any:get_class_var(Class:class, const var[], ...);
native set_class_var(Class:class, const var[], any:...);

const MAX_CLASS_VAR_LENGTH = 32;

stock get_class_var_string(Class:class, const var[]) {
	new value[MAX_CLASS_VAR_LENGTH];
	get_class_var(class, var, value, charsmax(value));
	return value;
}

native bool:is_class_extra_exists(Class:class, ExtraType:type, const key[]);

native any:get_class_extra(Class:class, ExtraType:type, const key[], any:...);
native set_class_extra(Class:class, ExtraType:type, const key[], any:...);

stock get_class_extra_string(Class:class, const key[]) {
	new value[MAX_EXTRA_VAR_LENGTH];
	get_class_extra(class, extra_string, key, value, charsmax(value));
	return value;
}

native classes_count();
native Class:classes_begin();
native Class:classes_end();
native Class:find_class(const handle[]);

forward @change_class_pre(player, Class:class, attacker);
forward @change_class_post(player, Class:class, attacker);

stock Class:checkClassExists(const handle[]) {
	new Class:class = find_class(handle);
	if (class == null) {
		log(Log_Warning, "Class '%s' not found", handle);
	}
	return class;
}


native Subclass:create_subclass(const handle[], Class:class);

native any:get_subclass_var(Subclass:subclass, const var[], ...);
native set_subclass_var(Subclass:subclass, const var[], any:...);

const MAX_SUBCLASS_VAR_LENGTH = 32;

stock get_subclass_var_string(Subclass:sublcass, const var[]) {
	new value[MAX_SUBCLASS_VAR_LENGTH];
	get_subclass_var(sublcass, var, value, charsmax(value));
	return value;
}

native subclasses_count();
native Subclass:subclasses_begin();
native Subclass:subclasses_end();
native Subclass:find_subclass(const handle[]);

forward @change_subclass_pre(player, Subclass:class, attacker);
forward @change_subclass_post(player, Subclass:class, attacker);


native Props:create_props(const handle[]);

native any:get_props_var(Props:props, const var[], ...);
native set_props_var(Props:props, const var[], any:...);

// check 120 maybe it 180?
native Flashlight:create_flashlight(const color[], size, fullDrainTime = 120, fullChargeTime = 20);

native any:get_flashlight_var(Flashlight:flashlight, const var[], any:...);
native set_flashlight_var(Flashlight:flashlight, const var[], any:...);

const MAX_FLASHLIGHT_VAR_LENGTH = 32;

stock get_flashlight_var_string(Flashlight:flashlight, const var[]) {
	new value[MAX_FLASHLIGHT_VAR_LENGTH];
	get_flashlight_var(flashlight, var, value, charsmax(value));
	return value;
}


native NightVision:create_night_vision(const color[], Fog:fog = any:0);

native any:get_night_vision_var(NightVision:nightVision, const var[], any:...);
native set_night_vision_var(NightVision:nightVision, const var[], any:...);

const MAX_NIGHT_VISION_VAR_LENGTH = 32;

stock get_night_vision_var_string(NightVision:nightVision, const var[]) {
	new value[MAX_NIGHT_VISION_VAR_LENGTH];
	get_night_vision_var(nightVision, var, value, charsmax(value));
	return value;
}

enum HudParams {
	Float:hud_x,
	Float:hud_y,
	hud_color1[32],
	hud_color2[32],
	hud_effect,
	Float:hud_fade_in_time,
	Float:hud_hold_time,
	Float:hud_fade_out_time,
	Float:hud_fx_time,
};

native send_hud(player, HudChannel:channel, hudParams[HudParams], const text[], any:...);

enum LargeHudParams {
	Float:large_hud_x,
	Float:large_hud_y,
	large_hud_color[32],
	large_hud_effect,
	Float:large_hud_fade_in_time,
	Float:large_hud_hold_time,
	Float:large_hud_fade_out_time,
	Float:large_hud_fx_time,
};

native send_large_hud(player, hudParams[LargeHudParams], const text[], any:...);

native send_weapon_anim(player, animNumber, body = 0);

stock hud_params(
	Float:x = -1.0,
	Float:y = -1.0,
	const color1[] = "",
	const color2[] = "",
	effect = 0,
	Float:fadeInTime = 0.0,
	Float:holdTime = 0.0,
	Float:fadeOutTime = 0.0,
	Float:fxTime = 0.0
) {
	new hudParams[HudParams]
	hudParams[hud_x] = x
	hudParams[hud_y] = y
	copy(hudParams[hud_color1], charsmax(hudParams[hud_color1]), color1)
	copy(hudParams[hud_color2], charsmax(hudParams[hud_color2]), color2)
	hudParams[hud_effect] = effect
	hudParams[hud_fade_in_time] = fadeInTime
	hudParams[hud_hold_time] = holdTime
	hudParams[hud_fade_out_time] = fadeOutTime
	hudParams[hud_fx_time] = fxTime
	return hudParams
}

stock large_hud_params(
	Float:x = -1.0,
	Float:y = -1.0,
	const color[] = "",
	effect = 0,
	Float:fadeInTime = 0.0,
	Float:fadeOutTime = 0.0,
	Float:holdTime = 0.0,
	Float:fxTime = 0.0
) {
	new hudParams[LargeHudParams]
	hudParams[large_hud_x] = x
	hudParams[large_hud_y] = y
	copy(hudParams[large_hud_color], charsmax(hudParams[large_hud_color]), color)
	hudParams[large_hud_effect] = effect
	hudParams[large_hud_fade_in_time] = fadeInTime
	hudParams[large_hud_hold_time] = holdTime
	hudParams[large_hud_fade_out_time] = fadeOutTime
	hudParams[large_hud_fx_time] = fxTime
	return hudParams
}
