#include <amxmodx>
#include <reapi>
#include <rezombie>

new Mode:InfectionMode
new Class:ZombieClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Infection", "1.0.0", "fl0wer")
	
	ZombieClass = checkClassExists("zombie")
	
	new Mode:mode = InfectionMode = create_mode("multi", "@launch_infection")
	set_mode_var(mode, "name", "MODE_INFECTION_NAME")
	set_mode_var(mode, "description", "MODE_INFECTION_DESC")
	set_mode_var(mode, "notice_message", "MODE_INFECTION_NOTICE")
	set_mode_var(mode, "color", "orange")
	set_mode_var(mode, "respawn", Respawn_ToZombiesTeam)
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_NOTICE, "RZ_GAMEMODE_NOTICE_MULTI")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_HUD_COLOR,  { 200, 50, 0 })
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_DEATHMATCH, RZ_GM_DEATHMATCH_ONLY_TR)
	
	Fog = create_fog("orange", 15)

	add_translate("modes/infection")
}

public plugin_init() {
	RegisterHookChain(RG_CBasePlayer_TakeDamage, "@Player_TakeDamage_Pre", false)
}

@launch_infection() {
	new alivesNum
	new Array:alivePlayers = get_alive_players(alivesNum)
	new maxZombies = 1
	if (alivesNum > 30) {
		maxZombies = 4
	} else if (alivesNum > 20) {
		maxZombies = 3
	} else if (alivesNum > 10) {
		maxZombies = 2
	}
	maxZombies = min(maxZombies, alivesNum)
	for (new zombieCount = 0; zombieCount < maxZombies; ++zombieCount) {
		new index = random(ArraySize(alivePlayers))
		new player = ArrayGetCell(alivePlayers, index)
		change_player_class(player, ZombieClass)
		ArrayDeleteItem(alivePlayers, index)
	}
	set_override_env_var("fog", Fog)
}

@Player_TakeDamage_Pre(player, inflictor, attacker, Float:damage, damageType) {
	if (get_game_var("mode") != InfectionMode) {
		return
	}
	if (!is_valid_attacker(attacker, player)) {
		return
	}
	if (get_player_var(attacker, "class") != ZombieClass) {
		return
	}
	new Weapon:weapon = get_entvar(inflictor, var_impulse)
	if (!is_weapon(weapon)) {
		return
	}
	if (get_weapon_var(weapon, "type") != weapon_type_melee) {
		return
	}
	new Float:armor = get_entvar(player, var_armorvalue)
	if (armor > 0.0) {
		armor = floatmax(armor - damage, 0.0)
		set_entvar(player, var_armorvalue, armor)
		SetHookChainArg(4, ATYPE_FLOAT, 0.0)
		// already played?
		//rh_emit_sound2(player, 0, CHAN_BODY, ARMOR_HIT_SOUND)
	}
	if (armor > 0.0 || (get_member(player, m_iKevlar) == ARMOR_VESTHELM && get_member(player, m_LastHitGroup) == HITGROUP_HEAD)) {
		return
	}
	if (change_player_class(player, ZombieClass, attacker) > RZ_CONTINUE) {
		return
	}
	SetHookChainArg(4, ATYPE_FLOAT, 0.0)
}
