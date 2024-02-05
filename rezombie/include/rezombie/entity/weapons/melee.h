#pragma once

#include "rezombie/entity/weapons/base_weapon.h"
#include <metamod/engine.h>
#include <array>

namespace rz
{
    using namespace metamod::engine;

    constexpr auto WEAPON_MELEE = "weapon_knife";

    enum class MeleeAttackResult {
        Miss,
        Hit,
        HitWall,
    };

    enum class MeleeSound : size_t {
        Deploy,
        Hit,
        Slash,
        Stab,
        HitWall,
        MAX_SOUND_TYPES
    };

    class Melee : public BaseWeapon {
        int swingBaseDamage_ = 15;
        int swingDistance_ = 48;
        int stabBaseDamage_ = 65;
        int stabDistance_ = 32;
        // float damageMulti;
        std::array<std::vector<std::string>, toInt(MeleeSound::MAX_SOUND_TYPES)> sounds_;
        std::array<bool, toInt(MeleeSound::MAX_SOUND_TYPES)> isDefaultSounds_{};

        auto sounds(const MeleeSound soundType) -> std::vector<std::string>& {
            return sounds_[toInt(soundType)];
        }

        auto isDefaultSounds(const MeleeSound soundType) -> bool& {
            return isDefaultSounds_[toInt(soundType)];
        }

        auto addDefault(const MeleeSound soundType, const std::string& path) -> void {
            sounds_[toInt(soundType)].emplace_back(path);
        }

        public:
            explicit Melee(std::string handle) : BaseWeapon(
                std::move(handle),
                WEAPON_MELEE,
                WeaponType::Melee
            ) {
                setHud("weapon_knife");
                isDefaultSounds_.fill(true);
                addDefault(MeleeSound::Deploy, "weapons/knife_deploy1.wav");
                addDefault(MeleeSound::Hit, "weapons/knife_hit1.wav");
                addDefault(MeleeSound::Hit, "weapons/knife_hit2.wav");
                addDefault(MeleeSound::Hit, "weapons/knife_hit3.wav");
                addDefault(MeleeSound::Hit, "weapons/knife_hit4.wav");
                addDefault(MeleeSound::Slash, "weapons/knife_slash1.wav");
                addDefault(MeleeSound::Slash, "weapons/knife_slash2.wav");
                addDefault(MeleeSound::Stab, "weapons/knife_stab.wav");
                addDefault(MeleeSound::HitWall, "weapons/knife_hitwall1.wav");
            }

            auto executeMeleePrimaryAttack(int weapon, int player) const -> MeleeAttackResult {
                if (getPrimaryAttackForward() == -1) {
                    return MeleeAttackResult::Miss;
                }
                return static_cast<MeleeAttackResult>(ExecuteForward(getPrimaryAttackForward(), weapon, player));
            }

            auto executeMeleeSecondaryAttack(int weapon, int player) const -> MeleeAttackResult {
                if (getSecondaryAttackForward() == -1) {
                    return MeleeAttackResult::Miss;
                }
                return static_cast<MeleeAttackResult>(ExecuteForward(getSecondaryAttackForward(), weapon, player));
            }

            auto add(const MeleeSound soundType, const std::string& path) -> void {
                if (isDefaultSounds(soundType)) {
                    isDefaultSounds(soundType) = false;
                    sounds(soundType).clear();
                }
                sounds(soundType).emplace_back(path);
            }

            auto getRandom(const MeleeSound soundType) -> const std::string& {
                // if (sounds_[toInt(soundType)].empty()) {
                //     return nullptr;
                // }
                return sounds(soundType)[RandomLong(0, sounds(soundType).size() - 1)];
            }
    };
}
