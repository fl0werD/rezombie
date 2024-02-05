#include <amxmodx>
#include <rezombie>

new Class:SurvivorClass
new Class:ZombieClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Survivor", "1.0.0", "fl0wer")

	SurvivorClass = checkClassExists("survivor")
	ZombieClass = checkClassExists("zombie")
	
	new Mode:mode = create_mode("survivor", "@launch_survivor", true)
	set_mode_var(mode, "name", "MODE_SURVIVOR_NAME")
	set_mode_var(mode, "description", "MODE_SURVIVOR_DESC")
	set_mode_var(mode, "notice_message", "MODE_SURVIVOR_NOTICE")
	set_mode_var(mode, "color", "blue")

	Fog = create_fog("blue", 15)

	add_translate("modes/survivor")
}

@launch_survivor(target) {
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
	change_player_class(target, SurvivorClass)
	set_game_var("override_default_class", TEAM_HUMAN, SurvivorClass)
	set_override_env_var("fog", Fog)
	return target
}
