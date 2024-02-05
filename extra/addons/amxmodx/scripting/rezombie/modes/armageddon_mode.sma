#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:SurvivorClass
new Class:SniperClass
new Class:NemesisClass
new Class:AssassinClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Armageddon", "1.0.0", "fl0wer")

	SurvivorClass = checkClassExists("survivor")
	SniperClass = checkClassExists("sniper")
	NemesisClass = checkClassExists("nemesis")
	AssassinClass = checkClassExists("assassin")
	
	new Mode:mode = create_mode("armageddon", "@launch_armageddon")
	set_mode_var(mode, "name", "MODE_ARMAGEDDON_NAME")
	set_mode_var(mode, "description", "MODE_ARMAGEDDON_DESC")
	set_mode_var(mode, "notice_message", "MODE_ARMAGEDDON_NOTICE")
	set_mode_var(mode, "respawn", Respawn_ToHumansTeam)
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_NOTICE, "RZ_GAMEMODE_NOTICE_LNJ")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_HUD_COLOR, { 181, 62, 244 })
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_CHANCE, 20)
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_MIN_ALIVES, 16)
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_DEATHMATCH, RZ_GM_DEATHMATCH_ONLY_CT)

	Fog = create_fog("black", 15)

	add_translate("modes/armageddon")
}

// this is with snipers, assassins?
@launch_armageddon() {
	new alivesNum
	new Array:alivePlayers = get_alive_players(alivesNum)
	new maxNemeses = floatround(alivesNum * 0.5, floatround_ceil)
	new Float:health
	new index
	new player
	for (new i = 0; i < maxNemeses; ++i) {
		index = random(ArraySize(alivePlayers))
		player = ArrayGetCell(alivePlayers, index)
		change_player_class(player, NemesisClass)
		health = Float:get_entvar(player, var_health) * 0.25
		set_entvar(player, var_health, health)
		set_entvar(player, var_max_health, health)
		ArrayDeleteItem(alivePlayers, index)
	}
	alivesNum = ArraySize(alivePlayers)
	for (new i = 0; i < alivesNum; ++i) {
		player = ArrayGetCell(alivePlayers, i)
		change_player_class(player, SurvivorClass)
		health = Float:get_entvar(player, var_health) * 0.25
		set_entvar(player, var_health, health)
		set_entvar(player, var_max_health, health)
	}
	set_game_var("override_default_class", TEAM_ZOMBIE, NemesisClass)
	set_game_var("override_default_class", TEAM_HUMAN, SurvivorClass)
	set_override_env_var("fog", Fog)
}
