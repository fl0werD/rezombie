#include "rezombie/weapons/api/weapon.h"
#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/player/players.h"
#include <metamod/engine.h>
#include <amxx/api.h>
#include <core/regamedll_api.h>

namespace rz
{
    using namespace metamod::engine;
    using namespace amx;
    using namespace amxx;

    enum class WeaponVars : int {
        Handle,
        Name,
        ViewModel,
        PlayerModel,
        WorldModel,
        Hud,
        MaxClip,
        MaxAmmo,
        InventorySlot,
        Weight,
        Flags,
        BaseAccuracy,
        CrosshairSize,
        ForwardDeploy,
        ForwardHolster,
        ForwardMaxSpeed,
        ForwardPrimaryAttack,
        ForwardSecondaryAttack,
        ForwardReload,
        ForwardIdle,
        ForwardFireRemaining,
    };

    const std::unordered_map<std::string, WeaponVars> WeaponVarsMap = {
        {"handle",                   WeaponVars::Handle},
        {"name",                     WeaponVars::Name},
        {"view_model",               WeaponVars::ViewModel},
        {"player_model",             WeaponVars::PlayerModel},
        {"world_model",              WeaponVars::WorldModel},
        {"hud",                      WeaponVars::Hud},
        {"max_clip",                 WeaponVars::MaxClip},
        {"max_ammo",                 WeaponVars::MaxAmmo},
        {"inventory_slot",           WeaponVars::InventorySlot},
        {"weight",                   WeaponVars::Weight},
        {"flags",                    WeaponVars::Flags},
        {"base_accuracy",            WeaponVars::BaseAccuracy},
        {"crosshair_size",           WeaponVars::CrosshairSize},
        {"forward_deploy",           WeaponVars::ForwardDeploy},
        {"forward_holster",          WeaponVars::ForwardHolster},
        {"forward_max_speed",        WeaponVars::ForwardMaxSpeed},
        {"forward_primary_attack",   WeaponVars::ForwardPrimaryAttack},
        {"forward_secondary_attack", WeaponVars::ForwardSecondaryAttack},
        {"forward_reload",           WeaponVars::ForwardReload},
        {"forward_idle",             WeaponVars::ForwardIdle},
        {"forward_fire_remaining",   WeaponVars::ForwardFireRemaining},
    };

    auto create_weapon(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_weapon_type,
        };

