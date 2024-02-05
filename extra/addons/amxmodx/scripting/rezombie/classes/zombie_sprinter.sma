#include <amxmodx>
#include <rezombie>

public plugin_precache() {
	register_plugin("Zombie: Sprinter", "1.0.0", "fl0wer")
	new Class:class = checkClassExists("zombie")
	add_translate("subclasses/zombie_sprinter")

	new Subclass:subclass = create_subclass("zombie_sprinter", class)
	set_subclass_var(subclass, "name", "RZ_SUBZOMBIE_SPRINTER_NAME")
	//rz_subclass_set(subclass, RZ_SUBCLASS_DESC, "RZ_SUBZOMBIE_SPRINTER_DESC")

	new Props:props = get_subclass_var(subclass, "props")
	set_props_var(props, "health", 700)
	set_props_var(props, "speed", 260)
	set_props_var(props, "gravity", 0.9)
}
