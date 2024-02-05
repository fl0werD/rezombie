#include <amxmodx>
#include <hamsandwich>
#include <fakemeta>
#include <reapi>
#include <rezombie>

new Array:WorldWeapons
new Float:NextPickupHudTime[MAX_PLAYERS + 1]

public plugin_init() {
	register_plugin("Weapons Pickup", "1.0.0", "fl0wer")
	WorldWeapons = ArrayCreate(1)
	/*RegisterHookChain(RG_CBasePlayer_PreThink, "@Player_PreThink_Post", true)
	RegisterHookChain(RG_CBasePlayer_DropPlayerItem, "@Player_DropPlayerItem_Post", true)
	RegisterHam(Ham_Touch, WEAPON_PLACEHOLDER, "@Weapon_Touch_Pre", false)
	RegisterHam(Ham_StartSneaking, WEAPON_PLACEHOLDER, "@Weapon_OnCreate_Post", true)
	RegisterHam(Ham_StopSneaking, WEAPON_PLACEHOLDER, "@Weapon_OnDestroy_Post", true)*/
}

@Player_PreThink_Post(player) {
	static Float:time
	if (!is_user_alive(player)) {
		return
	}
	if (!(get_entvar(player, var_button) & IN_USE) && get_entvar(player, var_oldbuttons) & IN_USE) {
		pickupWeapon(player)
		return
	}
	time = get_gametime()
	if (NextPickupHudTime[player] > time) {
		return
	}
	NextPickupHudTime[player] = time + 0.2
	showPickupHud(player)
}

@Player_DropPlayerItem_Post(player, itemName[]) {
	new item = GetHookChainReturn(ATYPE_INTEGER)
	if (is_nullent(item)) {
		return
	}
	new Float:velocity[3]
	new Float:viewAngle[3]
	new Float:viewForward[3]
	get_entvar(player, var_v_angle, viewAngle)
	angle_vector(viewAngle, ANGLEVECTOR_FORWARD, viewForward)
	for (new i = 0; i < sizeof(velocity); i++) {
		velocity[i] = viewForward[i] * 300.0
	}
	velocity[2] += 100.0
	set_entvar(item, var_velocity, velocity)
	set_entvar(item, var_avelocity, Float:{ 0.0, -128.0, 0.0 })
	//ArrayPushCell(WorldWeapons, item)
	new Weapon:impulse = get_entvar(item, var_impulse)
	// set trans
	client_print(player, print_center, "Вы выбросили %l", get_weapon_var_string(impulse, "name"))
	set_entvar(item, var_fuser1, get_gametime() + 1.5)
	//set_hudmessage(255, 255, 255, -1.0, 0.55, 0, 6.0, 2.0, 0.01, 0.01)
	//ShowSyncHudMsg(player, g_hudSync_Pickup, "Вы выбросили %s", WEAPON_NAMES[get_member(item, m_iId)])
}

@Weapon_Touch_Pre(weapon, other) {
	if (!(get_entvar(weapon, var_flags) & FL_ONGROUND)) {
		return HAM_IGNORED
	}
	if (!is_user_alive(other)) {
		return HAM_IGNORED
	}
	if (get_entvar(weapon, var_owner) != other) {
		return HAM_IGNORED
	}
	if (Float:get_entvar(weapon, var_fuser1) < get_gametime()) {
		return HAM_IGNORED
	}
	return HAM_SUPERCEDE
}

@Weapon_OnCreate_Post(weapon) {
	ArrayPushCell(WorldWeapons, weapon)
}

@Weapon_OnDestroy_Post(weapon) {
	new id = ArrayFindValue(WorldWeapons, weapon)
	if (id == -1) {
		return
	}
	ArrayDeleteItem(WorldWeapons, id)
}

showPickupHud(player) {
	static item
	static itemSlot
	static Weapon:weapon
	static playerItem
	item = findWorldWeaponByView(player)
	if (item == -1) {
		return
	}
	weapon = get_entvar(item, var_impulse)
	itemSlot = rg_get_iteminfo(item, ItemInfo_iSlot)
	static hudParams[HudParams]
	hudParams = hud_params(-1.0, 0.55, "white", "white", .holdTime = 0.21)
	if (itemSlot != 0 && itemSlot != 1) {
		send_hud(player, NOTICE_CHANNEL, hudParams, "%l", get_weapon_var_string(weapon, "name"))
		return
	}
	playerItem = get_member(player, m_rgpPlayerItems, itemSlot)
	if (is_nullent(playerItem) || get_entvar(playerItem, var_impulse) == weapon) {
		send_hud(player, NOTICE_CHANNEL, hudParams, "%l", get_weapon_var_string(weapon, "name"))
		return
	}
	send_hud(player, NOTICE_CHANNEL, hudParams, "[E] сменить на %l", get_weapon_var_string(weapon, "name"))
}

pickupWeapon(player) {
	static item
	static itemSlot
	item = findWorldWeaponByView(player)
	if (item == -1) {
		return
	}
	itemSlot = rg_get_iteminfo(item, ItemInfo_iSlot)
	if (itemSlot != 0 && itemSlot != 1) {
		return
	}
	rg_drop_items_by_slot(player, any:itemSlot)
	set_entvar(item, var_flags, get_entvar(item, var_flags) | FL_ONGROUND)
	set_entvar(item, var_fuser1, 0.0)
	ExecuteHamB(Ham_Touch, item, player)
}

findWorldWeaponByView(player) {
	static i
	static item
	static weaponsNum
	static Float:src[3]
	static Float:end[3]
	static Float:viewAngle[3]
	static Float:punchAngle[3]
	static Float:viewForward[3]
	ExecuteHamB(Ham_Player_GetGunPosition, player, src)
	get_entvar(player, var_v_angle, viewAngle)
	get_entvar(player, var_punchangle, punchAngle)
	for (i = 0; i < sizeof(viewAngle); i++) {
		viewAngle[i] += punchAngle[i]
	}
	angle_vector(viewAngle, ANGLEVECTOR_FORWARD, viewForward)
	for (i = 0; i < sizeof(end); i++) {
		end[i] = src[i] + viewForward[i] * 128.0
	}
	weaponsNum = ArraySize(WorldWeapons)
	for (i = 0; i < weaponsNum; ++i) {
		item = ArrayGetCell(WorldWeapons, i)
		if (is_nullent(item)) {
			continue
		}
		engfunc(EngFunc_TraceModel, src, end, HULL_POINT, item, 0)
		if (get_tr2(0, TR_pHit) != item) {
			continue
		}
		return item
	}
	return -1
}