        // Check arg count
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto weaponType = static_cast<WeaponType>(params[arg_weapon_type]);
        const auto weaponId = Weapons.add(handle, weaponType);
        return weaponId;
    }

    auto HandleWeaponVars(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_weapon,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = WeaponVars;
        using p = ForwardParam;

        const int weaponId = params[arg_weapon];
        const auto weaponRef = Weapons[weaponId];
        if (!weaponRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(WeaponVarsMap, key);
        if (!var) {
            // Invalid index
            return false;
        }
        auto& baseWeapon = weaponRef->get();
        const auto weapon = dynamic_cast<Weapon*>(&baseWeapon);
        switch (*var) {
            case vars::Handle: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], baseWeapon.getHandle().c_str(), params[arg_4]);
                } else {
                    // Invalid set var
                }
                break;
            }
            case vars::Name: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], baseWeapon.getName().c_str(), *Address(amx, params[arg_4]));
                } else {
                    baseWeapon.setName(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::ViewModel: {
                if (isGetter) {
                    return baseWeapon.getViewModel();
                } else {
                    baseWeapon.setViewModel(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::PlayerModel: {
                if (isGetter) {
                    return baseWeapon.getPlayerModel();
                } else {
                    baseWeapon.setPlayerModel(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::WorldModel: {
                if (isGetter) {
                    return baseWeapon.getWorldModel();
                } else {
                    baseWeapon.setWorldModel(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Hud: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], baseWeapon.getHud().c_str(), params[arg_4]);
                } else {
                    baseWeapon.setHud(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::MaxClip: {
                if (isGetter) {
                    return baseWeapon.getMaxClip();
                } else {
                    baseWeapon.setMaxClip(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::MaxAmmo: {
                if (isGetter) {
                    return baseWeapon.getMaxAmmo();
                } else {
                    baseWeapon.setMaxAmmo(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::InventorySlot: {
                if (isGetter) {
                    return toInt(baseWeapon.getInventorySlot());
                } else {
                    baseWeapon.setInventorySlot(static_cast<InventorySlot>(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::Weight: {
                if (isGetter) {
                    return baseWeapon.getWeight();
                } else {
                    baseWeapon.setWeight(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Flags: {
                if (isGetter) {
                    return baseWeapon.getFlags();
                } else {
                    baseWeapon.setFlags(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::BaseAccuracy: {
                if (weapon == nullptr) {
                    return false;
                }
                if (isGetter) {
                    return FloatToCell(weapon->getBaseAccuracy());
                } else {
                    weapon->setBaseAccuracy(CellToFloat(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::CrosshairSize: {
                if (weapon == nullptr) {
                    return false;
                }
                if (isGetter) {
                    return toInt(weapon->getCrosshairSize());
                } else {
                    weapon->setCrosshairSize(static_cast<CrosshairSize>(*Address(amx, params[arg_3])));
                }
                break;
            }
            case vars::ForwardDeploy: {
                if (isGetter) {
                    return baseWeapon.getDeployForward();
                } else {
                    const auto deploy = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Done
                    );
                    if (deploy == FORWARD_INVALID) {
                        return false;
                    }
                    baseWeapon.setDeployForward(deploy);
                }
                break;
            }
            case vars::ForwardHolster: {
                if (isGetter) {
                    return baseWeapon.getHolsterForward();
                } else {
                    const auto holster = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Done
                    );
                    if (holster == FORWARD_INVALID) {
                        return false;
                    }
                    baseWeapon.setHolsterForward(holster);
                }
                break;
            }
            case vars::ForwardMaxSpeed: {
                if (isGetter) {
                    return baseWeapon.getMaxSpeedForward();
                } else {
                    const auto maxSpeed = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Cell, p::Done
                    );
                    if (maxSpeed == FORWARD_INVALID) {
                        return false;
                    }
                    baseWeapon.setMaxSpeedForward(maxSpeed);
                }
                break;
            }
            case vars::ForwardPrimaryAttack: {
                if (isGetter) {
                    return baseWeapon.getPrimaryAttackForward();
                } else {
                    const auto primaryAttack = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Cell, p::Cell, p::Done
                    );
                    if (primaryAttack == FORWARD_INVALID) {
                        return false;
                    }
                    baseWeapon.setPrimaryAttackForward(primaryAttack);
                }
                break;
            }
            case vars::ForwardSecondaryAttack: {
                if (isGetter) {
                    return baseWeapon.getSecondaryAttackForward();
                } else {
                    const auto secondaryAttack = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Cell, p::Cell, p::Done
                    );
                    if (secondaryAttack == FORWARD_INVALID) {
                        return false;
                    }
                    baseWeapon.setSecondaryAttackForward(secondaryAttack);
                }
                break;
            }
            case vars::ForwardReload: {
                if (isGetter) {
                    return baseWeapon.getReloadForward();
                } else {
                    const auto reload = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Done
                    );
                    if (reload == FORWARD_INVALID) {
                        return false;
                    }
                    baseWeapon.setReloadForward(reload);
                }
                break;
            }
            case vars::ForwardIdle: {
                if (isGetter) {
                    return baseWeapon.getIdleForward();
                } else {
                    const auto idle = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Float, p::Done
                    );
                    if (idle == FORWARD_INVALID) {
                        return false;
                    }
                    baseWeapon.setIdleForward(idle);
                }
                break;
            }
            case vars::ForwardFireRemaining: {
                if (weapon == nullptr) {
                    return FORWARD_INVALID;
                }
                if (isGetter) {
                    return weapon->getFireRemainingForward();
                } else {
                    const auto fireRemaining = RegisterSpForwardByName(
                        amx, GetAmxString(amx, params[arg_3]),
                        p::Cell, p::Cell, p::Cell, p::Cell, p::Done
                    );
                    if (fireRemaining == FORWARD_INVALID) {
                        return false;
                    }
                    weapon->setFireRemainingForward(fireRemaining);
                }
                break;
            }
        }
        return true;
    }

    auto get_weapon_var(Amx* amx, cell* params) -> cell {
        return HandleWeaponVars(amx, params, true);
    }

    auto set_weapon_var(Amx* amx, cell* params) -> cell {
        return HandleWeaponVars(amx, params, false);
    }

    auto weapon_begin(Amx*, cell*) -> cell {
        return Weapons.begin();
    }

    auto weapon_end(Amx*, cell*) -> cell {
        return Weapons.end();
    }

    auto find_weapon(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return Weapons[handle];
    }

    auto give_weapon(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_handle,
            arg_give_type,
        };

        auto& player = Players[params[arg_player]];
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto giveType = static_cast<GiveType>(params[arg_give_type]);
        const auto weaponIndex = Weapons[handle];
        player.GiveWeapon(weaponIndex, giveType);
        return 1;
    }

    auto give_weapon_by_id(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_weapon,
            arg_give_type,
        };

        auto& player = Players[params[arg_player]];
        const int weaponId = params[arg_weapon];
        const auto giveType = static_cast<GiveType>(params[arg_give_type]);
        player.GiveWeapon(weaponId, giveType);
        return 1;
    }

    auto weapon_default_deploy(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_draw_anim,
            arg_player_animation,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerId = params[arg_player];
        const int drawAnim = params[arg_draw_anim];
        const auto playerAnimation = GetAmxString(amx, params[arg_player_animation]);
        auto& player = Players[playerId];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        return WeaponDefaultDeploy(player, baseWeapon, drawAnim, playerAnimation);
    }

    auto weapon_default_reload(Amx*, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_reload_anim,
            arg_reload_time,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerId = params[arg_player];
        const int reloadAnim = params[arg_reload_anim];
        const float reloadTime = CellToFloat(params[arg_reload_time]);
        auto& player = Players[playerId];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        return WeaponDefaultReload(player, baseWeapon, reloadAnim, reloadTime);
    }

    auto weapon_default_shotgun_reload(Amx* amx, cell* params) -> cell {
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
        const int playerId = params[arg_player];
        const int reloadAnim = params[arg_reload_anim];
        const int reloadStartAnim = params[arg_reload_start_anim];
        const float reloadTime = CellToFloat(params[arg_reload_time]);
        const float reloadStartTime = CellToFloat(params[arg_reload_start_time]);
        const auto reloadSound1 = GetAmxString(amx, params[arg_reload_sound_1], 0);
        const auto reloadSound2 = GetAmxString(amx, params[arg_reload_sound_2], 1);
        auto& player = Players[playerId];
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

    auto weapon_kick_back(Amx*, cell* params) -> cell {
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
        const int playerId = params[arg_player];
        auto& player = Players[playerId];
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

    auto weapon_throw_grenade(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_weapon_entity,
            arg_player,
            arg_origin,
            arg_velocity,
            arg_action_time,
        };

        const int weaponEntityIndex = params[arg_weapon_entity];
        const int playerId = params[arg_player];
        const auto originRef = Address(amx, params[arg_origin]);
        const auto velocityRef = Address(amx, params[arg_velocity]);
        const float actionTime = CellToFloat(params[arg_action_time]);
        const Vector origin{CellToFloat(originRef[0]), CellToFloat(originRef[1]), CellToFloat(originRef[2])};
        const Vector velocity{CellToFloat(velocityRef[0]), CellToFloat(velocityRef[1]), CellToFloat(velocityRef[2])};
        auto& player = Players[playerId];
        const auto weaponEdict = EdictByIndex(weaponEntityIndex);
        const auto baseWeapon = EntityPrivateData<PlayerWeaponBase>(weaponEdict);
        const auto grenade = WeaponThrowGrenade(player, baseWeapon, origin, velocity, actionTime);
        return grenade->EdictIndex();
    }

    auto AmxxWeapon::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_weapon",                 create_weapon},
            {"get_weapon_var",                get_weapon_var},
            {"set_weapon_var",                set_weapon_var},
            {"weapon_begin",                  weapon_begin},
            {"weapon_end",                    weapon_end},
            {"find_weapon",                   find_weapon},

            {"give_weapon",                   give_weapon},
            {"give_weapon_by_id",             give_weapon_by_id},

            {"weapon_default_deploy",         weapon_default_deploy},
            {"weapon_default_reload",         weapon_default_reload},
            {"weapon_default_shotgun_reload", weapon_default_shotgun_reload},
            {"weapon_kick_back",              weapon_kick_back},
            {"weapon_throw_grenade",          weapon_throw_grenade},

            {nullptr,                         nullptr},
        };
        AddNatives(natives);
    }
}
