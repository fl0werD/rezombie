#include <amxmodx>
#include <reapi>
#include <rezombie>

new Float:g_nextHudInfoTime[MAX_PLAYERS + 1]
new Float:g_lastHudInfoTime[MAX_PLAYERS + 1]

new Float:rz_playerinfo_hud_pos[2]
new rz_playerinfo_hud_hp

/**
 * Start settings
 */

new const HUMAN_DAMAGE_DELT = 1000

/**
 * End of settings
 */

// Human
// Blue screen fade when frag
// Red screen fade when killed

// extra jumps to jump?

new const MAX_SCORE_VALUE = 10000

new Class:HumanClass
new Float:HumanDamage[MAX_PLAYERS + 1]

new Class:ZombieClass
new ZombieHits[MAX_PLAYERS + 1]

new Currency:MoneyCurrency
new Currency:AmmoPacksCurrency

public plugin_init() {
	register_plugin("Addon: Player Info", "1.0.0", "fl0wer")

	register_message(get_user_msgid("Money"), "@Message_Money")
	register_message(get_user_msgid("SpecHealth"), "@Message_SpecHealth")
	register_message(get_user_msgid("SpecHealth2"), "@Message_SpecHealth2")

	RegisterHookChain(RG_CBasePlayer_Spawn, "@Player_Spawn_Post", true)
	//RegisterHookChain(RG_CBasePlayer_TakeDamage, "@Player_TakeDamage_Post", true)
	RegisterHookChain(RG_CBasePlayer_Killed, "@Player_Killed_Post", true)
	RegisterHookChain(RG_CBasePlayer_UpdateClientData, "@Player_UpdateClientData_Post", true)

	bind_convar(float, rz_playerinfo_hud_pos[0], "rz_playerinfo_hud_x", "-1.0", _, "", true, -1.0, true, 1.0)
	bind_convar(float, rz_playerinfo_hud_pos[1], "rz_playerinfo_hud_y", "0.9", _, "", true, -1.0, true, 1.0)
	bind_convar(num, rz_playerinfo_hud_hp, "rz_playerinfo_hud_hp", "1", _, "", true, 0.0, true, 1.0)

	register_message(get_user_msgid("ScoreInfo"), "@Message_ScoreInfo")
	RegisterHookChain(RG_CBasePlayer_TakeDamage, "@Player_TakeDamage_Post", true)

	HumanClass = find_class("human")
	ZombieClass = find_class("zombie")

	MoneyCurrency = find_currency("money")
	AmmoPacksCurrency = find_currency("ammo_packs")

	register_clcmd("say /st", "@s")
	//register_message(get_user_msgid("ScreenFade"), "@Message_ScreenFade")
}

@Message_ScreenFade() {
	return PLUGIN_HANDLED
}

@s(player) {
	new text[196]
	//formatex(text, charsmax(text), "1 %c1: %p2\n2  %h: %i3%%")//, player, floatround(get_entvar(player, var_health), floatround_floor))
	message_begin(MSG_ONE, get_user_msgid("StatusText"), _, player)
	write_byte(0)
	write_string("1 %p2^n2 [%i3 HP]")
	message_end()

	message_begin(MSG_ONE, get_user_msgid("StatusValue"), _, player)
	write_byte(1)
	write_short(2)
	message_end()

	message_begin(MSG_ONE, get_user_msgid("StatusValue"), _, player)
	write_byte(2)
	write_short(1)
	message_end()

	message_begin(MSG_ONE, get_user_msgid("StatusValue"), _, player)
	write_byte(3)
	write_short(1)
	message_end()
}

public client_putinserver(player) {
	g_nextHudInfoTime[player] = 0.0
	HumanDamage[player] = 0.0
	ZombieHits[player] = 0
}

public rz_class_change_post(player, attacker, class) {
	g_nextHudInfoTime[player] = get_gametime() + 0.1
}

