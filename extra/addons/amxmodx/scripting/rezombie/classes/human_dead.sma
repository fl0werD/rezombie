#include <amxmodx>
#include <rezombie>

public plugin_precache() {
	register_plugin("Human: Dead Inside", "1.0.0", "fl0wer")
	new Class:class = checkClassExists("human")
	add_translate("subclasses/human_dead_inside")

	new Subclass:subclass = create_subclass("human_dead_inside", class)
	set_subclass_var(subclass, "name", "RZ_SUBHUMAN_DEAD_NAME")
	//rz_subclass_set(subclass, RZ_SUBCLASS_DESC, "RZ_SUBHUMAN_DEAD_DESC")
	set_subclass_var(subclass, "model", create_model("rezombie/players/human/elite_crew/1.mdl"))

	new Props:props = get_subclass_var(subclass, "props")
	set_props_var(props, "health", 50)
	set_props_var(props, "armor", 50)
}
