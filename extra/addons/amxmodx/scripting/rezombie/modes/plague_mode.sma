#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:ZombieClass
new Class:NemesisClass
new Class:SurvivorClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Plague", "1.0.0", "fl0wer")

	ZombieClass = checkClassExists("zombie")
	NemesisClass = checkClassExists("nemesis")
	SurvivorClass = checkClassExists("survivor")
	
	new Mode:mode = create_mode("plague", "@launch_plague")
	set_mode_var(mode, "name", "MODE_PLAGUE_NAME")
	set_mode_var(mode, "description", "MODE_PLAGUE_DESC")
	set_mode_var(mode, "notice_message", "MODE_PLAGUE_NOTICE")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_NOTICE, "RZ_GAMEMODE_NOTICE_PLAGUE")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_HUD_COLOR, { 0, 50, 200 })
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_CHANCE, 20)
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_MIN_ALIVES, 8)

	Fog = create_fog("aqua", 15)

	add_translate("modes/plague")
}

@launch_plague() {
	new alivesNum
	new Array:alivePlayers = get_alive_players(alivesNum)
	new maxNemeses = 1
	new maxSurvivors = 1
	new maxZombies = floatround((alivesNum - (maxNemeses + maxSurvivors)) * 0.5, floatround_ceil)
	new Float:health
	new index
	new player
	for (new i = 0; i < maxNemeses; ++i) {
		index = random(ArraySize(alivePlayers))
		player = ArrayGetCell(alivePlayers, index)
		change_player_class(player, NemesisClass)
		health = Float:get_entvar(player, var_health) * 0.5
		set_entvar(player, var_health, health)
		set_entvar(player, var_max_health, health)
		ArrayDeleteItem(alivePlayers, index)
	}
	for (new i = 0; i < maxSurvivors; ++i) {
		index = random(ArraySize(alivePlayers))
		player = ArrayGetCell(alivePlayers, index)
		change_player_class(player, SurvivorClass)
		health = Float:get_entvar(player, var_health) * 0.5
		set_entvar(player, var_health, health)
		set_entvar(player, var_max_health, health)
		ArrayDeleteItem(alivePlayers, index)
	}
	for (new i = 0; i < maxZombies; ++i) {
		index = random(ArraySize(alivePlayers))
		player = ArrayGetCell(alivePlayers, index)
		change_player_class(player, ZombieClass)
		ArrayDeleteItem(alivePlayers, index)
	}
	set_override_env_var("fog", Fog)
}