public rz_subclass_change_post(player, subclass) {
	g_nextHudInfoTime[player] = get_gametime() + 0.1
}

@change_class_post(player, Class:class, attacker) {
	sendFakeScoreInfoToCatch(player)
}

@Message_Money(message, dest, player) {
	if (get_msg_arg_int(2) == 0) {
		return PLUGIN_CONTINUE
	}
	return PLUGIN_HANDLED
}

@Message_SpecHealth(message, dest, player) {
	new observerTarget = get_member(player, m_hObserverTarget)
	new Float:health = get_entvar(observerTarget, var_health)
	new Float:maxHealth = get_entvar(observerTarget, var_max_health)
	if (health < 0.0) {
		health = 0.0
	}
	set_msg_arg_int(1, ARG_BYTE, floatround((health / maxHealth) * 100.0))
}

@Message_SpecHealth2(message, dest, player) {
	new observerTarget = get_msg_arg_int(2)
	new Float:health = get_entvar(observerTarget, var_health)
	new Float:maxHealth = get_entvar(observerTarget, var_max_health)
	set_msg_arg_int(1, ARG_BYTE, floatround((health / maxHealth) * 100.0))
}

@Player_Spawn_Post(player) {
	if (!is_user_alive(player)) {
		return
	}
	g_nextHudInfoTime[player] = get_gametime() + 0.2
}
/*
@Player_TakeDamage_Post(player, inflictor, attacker, Float:damage, bitsDamageType) {
	if (!rg_is_player_can_takedamage(player, attacker)) {
		return
	}
	if (!is_user_alive(player)) {
		return
	}
	showPlayerInfoHud(player)
}
*/
@Player_Killed_Post(player, attacker, gib) {
	//ClearSyncHud(player, g_hudSync_Info)
}

@Player_UpdateClientData_Post(player) {
	if (!g_nextHudInfoTime[player]) {
		return
	}
	if (!is_user_alive(player)) {
		return
	}
	new Float:time = get_gametime()
	if (g_nextHudInfoTime[player] > time) {
		return
	}
	g_nextHudInfoTime[player] = time + 1.0
	showPlayerInfoHud(player)
}

showPlayerInfoHud(player) {
	new Class:class = get_player_var(player, "class")
	if (!class) {
		return
	}
	new len
	new text[512]
	SetGlobalTransTarget(player)
	if (rz_playerinfo_hud_hp) {
		add_formatex("%l: %d", "HEALTH", get_player_var(player, "health"))
	}
	//if (rz_main_ammopacks_enabled())
	//	add_formatex("[%l: %d]^n", "RZ_AMMOPACKS", get_member(player, m_iAccount))
	new hudColor[MAX_CLASS_VAR_LENGTH]
	new Subclass:subclass = get_player_var(player, "subclass", class)
	if (subclass) {
		add_formatex("^n%l: %l", get_class_var_string(class, "name"), get_subclass_var_string(subclass, "name"))
		hudColor = get_subclass_var_string(subclass, "hud_color")
	} else {
		add_formatex("^n%l: %l", "CLASS", get_class_var_string(class, "name"))
		hudColor = get_class_var_string(class, "hud_color")
	}
	if (class == HumanClass) {
		new humanDamageInt = floatround(HumanDamage[player], floatround_floor)
		add_formatex( \
			"^nОчки: %d (урон: %d/%d)", \
			getHumanScore(humanDamageInt), \
			humanDamageInt % HUMAN_DAMAGE_DELT, \
			HUMAN_DAMAGE_DELT \
		)
		if (MoneyCurrency) {
			add_formatex("^nДеньги: %d", get_player_var(player, "currency", MoneyCurrency))
		}
	} else if (class == ZombieClass) {
		add_formatex("^nОчки: %d", ZombieHits[player])
		if (AmmoPacksCurrency) {
			add_formatex("^nКредиты: %d", get_player_var(player, "currency", AmmoPacksCurrency))
		}
	}
	new maxJumps = get_jumps_var(player, "maximum")
	if (maxJumps > 1) {
		add_formatex("^nПрыжков: %d/%d", get_jumps_var(player, "count"), maxJumps)
	} else if (maxJumps == -1) {
		add_formatex("^nПрыжков: %d/∞", get_jumps_var(player, "count"))
	}
	switch (get_long_jump_var(player, "state")) {
		case long_jump_cooldown: {
			add_formatex("^nДлинный прыжок: через %d сек.", get_long_jump_var(player, "cooldown_timer"))
		}
		case long_jump_ready: {
			add_formatex("^nДлинный прыжок: готов")
		}
	}
	send_hud(
		player,
		PLAYER_INFO_CHANNEL,
		hud_params(
			rz_playerinfo_hud_pos[0],
			rz_playerinfo_hud_pos[1],
			hudColor,
			hudColor,
			.holdTime = 5.1
		),
		text
	)
}

