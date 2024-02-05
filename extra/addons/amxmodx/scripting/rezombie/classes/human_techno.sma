#include <amxmodx>
#include <rezombie>

public plugin_precache() {
	register_plugin("Human: Techno Dancer", "1.0.0", "fl0wer")
	new Class:class = checkClassExists("human")
	add_translate("subclasses/human_techno_dancer")
	
	new Subclass:subclass = create_subclass("human_techno_dancer", class)
	set_subclass_var(subclass, "name", "RZ_SUBHUMAN_TECHNO_NAME")
	//rz_subclass_set(subclass, RZ_SUBCLASS_DESC, "RZ_SUBHUMAN_TECHNO_DESC")
	set_subclass_var(subclass, "model", create_model("rezombie/players/human/phoenix/1.mdl"))

	new Props:props = get_subclass_var(subclass, "props")
	set_props_var(props, "health", 200)
}
