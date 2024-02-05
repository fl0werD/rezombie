#include <amxmodx>
#include <hamsandwich>
#include <fakemeta>
#include <reapi>

new ModelIndex_BloodSpray
new ModelIndex_BloodDrop

new Array:BloodDecals

public plugin_precache() {
	register_plugin("[RZ] Player Blood", "1.0", "fl0wer")
	ModelIndex_BloodSpray = precache_model("sprites/bloodspray.spr")
	ModelIndex_BloodDrop = precache_model("sprites/blood.spr")
	BloodDecals = ArrayCreate(1)
	for (new i = 1; i <= 6; ++i) {
		ArrayPushCell(BloodDecals, engfunc(EngFunc_DecalIndex, fmt("{blood%d", i)))
	}
}

public plugin_init() {
	RegisterHookChain(RG_CBasePlayer_TraceAttack, "@Player_TraceAttack_Pre", false)
}

@Player_TraceAttack_Pre(player, attacker, Float:damage, Float:direction[3], trace, bitsDamageType) {
	new bool:shouldBleed = true
	new bool:shouldSpark = false
	if (is_user_connected(attacker)) {
		//if (CSPlayer()->GetProtectionState() == CCSPlayer::ProtectionSt_Active) {
		//	return
		//}
		if (!rg_is_player_can_takedamage(player, attacker)) {
			shouldBleed = false
		}
	}
	if (Float:get_entvar(player, var_takedamage) == DAMAGE_NO) {
		return HC_SUPERCEDE
	}
	new HitBoxGroup:hitBoxGroup = any:get_tr2(trace, TR_iHitgroup)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	set_member(player, m_LastHitGroup, hitBoxGroup)
	switch (hitBoxGroup) {
		case HITGROUP_HEAD: {
			if (get_member(player, m_iKevlar) == ARMOR_VESTHELM) {
				shouldBleed = false
				shouldSpark = true
			}
			damage *= 4.0
			if (shouldBleed) {
				punchAngle[0] = floatmax(damage * -0.5, -12.0)
				punchAngle[2] = floatclamp(damage * random_float(-1.0, 1.0), -9.0, 9.0)
				set_entvar(player, var_punchangle, punchAngle)
			}
		}
		case HITGROUP_CHEST: {
			damage *= 1.0
			if (get_member(player, m_iKevlar) != ARMOR_NONE) {
				shouldBleed = false
			} else if (shouldBleed) {
				punchAngle[0] = floatmax(damage * -0.1, -4.0)
				set_entvar(player, var_punchangle, punchAngle)
			}
		}
		case HITGROUP_STOMACH: {
			damage *= 1.25
			if (get_member(player, m_iKevlar) != ARMOR_NONE) {
				shouldBleed = false
			} else if (shouldBleed) {
				punchAngle[0] = floatmax(damage * -0.1, -4.0)
				set_entvar(player, var_punchangle, punchAngle)
			}
		}
		case HITGROUP_LEFTARM, HITGROUP_RIGHTARM: {
			if (get_member(player, m_iKevlar) != ARMOR_NONE) {
				shouldBleed = false
			}
		}
		case HITGROUP_LEFTLEG, HITGROUP_RIGHTLEG: {
			damage *= 0.75
		}
	}
	new bloodColor = ExecuteHamB(Ham_BloodColor, player)
	new Float:endPosition[3]
	new Float:planeNormal[3]
	get_tr2(trace, TR_vecEndPos, endPosition)
	get_tr2(trace, TR_vecPlaneNormal, planeNormal)
	if (shouldBleed) {
		//BloodSplat(endPosition, direction, hitBoxGroup, damage * 5.0)
		SpawnBlood(endPosition, bloodColor, damage)
		TraceBleed(player, bloodColor, damage, endPosition, direction, bitsDamageType)
	} else if (hitBoxGroup == HITGROUP_HEAD && shouldSpark) {
		message_begin_f(MSG_PVS, SVC_TEMPENTITY, endPosition)
		write_byte(TE_STREAK_SPLASH)
		write_coord_f(endPosition[0])
		write_coord_f(endPosition[1])
		write_coord_f(endPosition[2])
		write_coord_f(planeNormal[0])
		write_coord_f(planeNormal[1])
		write_coord_f(planeNormal[2])
		write_byte(5) // color
		write_short(22) // count
		write_short(25) // base speed
		write_short(65) // random velocity
		message_end()
	}
	rg_multidmg_add(attacker, player, damage, bitsDamageType)
	return HC_SUPERCEDE
}

