#include <amxmodx>
#include <rezombie>

new Subclass:ZombieSwarm

public plugin_precache() {
	register_plugin("Zombie: Swarm", "1.0.0", "fl0wer")
	new Class:class = checkClassExists("zombie")
	add_translate("subclasses/zombie_swarm")

	new Subclass:subclass = ZombieSwarm = create_subclass("zombie_swarm", class)
	set_subclass_var(subclass, "name", "RZ_SUBZOMBIE_SWARM_NAME")
	//rz_subclass_set(subclass, RZ_SUBCLASS_DESC, "RZ_SUBZOMBIE_SWARM_DESC")

	new Props:props = get_subclass_var(subclass, "props")
	set_props_var(props, "health", 700)
	set_props_var(props, "speed", 260)
	set_props_var(props, "gravity", 0.9)
}

@change_class_pre(player, Class:class, attacker) {
	if (attacker && get_player_var(attacker, "subclass") == ZombieSwarm) {
		return RZ_SUPERCEDE
	}
	return RZ_CONTINUE
}
