#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:AssassinClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Assassin", "1.0.0", "fl0wer")

	AssassinClass = checkClassExists("assassin")

	new Mode:mode = create_mode("assassin", "@launch_assassin", true)
	set_mode_var(mode, "name", "MODE_ASSASSIN_NAME")
	set_mode_var(mode, "description", "MODE_ASSASSIN_DESC")
	set_mode_var(mode, "notice_message", "MODE_ASSASSIN_NOTICE")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_NOTICE, "RZ_GAMEMODE_NOTICE_ASSASSIN");
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_HUD_COLOR, { 255, 150, 20 });
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_CHANCE, 20);

	Fog = create_fog("yellow", 15)
	
	add_translate("modes/assassin")
}

@launch_assassin(target) {
	if (!target) {
		new alivesNum
		new Array:alivePlayers = get_alive_players(alivesNum)
		target = ArrayGetCell(alivePlayers, random(alivesNum))
	}
	change_player_class(target, AssassinClass)
	set_game_var("override_default_class", TEAM_ZOMBIE, AssassinClass)
	set_override_env_var("light", "a")
	set_override_env_var("fog", Fog)
	return target
}