SpawnBlood(Float:position[3], bloodColor, Float:damage) {
	new amount = floatround(damage, floatround_floor)
	if (bloodColor == DONT_BLEED || !amount) {
		return
	}
	amount *= 2
	if (amount > 255) {
		amount = 255
	}
	message_begin_f(MSG_PVS, SVC_TEMPENTITY, position)
	TE_BloodSprite(position, ModelIndex_BloodSpray, ModelIndex_BloodDrop, bloodColor, clamp(amount / 10, 3, 16))
}

TraceBleed(player, bloodColor, Float:damage, Float:src[3], Float:direction[3], bitsDamageType) {
	if (bloodColor == DONT_BLEED || !damage) {
		return
	}
	if (!(bitsDamageType & (DMG_CRUSH | DMG_BULLET | DMG_SLASH | DMG_BLAST | DMG_CLUB | DMG_MORTAR))) {
		return
	}
	new count
	new Float:noise
	if (damage < 10.0) {
		count = 1
		noise = 0.1
	} else if (damage < 25.0) {
		count = 2
		noise = 0.2
	} else {
		count = 4
		noise = 0.3
	}
	new Float:bloodDirection[3]
	new Float:end[3]
	new Float:fraction
	for (new i = 0; i < count; ++i) {
		for (new j = 0; j < 3; ++j) {
			bloodDirection[j] = direction[j] * -1.0
			bloodDirection[j] += random_float(-noise, noise)
			end[j] = src[j] + bloodDirection[j] * -172.0
		}
		engfunc(EngFunc_TraceLine, src, end, IGNORE_MONSTERS, player, 0)
		get_tr2(0, TR_flFraction, fraction)
		if (fraction >= 1.0) {
			continue
		}
		//if (random_num(0, 2)) {
		//	continue
		//}
		UTIL_DecalTrace(0, ArrayGetCell(BloodDecals, random_num(0, ArraySize(BloodDecals) - 1)))
	}
}

UTIL_DecalTrace(trace, decal) {
	if (decal < 0) {
		return
	}
	new Float:fraction
	get_tr2(trace, TR_flFraction, fraction)
	if (fraction >= 1.0) {
		return
	}
	new entityId
	new entity = get_tr2(trace, TR_pHit)
	if (entity != NULLENT && !ExecuteHamB(Ham_IsBSPModel, entity)) {
		entityId = entity
	}
	new message = TE_DECAL
	if (entityId) {
		if (decal > 255) {
			message = TE_DECALHIGH
			decal -= 256
		}
	} else {
		message = TE_WORLDDECAL
		if (decal > 255) {
			message = TE_WORLDDECALHIGH
			decal -= 256
		}
	}
	new Float:endPosition[3]
	get_tr2(trace, TR_vecEndPos, endPosition)
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(message)
	write_coord_f(endPosition[0])
	write_coord_f(endPosition[1])
	write_coord_f(endPosition[2])
	write_byte(decal)
	if (entityId) {
		write_short(entityId)
	}
	message_end()
}

TE_BloodSprite(Float:position[3], sprite1Index, sprite2Index, color, scale) {
	write_byte(TE_BLOODSPRITE)
	write_coord_f(position[0])
	write_coord_f(position[1])
	write_coord_f(position[2])
	write_short(sprite1Index)
	write_short(sprite2Index)
	write_byte(color)
	write_byte(scale)
	message_end()
}