@Player_TakeDamage_Post(player, inflictor, attacker, Float:damage, damageType) {
	//client_print(0, print_chat, "0")
	//new bool:isAlive = bool:is_user_alive(player)
	//if (!GetHookChainReturn(ATYPE_INTEGER) && isAlive) {
	//	return
	//}
	//client_print(0, print_chat, "1")
	if (player == attacker || !is_user_connected(attacker)) {
		return
	}
	//client_print(0, print_chat, "2")
	//if (!rg_is_player_can_takedamage(player, attacker)) {
	//	return
	//}
	//client_print(0, print_chat, "3")
	new Class:class = get_player_var(attacker, "class")
	if (class == HumanClass) {
	//client_print(0, print_chat, "4")
		new Float:realDamage
		if (is_user_alive(player)) {
			realDamage = damage
		} else {
			realDamage = damage + Float:get_entvar(player, var_health)
		}
		new Float:oldHumanDamage = HumanDamage[attacker]
		HumanDamage[attacker] += realDamage
		new realDamageInt = floatround(realDamage, floatround_floor)
		add_player_currency(attacker, MoneyCurrency, realDamageInt)
		if (HumanDamage[attacker] - oldHumanDamage >= float(HUMAN_DAMAGE_DELT)) {
			sendFakeScoreInfoToCatch(attacker)
		}
	} else if (class == ZombieClass) {
	//client_print(0, print_chat, "5")
		ZombieHits[attacker]++
		add_player_currency(attacker, AmmoPacksCurrency, 1)
		sendFakeScoreInfoToCatch(attacker)
	}
	//client_print(0, print_chat, "6")
}

@Message_ScoreInfo(message, dest, receiver) {
	new score = 0
	new player = get_msg_arg_int(1)
	new Class:class = get_player_var(player, "class")
	if (class == HumanClass) {
		new humanDamageInt = floatround(HumanDamage[player], floatround_floor)
		score = getHumanScore(humanDamageInt)
	} else if (class == ZombieClass) {
		score = ZombieHits[player]
	}
	set_msg_arg_int(2, ARG_SHORT, min(score, MAX_SCORE_VALUE))
}

getHumanScore(humanDamage) {
	return humanDamage / HUMAN_DAMAGE_DELT
}

sendFakeScoreInfoToCatch(player) {
	emessage_begin(MSG_ALL, get_user_msgid("ScoreInfo"))
	ewrite_byte(player)
	ewrite_short(0)
	ewrite_short(get_member(player, m_iDeaths))
	ewrite_short(0)
	ewrite_short(get_member(player, m_iTeam))
	emessage_end()
}

public plugin_natives() {
	register_native("update_player_info", "@update_player_info")
}

@update_player_info(plugin, argc) {
	enum {
		arg_player = 1
	}

	new player = get_param(arg_player)
	showPlayerInfoHud(player)
	return true
}
