#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:ZombieClass
new Fog:Fog

public plugin_precache() {
	register_plugin("Mode: Swarm", "1.0.0", "fl0wer")

	ZombieClass = checkClassExists("zombie")

	new Mode:mode = create_mode("swarm", "@launch_swarm")
	set_mode_var(mode, "name", "MODE_SWARM_NAME")
	set_mode_var(mode, "description", "MODE_SWARM_DESC")
	set_mode_var(mode, "notice_message", "MODE_SWARM_NOTICE")
	set_mode_var(mode, "color", "green")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_NOTICE, "RZ_GAMEMODE_NOTICE_SWARM")
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_HUD_COLOR, { 20, 255, 20 })
	//rz_gamemode_set(gameMode, RZ_GAMEMODE_CHANCE, 20)

	Fog = create_fog("green", 15)
	
	add_translate("modes/swarm")
}

@launch_swarm() {
	new alivesNum
	new Array:alivePlayers = get_alive_players(alivesNum)
	new maxZombies = alivesNum / 2
	new index
	new player
	for (new i = 0; i < maxZombies; ++i) {
		index = random(ArraySize(alivePlayers))
		player = ArrayGetCell(alivePlayers, index)
		change_player_class(player, ZombieClass)
		ArrayDeleteItem(alivePlayers, index)
	}
	set_override_env_var("fog", Fog)
}
