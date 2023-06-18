#include "rezombie/amxmodx/weapon.h"
#include "rezombie/modules/weapon.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>
#include <core/regamedll_api.h>
#include <metamod/engine.h>

namespace rz
{
    using namespace metamod::engine;
    using namespace amx;
    using namespace amxx;
    using namespace player;

    auto AMX_NATIVE_CALL rz_create_weapon(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_weapon_type,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto weaponType = static_cast<WeaponType>(params[arg_weapon_type]);
        const auto weaponIndex = weaponModule.add(handle, weaponType);
        return weaponIndex;
    }

    auto AMX_NATIVE_CALL rz_get_weapon_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon,
            arg_var,
            arg_3,
            arg_4,
        };

        const int weaponIndex = params[arg_weapon];
        const auto weaponRef = weaponModule[weaponIndex];
        if (!weaponRef) {
            // Invalid index
            return false;
        }
        auto& baseWeapon = weaponRef->get();
        const auto weapon = dynamic_cast<Weapon*>(&baseWeapon);
        using var = WeaponVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                SetAmxString(amx, params[arg_3], baseWeapon.getHandle().c_str(), params[arg_4]);
                break;
            }
            case var::Name: {
                SetAmxString(amx, params[arg_3], baseWeapon.getName().c_str(), *Address(amx, params[arg_4]));
                break;
            }
            case var::ViewModel: {
                SetAmxString(amx, params[arg_3], baseWeapon.getViewModel().c_str(), params[arg_4]);
                break;
            }
            case var::PlayerModel: {
                SetAmxString(amx, params[arg_3], baseWeapon.getPlayerModel().c_str(), params[arg_4]);
                break;
            }
            case var::PlayerModelBody: {
                return baseWeapon.getPlayerModelBody();
            }
            case var::WorldModel: {
                SetAmxString(amx, params[arg_3], baseWeapon.getWorldModel().c_str(), params[arg_4]);
                break;
            }
            case var::WorldModelBody: {
                return baseWeapon.getWorldModelBody();
            }
            case var::Hud: {
                SetAmxString(amx, params[arg_3], baseWeapon.getHud().c_str(), params[arg_4]);
                break;
            }
            case var::MaxClip: {
                return baseWeapon.getMaxClip();
            }
            case var::MaxAmmo: {
                return baseWeapon.getMaxAmmo();
            }
            case var::InventorySlot: {
                return toInt(baseWeapon.getInventorySlot());
            }
            case var::BaseAccuracy: {
                if (weapon == nullptr) {
                    return 0;
                }
                return FloatToCell(weapon->getBaseAccuracy());
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_set_weapon_var(Amx* amx, cell* params) -> cell {
        using p = ForwardParam;

        enum {
            arg_count,
            arg_weapon,
            arg_var,
            arg_value,
        };

        const int weaponIndex = params[arg_weapon];
        const auto weaponRef = weaponModule[weaponIndex];
        if (!weaponRef) {
            // Invalid index
            return false;
        }
        auto& baseWeapon = weaponRef->get();
        const auto weapon = dynamic_cast<Weapon*>(&baseWeapon);
        using var = WeaponVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                // Invalid set var
                break;
            }
            case var::Name: {
                baseWeapon.setName(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::ViewModel: {
                baseWeapon.setViewModel(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::PlayerModel: {
                baseWeapon.setPlayerModel(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::PlayerModelBody: {
                baseWeapon.setPlayerModelBody(*Address(amx, params[arg_value]));
                break;
            }
            case var::WorldModel: {
                baseWeapon.setWorldModel(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::WorldModelBody: {
                baseWeapon.setWorldModelBody(*Address(amx, params[arg_value]));
                break;
            }
            case var::Hud: {
                baseWeapon.setHud(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::MaxClip: {
                baseWeapon.setMaxClip(*Address(amx, params[arg_value]));
                break;
            }
            case var::MaxAmmo: {
                baseWeapon.setMaxAmmo(*Address(amx, params[arg_value]));
                break;
            }
            case var::InventorySlot: {
                baseWeapon.setInventorySlot(static_cast<InventorySlot>(*Address(amx, params[arg_value])));
                break;
            }
            case var::Weight: {
                baseWeapon.setWeight(*Address(amx, params[arg_value]));
                break;
            }
            case var::Flags: {
                baseWeapon.setFlags(*Address(amx, params[arg_value]));
                break;
            }
            case var::BaseAccuracy: {
                if (weapon == nullptr) {
                    return false;
                }
                weapon->setBaseAccuracy(CellToFloat(*Address(amx, params[arg_value])));
                break;
            }
            case var::CrosshairSize: {
                if (weapon == nullptr) {
                    return false;
                }
                weapon->setCrosshairSize(static_cast<CrosshairSize>(*Address(amx, params[arg_value])));
                break;
            }
            case var::ForwardDeploy: {
                const auto deploy =
                    RegisterSpForwardByName(amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Done);
                if (deploy == -1) {
                    return false;
                }
                baseWeapon.setDeployForward(deploy);
                break;
            }
            case var::ForwardHolster: {
                const auto holster =
                    RegisterSpForwardByName(amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Done);
                if (holster == -1) {
                    return false;
                }
                baseWeapon.setHolsterForward(holster);
                break;
            }
            case var::ForwardMaxSpeed: {
                const auto maxSpeed = RegisterSpForwardByName(
                    amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Cell, p::Done
                );
                if (maxSpeed == -1) {
                    return false;
                }
                baseWeapon.setMaxSpeedForward(maxSpeed);
                break;
            }
            case var::ForwardPrimaryAttack: {
                const auto primaryAttack = RegisterSpForwardByName(
                    amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Cell, p::Cell, p::Done
                );
                if (primaryAttack == -1) {
                    return false;
                }
                baseWeapon.setPrimaryAttackForward(primaryAttack);
                break;
            }
            case var::ForwardSecondaryAttack: {
                const auto secondaryAttack = RegisterSpForwardByName(
                    amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Cell, p::Cell, p::Done
                );
                if (secondaryAttack == -1) {
                    return false;
                }
                baseWeapon.setSecondaryAttackForward(secondaryAttack);
                break;
            }
            case var::ForwardReload: {
                const auto reload =
                    RegisterSpForwardByName(amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Done);
                if (reload == -1) {
                    return false;
                }
                baseWeapon.setReloadForward(reload);
                break;
            }
            case var::ForwardIdle: {
                const auto idle = RegisterSpForwardByName(
                    amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Float, p::Done
                );
                if (idle == -1) {
                    return false;
                }
                baseWeapon.setIdleForward(idle);
                break;
            }
            case var::ForwardFireRemaining: {
                if (weapon == nullptr) {
                    return false;
                }
                const auto fireRemaining = RegisterSpForwardByName(
                    amx, GetAmxString(amx, params[arg_value]), p::Cell, p::Cell, p::Cell, p::Cell, p::Done
                );
                if (fireRemaining == -1) {
                    return false;
                }
                weapon->setFireRemainingForward(fireRemaining);
                break;
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_weapon_begin(Amx*, cell*) -> cell {
        return weaponModule.begin();
    }

    auto AMX_NATIVE_CALL rz_weapon_end(Amx*, cell*) -> cell {
        return weaponModule.end();
    }

    auto AMX_NATIVE_CALL rz_find_weapon(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return weaponModule[handle];
    }

    auto AMX_NATIVE_CALL rz_give_weapon(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_handle,
            arg_give_type,
        };

        const int playerIndex = params[arg_player];
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto giveType = static_cast<GiveType>(params[arg_give_type]);
        const auto weaponIndex = weaponModule[handle];
        auto& player = players[playerIndex];
        player.GiveWeapon(weaponIndex, giveType);
        return 1;
    }

    auto AMX_NATIVE_CALL rz_give_weapon_fast(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_weapon,
            arg_give_type,
        };

        const int playerIndex = params[arg_player];
        const int weaponIndex = params[arg_weapon];
        const auto giveType = static_cast<GiveType>(params[arg_give_type]);
        auto& player = players[playerIndex];
        player.GiveWeapon(weaponIndex, giveType);
        return 1;
    }

    auto AMX_NATIVE_CALL rz_get_player_ammo(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_weapon_entity,
        };

        const int playerIndex = params[arg_player];
        const int weaponEntityIndex = params[arg_weapon_entity];
        const auto& player = players[playerIndex];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        return player.getAmmo(baseWeapon->primary_ammo_type);
    }

    auto AMX_NATIVE_CALL rz_set_player_ammo(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_weapon_entity,
            arg_amount,
        };

        const int playerIndex = params[arg_player];
        const int weaponEntityIndex = params[arg_weapon_entity];
        const int amount = params[arg_amount];
        auto& player = players[playerIndex];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        player.setAmmo(baseWeapon->primary_ammo_type, amount);
        return true;
    }

    auto AMX_NATIVE_CALL rz_weapon_default_deploy(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_draw_anim,
            arg_player_animation,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerIndex = params[arg_player];
        const int drawAnim = params[arg_draw_anim];
        const auto playerAnimation = GetAmxString(amx, params[arg_player_animation]);
        auto& player = players[playerIndex];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        return WeaponDefaultDeploy(player, baseWeapon, drawAnim, playerAnimation);
    }

    auto AMX_NATIVE_CALL rz_weapon_default_reload(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_reload_anim,
            arg_reload_time,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerIndex = params[arg_player];
        const int reloadAnim = params[arg_reload_anim];
        const float reloadTime = CellToFloat(params[arg_reload_time]);
        auto& player = players[playerIndex];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        return WeaponDefaultReload(player, baseWeapon, reloadAnim, reloadTime);
    }

    auto AMX_NATIVE_CALL rz_weapon_default_shotgun_reload(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_reload_anim,
            arg_reload_start_anim,
            arg_reload_time,
            arg_reload_start_time,
            arg_reload_sound_1,
            arg_reload_sound_2,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerIndex = params[arg_player];
        const int reloadAnim = params[arg_reload_anim];
        const int reloadStartAnim = params[arg_reload_start_anim];
        const float reloadTime = CellToFloat(params[arg_reload_time]);
        const float reloadStartTime = CellToFloat(params[arg_reload_start_time]);
        const auto reloadSound1 = GetAmxString(amx, params[arg_reload_sound_1], 0);
        const auto reloadSound2 = GetAmxString(amx, params[arg_reload_sound_2], 1);
        auto& player = players[playerIndex];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        return WeaponDefaultShotgunReload(
            player,
            baseWeapon,
            reloadAnim,
            reloadStartAnim,
            reloadTime,
            reloadStartTime,
            reloadSound1,
            reloadSound2
        );
    }

    auto AMX_NATIVE_CALL rz_weapon_kick_back(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_up_base,
            arg_lateral_base,
            arg_up_modifier,
            arg_lateral_modifier,
            arg_up_max,
            arg_lateral_max,
            arg_direction_change,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerIndex = params[arg_player];
        auto& player = players[playerIndex];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        const float upBase = CellToFloat(params[arg_up_base]);
        const float lateralBase = CellToFloat(params[arg_lateral_base]);
        const float upModifier = CellToFloat(params[arg_up_modifier]);
        const float lateralModifier = CellToFloat(params[arg_lateral_modifier]);
        const float upMax = CellToFloat(params[arg_up_max]);
        const float lateralMax = CellToFloat(params[arg_lateral_max]);
        const int directionChange = params[arg_direction_change];
        WeaponKickBack(
            player,
            baseWeapon,
            upBase,
            lateralBase,
            upModifier,
            lateralModifier,
            upMax,
            lateralMax,
            directionChange
        );
        return true;
    }

    auto AMX_NATIVE_CALL rz_weapon_throw_grenade(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_origin,
            arg_velocity,
            arg_action_time,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerIndex = params[arg_player];
        const auto originRef = Address(amx, params[arg_origin]);
        const auto velocityRef = Address(amx, params[arg_velocity]);
        const float actionTime = CellToFloat(params[arg_action_time]);
        const Vector origin{CellToFloat(originRef[0]), CellToFloat(originRef[1]), CellToFloat(originRef[2])};
        const Vector velocity{CellToFloat(velocityRef[0]), CellToFloat(velocityRef[1]), CellToFloat(velocityRef[2])};
        auto& player = players[playerIndex];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        const auto grenade = WeaponThrowGrenade(player, baseWeapon, origin, velocity, actionTime);
        return grenade->EdictIndex();
    }

    auto AmxxWeapon::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_create_weapon",                 rz_create_weapon},
            {"rz_get_weapon_var",                rz_get_weapon_var},
            {"rz_set_weapon_var",                rz_set_weapon_var},
            {"rz_weapon_begin",                  rz_weapon_begin},
            {"rz_weapon_end",                    rz_weapon_end},
            {"rz_find_weapon",                   rz_find_weapon},

            {"rz_give_weapon",                   rz_give_weapon},
            {"rz_give_weapon_fast",              rz_give_weapon_fast},

            {"rz_get_player_ammo",               rz_get_player_ammo},
            {"rz_set_player_ammo",               rz_set_player_ammo},

            {"rz_weapon_default_deploy",         rz_weapon_default_deploy},
            {"rz_weapon_default_reload",         rz_weapon_default_reload},
            {"rz_weapon_default_shotgun_reload", rz_weapon_default_shotgun_reload},
            {"rz_weapon_kick_back",              rz_weapon_kick_back},
            {"rz_weapon_throw_grenade",          rz_weapon_throw_grenade},

            {nullptr,                            nullptr},
        };
        AddNatives(natives);
    }
}
