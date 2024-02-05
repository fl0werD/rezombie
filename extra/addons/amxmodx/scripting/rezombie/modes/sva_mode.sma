#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:SniperClass
new Class:AssassinClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Snipers vs. Assassins", "1.0.0", "fl0wer")

	SniperClass = checkClassExists("sniper")
	AssassinClass = checkClassExists("assassin")
	
	new Mode:mode = create_mode("sva", "@launch_sva")
	set_mode_var(mode, "name", "MODE_SVA_NAME")
	set_mode_var(mode, "description", "MODE_SVA_DESC")
	set_mode_var(mode, "notice_message", "MODE_SVA_NOTICE")
	set_mode_var(mode, "respawn", Respawn_ToZombiesTeam)
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_NOTICE, "RZ_GAMEMODE_NOTICE_AVS");
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_HUD_COLOR, { 200, 150, 20 });
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_CHANCE, 20);
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_MIN_ALIVES, 8);

	Fog = create_fog("white", 15)
	
	add_translate("modes/snipers_vs_assassins")
}

@launch_sva() {
	new alivesNum
	new Array:alivePlayers = get_alive_players(alivesNum)
	new maxAssassins = floatround(alivesNum * 0.5, floatround_ceil)
	new Float:health
	new index
	new player
	for (new i = 0; i < maxAssassins; ++i) {
		index = random(ArraySize(alivePlayers))
		player = ArrayGetCell(alivePlayers, index)
		change_player_class(player, AssassinClass)
		health = Float:get_entvar(player, var_health) * 0.5
		set_entvar(player, var_health, health)
		set_entvar(player, var_max_health, health)
		ArrayDeleteItem(alivePlayers, index)
	}
	alivesNum = ArraySize(alivePlayers)
	for (new i = 0; i < alivesNum; ++i) {
		player = ArrayGetCell(alivePlayers, i)
		change_player_class(player, SniperClass)
		health = Float:get_entvar(player, var_health) * 1.5
		set_entvar(player, var_health, health)
		set_entvar(player, var_max_health, health)
	}
	set_game_var("override_default_class", TEAM_HUMAN, SniperClass)
	set_game_var("override_default_class", TEAM_ZOMBIE, AssassinClass)
	set_override_env_var("fog", Fog)
}
