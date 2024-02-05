#include <amxmodx>
#include <reapi>
#include <rezombie>

new Currency:MoneyCurrency

new Money[MAX_PLAYERS + 1]

public plugin_precache() {
	register_plugin("Currency: Money", "1.0.0", "fl0wer")
	MoneyCurrency = create_currency(
		.currencyName = "money",
		.getFunction = "@getMoney",
		.setFunction = "@setMoney",
		.formatFunction = "@formatMoney"
	)
}

public client_putinserver(player) {
	//set_player_var(player, "currency", MoneyCurrency, 100)
}

@getMoney(player) {
	return Money[player]
}

@setMoney(player, amount, const reason[]) {
	new oldAmount = Money[player]
	Money[player] = amount
	set_member(player, m_iAccount, amount)
	send_money(player, amount)
	if (reason[0]) {
		if (amount >= oldAmount) {
			print_to_chat(
				player, print_team_default,
				"^4+%s^1: %s",
				format_currency(MoneyCurrency, amount - oldAmount), reason
			)
		} else if (amount < oldAmount) {
			print_to_chat(
				player, print_team_red,
				"^3-%s^1: %s",
				format_currency(MoneyCurrency, oldAmount - amount), reason
			)
		}
	}
	return true
}

@formatMoney(amount, bool:isShort, text[], textLength) {
	formatex(text, textLength, "$%d", amount)
}
