#include <amxmodx>
#include <reapi>
#include <rezombie>

const GIVE_ARMOR = 100
new const ARMOR_PICKUP_SOUND[] = "items/ammopickup2.wav"
/*
native ItemFilter:add_item_filter(Item:item, const filter[], any:value);
native ItemRestrict:add_item_restrict(Item:item, const filter[], any:value);

native Cost:create_cost(const value, const currency[]);
*/

new NightVision:HumanNightVision

public plugin_precache() {
	register_plugin("[RZ] Item: Armor", "1.0.0", "fl0wer")
	new Class:human = checkClassExists("human")
	new Item:item = create_item("human_armor", "@give_armor")
	if (!item) {
		return
	}
	add_translate("items/human_armor")
	set_item_var(item, "name", "RZ_ITEM_ARMOR")
	set_item_var(item, "price", create_price(find_currency("money"), 10))
	/*rz_add_item_filter(item, "minimum_players", 4)
	rz_add_item_filter(item, "rounds_delay", 2)
	rz_add_item_filter(item, "map_limit", 1)
	rz_add_item_filter(item, "@filter")
	rz_add_item_filter(item, item_filter_rounds_delay, 2)*/
	//rz_item_command_add(item, "say /armor")
	precache_sound(ARMOR_PICKUP_SOUND)

	new Item:item2 = create_item("human_armor2", "@give_armor")
	if (!item2) {
		return
	}
	add_translate("items/human_armor")
	set_item_var(item2, "name", "RZ_ITEM_ARMOR")
	set_item_var(item2, "price", create_price(find_currency("ammo_packs"), 7))
	/*add_item_filter(item2, "class", human)
	add_item_restrict(item2, "cost", create_cost(7, "ammo_packs"))
	add_item_restrict(item2, "map", 1)*/

	new Item:item3 = create_item("human_armor3", "@give_armor")
	if (!item3) {
		return
	}
	add_translate("items/human_armor")
	set_item_var(item3, "name", "RZ_ITEM_ARMOR")

	new Item:item4 = create_item("human_armor4", "@give_night_vision")
	if (!item4) {
		return
	}
	add_translate("items/human_armor")
	set_item_var(item4, "name", "ITEM_NIGHT_VISION")
	HumanNightVision = create_night_vision("blue")
}

@give_armor(player) {
	rg_set_user_armor(player, max(rg_get_user_armor(player), GIVE_ARMOR), ARMOR_VESTHELM)
	rh_emit_sound2(player, 0, CHAN_ITEM, ARMOR_PICKUP_SOUND, VOL_NORM, ATTN_NORM)
}

@give_night_vision(player) {
	change_player_night_vision(player, HumanNightVision)
}
