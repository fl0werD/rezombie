#include <amxmodx>
#include <reapi>
#include <rezombie>

new ModelIndex_ShockWave

public plugin_precache() {
	register_plugin("Jumps", "1.0", "fl0wer")
	ModelIndex_ShockWave = precache_model("sprites/shockwave.spr")
}

public plugin_init() {
	register_clcmd("radio1", "@p")
}

@p(player) {
	@player_jump(player, 0)
}

@player_jump(player, count) {
	if (!count) {
		update_player_info(player)
		return
	}
	new Float:velocity[3]
	get_entvar(player, var_velocity, velocity)
	velocity[2] = 268.3281573
	set_entvar(player, var_velocity, velocity)
	rg_set_animation(player, PLAYER_JUMP)
	update_player_info(player)
	jumpEffects(player)
}

jumpEffects(player) {
	new Float:origin[3]
	new Float:mins[3]
	new Float:axis[3]
	get_entvar(player, var_origin, origin)
	get_entvar(player, var_mins, mins)
	origin[2] += mins[2]
	axis = origin
	axis[2] += 48.0
	message_begin_f(MSG_PVS, SVC_TEMPENTITY, origin)
	TE_BeamDisk(origin, axis, ModelIndex_ShockWave, 0, 0, 4, 255, 0, { 200, 0, 0 }, 200, 0)
}

stock TE_BeamDisk(Float:position[3], Float:axis[3], spriteIndex, startingFrame, frameRate, life, lineWidth, noiseAmplitude, color[3], brightness, scrollSpeed)
{
	write_byte(TE_BEAMDISK);
	write_coord_f(position[0]); // position.x
	write_coord_f(position[1]); // position.y
	write_coord_f(position[2]); // position.z
	write_coord_f(axis[0]); // axis.x
	write_coord_f(axis[1]); // axis.y
	write_coord_f(axis[2]); // axis.z
	write_short(spriteIndex); // sprite index
	write_byte(startingFrame); // starting frame
	write_byte(frameRate); // frame rate in 0.1's
	write_byte(life); // life in 0.1's
	write_byte(lineWidth); // line width in 0.1's
	write_byte(noiseAmplitude); // noise amplitude in 0.01's
	write_byte(color[0]); // red
	write_byte(color[1]); // green
	write_byte(color[2]); // blue
	write_byte(brightness); // brightness
	write_byte(scrollSpeed); // scroll speed in 0.1's
	message_end();
}
