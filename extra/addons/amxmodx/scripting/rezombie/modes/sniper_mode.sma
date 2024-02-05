#include <amxmodx>
#include <rezombie>

new Class:SniperClass
new Class:ZombieClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Sniper", "1.0.0", "fl0wer")

	SniperClass = checkClassExists("sniper")
	ZombieClass = checkClassExists("zombie")
	
	new Mode:mode = create_mode("sniper", "@launch_sniper", true)
	set_mode_var(mode, "name", "MODE_SNIPER_NAME")
	set_mode_var(mode, "description", "MODE_SNIPER_DESC")
	set_mode_var(mode, "notice_message", "MODE_SNIPER_NOTICE")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_NOTICE, "RZ_GAMEMODE_NOTICE_SNIPER")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_HUD_COLOR, { 0, 250, 250 })
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_CHANCE, 20)

	Fog = create_fog("cyan", 15)
	
	add_translate("modes/sniper")
}

@launch_sniper(target) {
	new alivesNum
	new Array:alivePlayers = get_alive_players(alivesNum)
	if (!target) {
		target = ArrayGetCell(alivePlayers, random(alivesNum))
	}
	new player
	for (new i = 0; i < alivesNum; ++i) {
		player = ArrayGetCell(alivePlayers, i)
		if (player == target) {
			continue
		}
		change_player_class(player, ZombieClass)
	}
	change_player_class(target, SniperClass)
	set_game_var("override_default_class", TEAM_HUMAN, SniperClass)
	set_override_env_var("fog", Fog)
	return target
}
