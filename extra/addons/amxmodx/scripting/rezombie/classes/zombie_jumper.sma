#include <amxmodx>
#include <rezombie>

public plugin_precache() {
	register_plugin("Zombie: Jumper", "1.0.0", "fl0wer")
	new Class:class = checkClassExists("zombie")
	add_translate("subclasses/zombie_jumper")

	new Subclass:subclass = create_subclass("zombie_jumper", class)
	set_subclass_var(subclass, "name", "RZ_SUBZOMBIE_JUMPER_NAME")
	//rz_subclass_set(subclass, RZ_SUBCLASS_DESC, "RZ_SUBZOMBIE_JUMPER_DESC")

	new Props:props = get_subclass_var(subclass, "props")
	set_props_var(props, "health", 1000)
	set_props_var(props, "speed", 250)
	set_props_var(props, "gravity", 0.7)
}
