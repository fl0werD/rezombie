#include <amxmodx>
#include <reapi>
#include <rezombie>

// add total damage for player per death

new Float:TotalDamage[MAX_PLAYERS + 1][MAX_PLAYERS + 1]
new TotalHits[MAX_PLAYERS + 1][MAX_PLAYERS + 1]

new Float:LastDamageTime[MAX_PLAYERS + 1]
new Float:VictimsDamage[MAX_PLAYERS + 1]
new VictimsCount[MAX_PLAYERS + 1]

public plugin_precache() {
	register_plugin("Damager", "1.0.0", "fl0wer")
	RegisterHookChain(RG_CBasePlayer_Spawn, "@Player_Spawn_Post", true)
	RegisterHookChain(RG_CBasePlayer_TakeDamage, "@Player_TakeDamage_Post", true)
}

public plugin_init() {
	register_clcmd("say hud", "@hud")
}

@hud(player) {
	client_print(0, print_chat, "HUD")
	new hudParams[HudParams]
	hudParams = hud_params(
		.color1 = "cyan",
		.color2 = "red",
		.effect = 2,
		.fadeInTime = 0.01,
		.holdTime = 15.0,
		.fadeOutTime = 0.01,
		.fxTime = 0.1
	)
	/*new range[] = { 0x0370, 0x03FF };
	//new range[] = { 0x2600, 0x26FF };
	new maxChars = range[1] - range[0];

	new text[512]
	for (new i = 0; i < maxChars + 1; ++i) {
		add(text, charsmax(text), fmt("%c", range[0] + i))
	}*/
	send_hud(
		player,
		DAMAGER_CHANNEL,
		hudParams,
		fmt("Text: [ᐄᐂ⇗⬈⬀🡽🢅🡥🡭🡵🡽🢅߽]")
	)
	client_print(0, print_chat, "HUD END")
}

@Player_Spawn_Post(player) {
	if (!is_user_alive(player)) {
		return
	}
	for (new target = 1; target <= MaxClients; ++target) {
		TotalDamage[target][player] = 0.0
		TotalHits[target][player] = 0
	}
}

// Can attack another player (attacker, player)

@Player_TakeDamage_Post(player, inflictor, attacker, Float:damage, damageType) {
	if (player == attacker || !is_user_connected(attacker)) {
		return
	}
	if (!rg_is_player_can_takedamage(player, attacker)) {
		return
	}
	TotalHits[attacker][player]++
	new Float:health = get_entvar(player, var_health)
	new Float:realDamage
	if (is_user_alive(player)) {
		realDamage = damage
	} else {
		realDamage = damage + health
	}
	new healthInt = max(0, floatround(health, floatround_floor))
	//client_print(0, print_chat, "HP: %.2f DMG: %.2f to %.2f", health, damage, realDamage)
	//if (damageInt < 1.0) {
	//	return
	//}
	TotalDamage[attacker][player] += realDamage
	new Float:time = get_gametime()
	if (LastDamageTime[attacker] == time) {
		VictimsCount[attacker]++
		VictimsDamage[attacker] += damage
	} else {
		VictimsCount[attacker] = 1
		VictimsDamage[attacker] = damage
	}
	LastDamageTime[attacker] = time
	new victimsDamageInt = floatround(VictimsDamage[attacker], floatround_floor)
	new hudParams[HudParams]
	hudParams = hud_params(
		.color1 = getHudColorByHitBoxGroup(get_member(player, m_LastHitGroup)),
		.color2 = getHudColorByHitBoxGroup(get_member(player, m_LastHitGroup)),
		.effect = 2,
		.fadeInTime = 0.01,
		.holdTime = 1.5,
		.fadeOutTime = 0.01,
		.fxTime = 0.1
	)
	if (VictimsCount[attacker] <= 1) {
		showOnceDamage(attacker, hudParams, realDamage, TotalDamage[attacker][player], player, healthInt)
	} else {
		showMultiDamage(attacker, hudParams, victimsDamageInt, VictimsCount[attacker])
	}
	for (new target = 1; target <= MaxClients; ++target) {
		if (target == attacker) {
			continue
		}
		if (!is_user_connected(target)) {
			continue
		}
		if (get_entvar(target, var_iuser1) != OBS_IN_EYE) {
			continue
		}
		if (get_member(target, m_hObserverTarget) != attacker) {
			continue
		}
		if (VictimsCount[attacker] <= 1) {
			showOnceDamage(target, hudParams, realDamage, TotalDamage[attacker][player], player, healthInt)
		} else {
			showMultiDamage(target, hudParams, victimsDamageInt, VictimsCount[attacker])
		}
	}
	/*if (!is_user_alive(player)) {
		new activeItem = get_member(attacker, m_pActiveItem)
		if (is_nullent(activeItem)) {
			return
		}
		// check null, add grenade
		// multi kills
		new Weapon:weapon = get_entvar(activeItem, var_impulse)
		new intTotalDamage = floatround(TotalDamage[attacker][player], floatround_floor)
		new formatKill[128]
		formatex(
			formatKill, charsmax(formatKill),
			"+1 фраг за раунд^n^nВы убили %s с %L^nУрон: %d (%d попаданий)",
			name, LANG_PLAYER, get_weapon_var_string(weapon, "name"),
			intTotalDamage, TotalHits[attacker][player]
		)
		set_hudmessage(0, 200, 200, 0.6, 0.6, 2, 0.1, 5.0, 0.01, 0.01)
		ShowSyncHudMsg(attacker, g_hudSync_KillInfo, formatKill)
	}*/
}

showOnceDamage(player, hudParams[HudParams], Float:damage, Float:totalDamage, victim, health) {
	new damageInt = floatround(damage, floatround_floor)
	new totalDamageInt = floatround(totalDamage, floatround_floor)
	new damageText[64]
	if (damage == totalDamage) {
		formatex(damageText, charsmax(damageText), "Σ=%d DMG", damageInt, totalDamageInt)
	} else {
		formatex(damageText, charsmax(damageText), "%d DMG (Σ=%d)", damageInt, totalDamageInt)
	}
	if (health) {
		send_hud(
			player,
			DAMAGER_CHANNEL,
			hudParams,
			"^n^n^nx^n^n%s^n%n: %d HP",
			damageText,
			victim, health
		)
	} else {
		send_hud(
			player,
			DAMAGER_CHANNEL,
			hudParams,
			"^n^n^nx^n^n%s^nYou killed %n",
			damageText,
			victim
		)
	}
}

showMultiDamage(player, hudParams[HudParams], victimsDamage, victimsCount) {
	send_hud(
		player,
		DAMAGER_CHANNEL,
		hudParams,
		"^n^n^n\     /^n^n/    \^n^n%d DMG^n%d victims",
		victimsDamage,
		victimsCount
	)
}

	// need invert colors effects

		// change hp to sprite
		// change colors

		// change colors by 100 200 300 dmg

		// ammo pcaks delta [%d/%d]
getHudColorByHitBoxGroup(HitBoxGroup:hitBoxGroup) {
	new color[32]
	switch (hitBoxGroup) {
		case HITGROUP_GENERIC: {
			color = "green"
		}
		case HITGROUP_HEAD: {
			color = "red"
		}
		case HITGROUP_CHEST, HITGROUP_STOMACH: {
			color = "yellow"
		}
		case HITGROUP_LEFTARM,
			HITGROUP_RIGHTARM,
			HITGROUP_LEFTLEG,
			HITGROUP_RIGHTLEG: {
				color = "green"
		}
	}
	return color
}
