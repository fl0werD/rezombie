#include "rezombie/messages/api/user_message.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto send_death_msg(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_receiver,
            arg_attacker,
            arg_victim,
            arg_weapon_name,
            arg_is_headshot,
        };

        // Check arg count
        const auto receiver = params[arg_receiver];
        const auto attacker = params[arg_attacker];
        const auto victim = params[arg_victim];
        const auto weaponName = GetAmxString(amx, params[arg_weapon_name]);
        const auto isHeadshot = params[arg_is_headshot];
        if (receiver) {
            auto& player = Players[receiver];
            sendDeathMsg(player, attacker, victim, weaponName, isHeadshot);
        } else {
            sendDeathMsg(TO_ALL, attacker, victim, weaponName, isHeadshot);
        }
        return true;
    }

    auto send_score_attrib(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_receiver,
            arg_target,
            arg_attrib_flags,
        };

        // Check arg count
        const auto receiver = params[arg_receiver];
        const auto target = params[arg_target];
        const auto attribFlags = params[arg_attrib_flags];
        if (receiver) {
            auto& player = Players[receiver];
            sendScoreAttrib(player, target, attribFlags);
        } else {
            sendScoreAttrib(TO_ALL, target, attribFlags);
        }
        return true;
    }

    auto send_money(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_receiver,
            arg_amount,
            arg_track_change,
            arg_color,
        };

        // Check arg count
        const auto receiver = params[arg_receiver];
        const auto amount = params[arg_amount];
        const auto isTrackChange = params[arg_track_change];
        if (receiver) {
            auto& player = Players[receiver];
            sendMoney(player, amount, isTrackChange);
        } else {
            sendMoney(TO_ALL, amount, isTrackChange);
        }
        return true;
    }

    auto send_status_icon(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_receiver,
            arg_status,
            arg_sprite,
            arg_color,
        };

        // Check arg count
        const auto receiver = params[arg_receiver];
        const auto status = params[arg_status];
        const auto sprite = GetAmxString(amx, params[arg_sprite], 0);
        const auto color = GetAmxString(amx, params[arg_color], 1);
        if (receiver) {
            auto& player = Players[receiver];
            sendStatusIcon(player, status, sprite, color);
        } else {
            sendStatusIcon(TO_ALL, status, sprite, color);
        }
        return true;
    }

    auto AmxxUserMessage::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"send_death_msg",    send_death_msg},
            {"send_score_attrib", send_score_attrib},
            {"send_money",        send_money},
            {"send_status_icon",  send_status_icon},

            {nullptr,             nullptr},
        };
        AddNatives(natives);
    }
}
