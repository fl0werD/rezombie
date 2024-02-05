#include "rezombie/player/api/player.h"
#include "rezombie/player/player.h"
#include "rezombie/player/players.h"
#include "rezombie/currency/modules/currency.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/core/api/amxx_helper.h"
#include "rezombie/gamerules/game_rules.h"

namespace rz
{
    using namespace amxx;

    auto AmxxPlayer::Joining(int player) const -> void {
        executeForward(PlayerForward::Joining, player);
    }

    auto AmxxPlayer::Joined(int player) const -> void {
        executeForward(PlayerForward::Joined, player);
    }

    auto AmxxPlayer::GiveDefaultItems(int player, int classId) const -> ForwardReturn {
        return executeForward(PlayerForward::GiveDefaultItems, player, classId);
    }

    auto AmxxPlayer::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            PlayerForward::Joining,
            RegisterForward("@player_joining", e::Ignore, p::Cell, p::Done)
        );
        setForward(
            PlayerForward::GiveDefaultItems,
            RegisterForward("@player_give_default_items", e::Ignore, p::Cell, p::Cell, p::Done)
        );
    }

    enum class RzPlayerVars {
        Health,
        Team,
        Class,
        Subclass,
        KeepSubclass,
        Model,
        Currency,
        ActiveWeapon,
        Ammo,
        MapCamera,
        NextMapCameraTime,
    };

    const std::unordered_map<std::string, RzPlayerVars> PlayerVarsMap = {
        {"health", RzPlayerVars::Health},
        {"team", RzPlayerVars::Team},
        {"class", RzPlayerVars::Class},
        {"subclass", RzPlayerVars::Subclass},
        {"keep_subclass", RzPlayerVars::KeepSubclass},
        {"model", RzPlayerVars::Model},
        {"currency", RzPlayerVars::Currency},
        {"active_weapon", RzPlayerVars::ActiveWeapon},
        {"ammo", RzPlayerVars::Ammo},
        {"map_camera", RzPlayerVars::MapCamera},
        {"next_map_camera_time", RzPlayerVars::NextMapCameraTime},
    };

    auto HandlePlayerVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = RzPlayerVars;

        const int playerId = params[arg_player];
        CHECK_PLAYER(playerId)
        auto& player = Players[playerId];
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(PlayerVarsMap, key);
        CHECK_VAR_EXISTS("Invalid player '%s' var", key)
        switch (*var) {
            case vars::Health: {
                if (isGetter) {
                    return std::max(0, player.getHealth());
                } else {
                    // Invalid set var
                }
                break;
            }
            case vars::Team: {
                if (isGetter) {
                    return toInt(player.getTeam());
                } else {
                    // Invalid set var
                }
                break;
            }
            case vars::Class: {
                if (isGetter) {
                    return player.getClass();
                } else {
                    // Invalid set var
                }
                break;
            }
            case vars::Subclass: {
                if (isGetter) {
                    return player.getSubclass();
                } else {
                    // Invalid set var
                }
                break;
            }
            case vars::KeepSubclass: {
                if (isGetter) {
                    return player.getKeepSubclass(*Address(amx, params[arg_3]));
                } else {
                    player.setKeepSubclass(*Address(amx, params[arg_3]), *Address(amx, params[arg_4]));
                }
                break;
            }
            case vars::Model: {
                if (isGetter) {
                    return player.getModel();
                } else {
                    player.ChangeModel(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Currency: {
                const auto currencyId = *Address(amx, params[arg_3]);
                const auto& currencyRef = Currency[currencyId];
                if (!currencyRef) {
                    // throw
                    return false;
                }
                const auto currency = currencyRef->get();
                if (isGetter) {
                    return currency.executeGet(player);
                } else {
                    return currency.executeSet(player, *Address(amx, params[arg_4]));
                }
            }
            case vars::ActiveWeapon: {
                if (isGetter) {
                    const auto& activeItem = player.getActiveItem();
                    if (activeItem == nullptr) {
                        return 0;
                    } else {
                        return activeItem->vars->impulse;
                    }
                } else {
                    // TODO: do or invalid
                }
                break;
            }
            case vars::Ammo: {
                const auto weaponEdict = EdictByIndex(*Address(amx, params[arg_3]));
                const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
                if (isGetter) {
                    return player.getAmmo(baseWeapon->primary_ammo_type);
                } else {
                    player.setAmmo(baseWeapon->primary_ammo_type, *Address(amx, params[arg_4]));
                }
                break;
            }
            case vars::MapCamera: {
                auto& camera = player.getMapCameraVars();
                if (isGetter) {
                    return camera.getCamera();
                } else {
                    player.setMapCamera(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::NextMapCameraTime: {
                auto& camera = player.getMapCameraVars();
                if (isGetter) {
                    return FloatToCell(camera.getNextCameraTime());
                } else {
                    camera.setNextCameraTime(CellToFloat(*Address(amx, params[arg_3])));
                }
                break;
            }
        }
        return true;
    }

    auto get_player_var(Amx* amx, cell* params) -> cell {
        return HandlePlayerVar(amx, params, true);
    }

    auto set_player_var(Amx* amx, cell* params) -> cell {
        return HandlePlayerVar(amx, params, false);
    }

    auto is_player_extra_exists(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_type,
            arg_key,
        };

        const int playerId = params[arg_player];
        CHECK_PLAYER(playerId)
        auto& player = Players[playerId];
        const auto type = static_cast<ExtraType>(params[arg_type]);
        const auto key = GetAmxString(amx, params[arg_key]);
        return player.Extras().isKeyExists(type, key);
    }

    auto HandlePlayerExtra(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_type,
            arg_key,
            arg_4,
            arg_5,
        };

        const int playerId = params[arg_player];
        CHECK_PLAYER(playerId)
        auto& player = Players[playerId];
        const auto type = static_cast<ExtraType>(params[arg_type]);
        const auto* key = GetAmxString(amx, params[arg_key], 0);
        switch (type) {
            case ExtraType::Int: {
                auto& extra = player.Extras().getInts();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid player extra '%s' var (int)", key)
                    return extra[key];
                } else {
                    extra[key] = *Address(amx, params[arg_4]);
                }
                break;
            }
            case ExtraType::Float: {
                auto& extra = player.Extras().getFloats();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid player extra '%s' var (float)", key)
                    return FloatToCell(extra[key]);
                } else {
                    extra[key] = CellToFloat(*Address(amx, params[arg_4]));
                }
                break;
            }
            case ExtraType::String: {
                auto& extra = player.Extras().getStrings();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid player extra '%s' var (string)", key)
                    SetAmxString(amx, params[arg_4], extra[key].c_str(), *Address(amx, params[arg_5]));
                } else {
                    extra[key] = GetAmxString(amx, params[arg_4], 1);
                }
                break;
            }
        }
        return true;
    }

    auto get_player_extra(Amx* amx, cell* params) -> cell {
        return HandlePlayerExtra(amx, params, true);
    }

    auto set_player_extra(Amx* amx, cell* params) -> cell {
        return HandlePlayerExtra(amx, params, false);
    }

    auto is_valid_attacker(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_attacker,
            arg_player,
            arg_self,
        };

        const int attackerId = params[arg_attacker];
        const int playerId = params[arg_player];
        const bool self = params[arg_self];
        const auto& attacker = Players[attackerId];
        const auto& player = Players[playerId];
        // connect attacker?
        GameRules.PlayerCanTakeDamage(player, attacker);
        return true;
    }

    auto change_player_class(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_class,
            arg_attacker,
        };

        const int playerId = params[arg_player];
        const int classId = params[arg_class];
        const int attackerId = params[arg_attacker];
        auto& player = Players[playerId];
        if (attackerId != 0) {
            auto& attacker = Players[attackerId];
            return toInt(player.ChangeClass(classId, &attacker));
        }
        return toInt(player.ChangeClass(classId));
    }

    auto change_player_subclass(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_subclass,
        };

        const int playerId = params[arg_player];
        const int subclassId = params[arg_subclass];
        auto& player = Players[playerId];
        return toInt(player.ChangeSubclass(subclassId));
    }

    auto change_player_flashlight(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_flashlight,
        };

        const int playerId = params[arg_player];
        const int flashlightId = params[arg_flashlight];
        auto& player = Players[playerId];
        player.Flashlight().setId(flashlightId);
        return true;
    }

    auto change_player_night_vision(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_night_vision,
        };

        const int playerId = params[arg_player];
        const int nightVisionId = params[arg_night_vision];
        auto& player = Players[playerId];
        return player.ChangeNightVision(nightVisionId);
    }

    auto player_joined(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
        };

        auto& player = Players[params[arg_player]];
        player.Joined();
        return true;
    }

    auto respawn_player(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_timer,
        };

        auto& player = Players[params[arg_player]];
        const auto timer = params[arg_timer];
        if (timer == 0) {
            player.RoundRespawn();
        } else {
            player.RoundRespawn();
        }
        return true;
    }

    auto freeze_player(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_freeze_time,
            arg_in_air,
        };

        const int playerId = params[arg_player];
        const float freezeTime = CellToFloat(params[arg_freeze_time]);
        // const bool inAir = params[arg_in_air];
        auto& player = Players[playerId];
        player.Freeze(freezeTime);
        return true;
    }

    auto is_player_freeze(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
        };

        const int playerId = params[arg_player];
        const auto& player = Players[playerId];
        return player.isFrozen();
    }

    auto send_hud(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_channel,
            arg_hud_params,
            arg_text,
            arg_text_arguments,
        };

        struct HudParams {
            cell x;
            cell y;
            cell color1[32];
            cell color2[32];
            cell effect;
            cell fadeInTime;
            cell holdTime;
            cell fadeOutTime;
            cell fxTime;
        };

        const int playerId = params[arg_player];
        // check 0..3
        const int channel = params[arg_channel];
        const auto value = *reinterpret_cast<HudParams*>(Address(amx, params[arg_hud_params]));
        const auto color1 = Colors.parse(CellToString(value.color1, sizeof(value.color1)));
        if (!color1) {
            return false;
        }
        const auto color2 = Colors.parse(CellToString(value.color2, sizeof(value.color2)));
        if (!color2) {
            return false;
        }
        auto length = 0;
        const auto text = FormatAmxString(amx, params, arg_text, &length);
        const auto& hudParams = HudMessageParams()
            .setX(CellToFloat(value.x))
            .setY(CellToFloat(value.y))
            .setEffect(value.effect)
            .setColor1(*color1)
            .setColor2(*color2)
            .setFadeInTime(CellToFloat(value.fadeInTime))
            .setHoldTime(CellToFloat(value.holdTime))
            .setFadeOutTime(CellToFloat(value.fadeOutTime))
            .setFxTime(CellToFloat(value.fxTime))
            .setChannel(channel);
        if (playerId) {
            const auto& player = Players[playerId];
            netHudMessage(player, hudParams, text);
        } else {
            netHudMessage(TO_ALL, hudParams, text);
        }
        return true;
    }

    auto send_large_hud(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_hud_params,
            arg_text,
            arg_text_arguments,
        };

        struct LargeHudParams {
            cell x;
            cell y;
            cell color[32];
            cell effect;
            cell fadeInTime;
            cell holdTime;
            cell fadeOutTime;
            cell fxTime;
        };

        auto length = 0;
        const int playerId = params[arg_player];
        const auto value = *reinterpret_cast<LargeHudParams*>(Address(amx, params[arg_hud_params]));
        const auto color = Colors.parse(CellToString(value.color, sizeof(value.color)));
        if (!color) {
            return false;
        }
        const auto text = FormatAmxString(amx, params, arg_text, &length);
        const auto& hudParams = LargeHudMessageParams()
            .setX(CellToFloat(value.x))
            .setY(CellToFloat(value.y))
            .setEffect(value.effect)
            .setColor(*color)
            .setFadeInTime(CellToFloat(value.fadeInTime))
            .setHoldTime(CellToFloat(value.holdTime))
            .setFadeOutTime(CellToFloat(value.fadeOutTime))
            .setFxTime(CellToFloat(value.fxTime));
        if (playerId) {
            const auto& player = Players[playerId];
            netDHudMessage(player, hudParams, text);
        } else {
            netDHudMessage(TO_ALL, hudParams, text);
        }
        return true;
    }

    auto send_weapon_anim(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_anim_number,
            arg_body,
        };

        const int playerId = params[arg_player];
        const int animNumber = params[arg_anim_number];
        const int body = params[arg_body];
        auto& player = Players[playerId];
        player.SendWeaponAnim(animNumber, body);
        return true;
    }

    auto AmxxPlayer::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_player_var", get_player_var},
            {"set_player_var", set_player_var},
            {"is_player_extra_exists", is_player_extra_exists},
            {"get_player_extra", get_player_extra},
            {"set_player_extra", set_player_extra},
            {"is_valid_attacker", is_valid_attacker},
            {"change_player_class", change_player_class},
            {"change_player_subclass", change_player_subclass},
            {"change_player_flashlight", change_player_flashlight},
            {"change_player_night_vision", change_player_night_vision},
            {"player_joined", player_joined},
            {"respawn_player", respawn_player},
            {"freeze_player", freeze_player},
            {"is_player_freeze", is_player_freeze},

            {"send_hud", send_hud},
            {"send_large_hud", send_large_hud},
            {"send_weapon_anim", send_weapon_anim},

            {nullptr, nullptr},
        };
        AddNatives(natives);
    }
}
