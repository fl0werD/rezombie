#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:NemesisClass
new Fog:Fog
new NightVision:HumanNightVision

public plugin_precache() {
	register_plugin("Mode: Nemesis", "1.0.0", "fl0wer")

	NemesisClass = checkClassExists("nemesis")
	
	new Mode:mode = create_mode("nemesis", "@launch_nemesis", true)
	set_mode_var(mode, "name", "MODE_NEMESIS_NAME")
	set_mode_var(mode, "description", "MODE_NEMESIS_DESC")
	set_mode_var(mode, "notice_message", "MODE_NEMESIS_NOTICE")
	set_mode_var(mode, "color", "red")
	set_mode_var(mode, "respawn", Respawn_ToZombiesTeam)

	Fog = create_fog("red", 15)
	HumanNightVision = create_night_vision("red", create_fog("darkred", 30))
	
	add_translate("modes/nemesis")
}

@launch_nemesis(target) {
	if (!target) {
		new alivesNum
		new Array:alivePlayers = get_alive_players(alivesNum)
		target = ArrayGetCell(alivePlayers, random(alivesNum))
	}
	change_player_class(target, NemesisClass)
	set_game_var("override_default_class", TEAM_ZOMBIE, NemesisClass)
	set_override_env_var("fog", Fog)
	change_player_night_vision(1, HumanNightVision)
	return target
}
