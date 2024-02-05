#include <amxmodx>
#include <rezombie>

new Currency:AmmoPacksCurrency

new AmmoPacks[MAX_PLAYERS + 1]

new ChangingHudParams[HudParams]

new const AMMO_PACKS_PLURALS[Plurals][MAX_PLURAL_LENGTH] = { "AMMO_PACKS_ONE", "AMMO_PACKS_FEW", "AMMO_PACKS_MANY" }

public plugin_precache() {
	register_plugin("Currency: Ammo Packs", "1.0.0", "fl0wer")
	AmmoPacksCurrency = create_currency(
		.currencyName = "ammo_packs",
		.getFunction = "@getAmmoPacks",
		.setFunction = "@setAmmoPacks",
		.formatFunction = "@formatAmmoPacks"
	)
	ChangingHudParams = hud_params(
		.x = 0.6,
		.y = 0.6,
		.color1 = "red",
		.color2 = "red",
		.effect = 2,
		.fadeInTime = 0.01,
		.holdTime = 3.0,
		.fadeOutTime = 0.01,
		.fxTime = 0.1
	)
	add_translate("currency/ammo_packs")
}

public client_putinserver(player) {
	//set_player_var(player, "currency", AmmoPacksCurrency, 25)
}

@getAmmoPacks(player) {
	return AmmoPacks[player]
}

@setAmmoPacks(player, amount, const reason[]) {
	new oldAmount = AmmoPacks[player]
	AmmoPacks[player] = amount
	new different
	new diffText[MAX_CURRENCY_FORMAT_LENGTH]
	if (amount > oldAmount) {
		different = amount - oldAmount
		copy(ChangingHudParams[hud_color1], charsmax(ChangingHudParams[hud_color1]), "green")
	} else if (amount < oldAmount) {
		different = oldAmount - amount
		copy(ChangingHudParams[hud_color1], charsmax(ChangingHudParams[hud_color1]), "red")
	}
	diffText = format_currency(AmmoPacksCurrency, different, reason[0] ? true : false)
	if (reason[0]) {
		if (amount > oldAmount) {
			send_hud(player, NOTICE_CHANNEL, ChangingHudParams, "+%s (%s)", diffText, reason)
		} else if (amount < oldAmount) {
			send_hud(player, NOTICE_CHANNEL, ChangingHudParams, "-%s (%s)", diffText, reason)
		}
	} else {
		if (amount > oldAmount) {
			send_hud(player, NOTICE_CHANNEL, ChangingHudParams, "+%s", diffText)
		} else if (amount < oldAmount) {
			send_hud(player, NOTICE_CHANNEL, ChangingHudParams, "-%s", diffText)
		}
	}
	return true
}

@formatAmmoPacks(amount, bool:isShort, text[], textLength) {
	if (isShort) {
		formatex(text, textLength, "%d %l", amount, "AMMO_PACKS_SHORT")
	} else {
		formatex(text, textLength, "%d %l", amount, quantity(amount, AMMO_PACKS_PLURALS))
	}
}
