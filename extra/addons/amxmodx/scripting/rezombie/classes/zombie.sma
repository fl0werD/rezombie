#include <amxmodx>
#include <reapi>
#include <rezombie>

new const INFECTION_ICON[] = "teammate"

new Class:ZombieClass
new Melee:ZombieMelee
new NightVision:ZombieNightVision

public plugin_precache() {
	register_plugin("Class: Zombie", "1.0.0", "fl0wer")

	new Class:class = ZombieClass = create_class("zombie", TEAM_ZOMBIE)
	set_class_var(class, "name", "RZ_ZOMBIE")
	set_class_var(class, "forward_give_default_items", "@zombieGiveDefaultItems")

	new Props:props = get_class_var(class, "props")
	set_props_var(props, "health", 500)
	set_props_var(props, "gravity", 0.8)
	set_props_var(props, "weapons_interaction", false)

	new ModelsPack:models = get_class_var(class, "models")
	models_pack_add_model(models, create_model("models/player/leet/leet.mdl"))

	ZombieMelee = get_class_var(class, "melee")
	ZombieNightVision = create_night_vision("red", create_fog("darkred", 30))
	
	add_translate("classes/zombie")
}

@zombieGiveDefaultItems(player) {
	change_player_night_vision(player, ZombieNightVision)
}

@change_class_post(player, Class:class, attacker) {
	if (class != ZombieClass) {
		return
	}
	if (attacker) {
		send_death_msg(TO_ALL, attacker, player, INFECTION_ICON)
		send_score_attrib(TO_ALL, player)
	} else {
		send_death_msg(TO_ALL, attacker, player, INFECTION_ICON)
		send_score_attrib(TO_ALL, player)
	}
	infectionEffects(player)
}

infectionEffects(player) {
	message_begin(MSG_ONE, get_user_msgid("ScreenShake"), _, player)
	write_short((1<<12) * 4)
	write_short((1<<12) * 2)
	write_short((1<<12) * 10)
	message_end()

	new Float:origin[3]
	get_entvar(player, var_origin, origin)
	
	message_begin_f(MSG_PVS, SVC_TEMPENTITY, origin)
	TE_Implosion(origin, 128, 20, 3)
	
	message_begin_f(MSG_PVS, SVC_TEMPENTITY, origin)
	TE_ParticleBurst(origin, 50, 70, 3)
	
	new cvar_infect_sparkle_color[3] = { 0, 150, 0 }

	message_begin_f(MSG_PVS, SVC_TEMPENTITY, origin)
	TE_DLight(origin, 20, cvar_infect_sparkle_color, 2, 0)
}

stock TE_Implosion(Float:position[3], radius, count, life)
{
	write_byte(TE_IMPLOSION);
	write_coord_f(position[0]); // position.x
	write_coord_f(position[1]); // position.y
	write_coord_f(position[2]); // position.z
	write_byte(radius); // radius
	write_byte(count); // count
	write_byte(life); // life in 0.1's
	message_end();
}

stock TE_ParticleBurst(Float:origin[3], radius, particleColor, duration)
{
	write_byte(TE_PARTICLEBURST);
	write_coord_f(origin[0]); // origin
	write_coord_f(origin[1]);
	write_coord_f(origin[2]);
	write_short(radius); // radius
	write_byte(particleColor); // particle color
	write_byte(duration); // duration * 10 (will be randomized a bit)
	message_end();
}

stock TE_DLight(Float:position[3], radius, color[3], life, decayRate)
{
	write_byte(TE_DLIGHT);
	write_coord_f(position[0]); // position.x
	write_coord_f(position[1]); // position.y
	write_coord_f(position[2]); // position.z
	write_byte(radius); // radius in 10's
	write_byte(color[0]); // red
	write_byte(color[1]); // green
	write_byte(color[2]); // blue
	write_byte(life); // life in 10's
	write_byte(decayRate); // decay rate in 10's
	message_end();
}
