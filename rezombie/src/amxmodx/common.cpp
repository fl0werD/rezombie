#include "rezombie/amxmodx/common.h"
#include "rezombie/player/players.h"
#include <messages/user_message.h>
#include <amxx/api.h>

namespace rz {
    using namespace amx;
    using namespace amxx;
    using namespace player;
    using namespace message;

    auto AMX_NATIVE_CALL rz_get_players_count(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_players_count_type,
        };

        const auto countType = static_cast<PlayersCountType>(params[arg_players_count_type]);
        return getPlayersCount(Team::Unassigned, countType);
    }

    auto AMX_NATIVE_CALL rz_get_team_players_count(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_team,
            arg_players_count_type,
        };

        const auto team = static_cast<Team>(params[arg_team]);
        const auto countType = static_cast<PlayersCountType>(params[arg_players_count_type]);
        return getPlayersCount(team, countType);
    }

    auto AMX_NATIVE_CALL rz_send_hud(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_hud_params,
            arg_text,
            arg_text_arguments,
        };

        auto length = 0;
        const int playerIndex = params[arg_player];
        const auto value = Address(amx, params[arg_hud_params]);
        // const auto text = GetAmxString(amx, params[arg_text]);
        const auto text = FormatAmxString(amx, params, arg_text, &length);
        const auto& hudParams =
            HudMessageParams()
                .setX(CellToFloat(value[toInt(RzHudParams::X)]))
                .setY(CellToFloat(value[toInt(RzHudParams::Y)]))
                .setEffect(value[toInt(RzHudParams::Effect)])
                .setColor1(
                    {toUChar(value[toInt(RzHudParams::Color1Red)]), toUChar(value[toInt(RzHudParams::Color1Green)]),
                     toUChar(value[toInt(RzHudParams::Color1Blue)]), toUChar(value[toInt(RzHudParams::Color1Alpha)])}
                )
                .setColor2(
                    {toUChar(value[toInt(RzHudParams::Color2Red)]), toUChar(value[toInt(RzHudParams::Color2Green)]),
                     toUChar(value[toInt(RzHudParams::Color2Blue)]), toUChar(value[toInt(RzHudParams::Color2Alpha)])}
                )
                .setFadeInTime(CellToFloat(value[toInt(RzHudParams::FadeInTime)]))
                .setFadeOutTime(CellToFloat(value[toInt(RzHudParams::FadeOutTime)]))
                .setHoldTime(CellToFloat(value[toInt(RzHudParams::HoldTime)]))
                .setFxTime(CellToFloat(value[toInt(RzHudParams::FxTime)]))
                .setChannel(value[toInt(RzHudParams::Channel)]);
        if (playerIndex) {
            auto& player = players[playerIndex];
            netHudMessage(player, hudParams, text);
        } else {
            netHudMessage(TO_ALL, hudParams, text);
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_send_dhud(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_dhud_params,
            arg_text,
            arg_text_arguments,
        };

        auto length = 0;
        const int playerIndex = params[arg_player];
        const auto value = Address(amx, params[arg_dhud_params]);
        // const auto text = GetAmxString(amx, params[arg_text]);
        const auto text = FormatAmxString(amx, params, arg_text, &length);
        const auto& dHudParams = DHudMessageParams()
            .setX(CellToFloat(value[toInt(RzDHudParams::X)]))
            .setY(CellToFloat(value[toInt(RzDHudParams::Y)]))
            .setEffect(value[toInt(RzDHudParams::Effect)])
            .setColor(
                {
                    toUChar(value[toInt(RzDHudParams::ColorRed)]),
                    toUChar(value[toInt(RzDHudParams::ColorGreen)]),
                    toUChar(value[toInt(RzDHudParams::ColorBlue)])
                }
            )
            .setFadeInTime(CellToFloat(value[toInt(RzDHudParams::FadeInTime)]))
            .setFadeOutTime(CellToFloat(value[toInt(RzDHudParams::FadeOutTime)]))
            .setHoldTime(CellToFloat(value[toInt(RzDHudParams::HoldTime)]))
            .setFxTime(CellToFloat(value[toInt(RzDHudParams::FxTime)]));
        if (playerIndex) {
            auto& player = players[playerIndex];
            netDHudMessage(player, dHudParams, text);
        } else {
            netDHudMessage(TO_ALL, dHudParams, text);
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_send_weapon_anim(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_anim_number,
            arg_body,
        };

        const int playerIndex = params[arg_player];
        const int animNumber = params[arg_anim_number];
        const int body = params[arg_body];
        auto& player = players[playerIndex];
        player.SendWeaponAnim(animNumber, body);
        return true;
    }

    auto AmxxCommon::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_get_players_count",      rz_get_players_count},
            {"rz_get_team_players_count", rz_get_team_players_count},
            {"rz_send_hud",               rz_send_hud},
            {"rz_send_dhud",              rz_send_dhud},
            {"rz_send_weapon_anim",       rz_send_weapon_anim},

            {nullptr,                     nullptr},
        };
        AddNatives(natives);
    }
}
