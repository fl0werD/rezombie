#pragma once

#include "rezombie/core/object.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/models/modules/models_pack.h"
#include "rezombie/player/modules/nightvision.h"
#include "rezombie/player/modules/player_props.h"
#include "rezombie/player/modules/player_sounds.h"
#include <cssdk/dll/player.h>

namespace rz
{
    class PlayerClass : public Object {
        Team team_;
        std::string hudColor_{"white"};
        int propsId_{};
        int modelsPackId_{};
        int soundsId_{};
        int meleeId_{};
        int giveDefaultItemsForward_ = FORWARD_INVALID;

      public:
        PlayerClass(
            std::string handle,
            Team team
        ) : Object(std::move(handle)),
            team_(team) {
            setProps(Props.add(getHandle()));
            setModelsPack(ModelsPack.add(getHandle()));
            setSounds(PlayerSounds.add(getHandle()));
            setMelee(Weapons.add(getHandle(), WeaponType::Melee));
        }

        auto getHudColor() const -> const std::string& { return hudColor_; }

        auto setHudColor(std::string hudColor) -> void { hudColor_ = std::move(hudColor); }

        auto getTeam() const -> Team { return team_; }

        auto getProps() const -> int { return propsId_; }

        auto setProps(int propsId) -> void { propsId_ = propsId; }

        auto getModelsPack() const -> int { return modelsPackId_; }

        auto setModelsPack(int modelsPackId) -> void { modelsPackId_ = modelsPackId; }

        auto getSounds() const -> int { return soundsId_; }

        auto setSounds(int soundsId) -> void { soundsId_ = soundsId; }

        auto getMelee() const -> int { return meleeId_; }

        auto setMelee(int meleeId) -> void { meleeId_ = meleeId; }

        auto getGiveDefaultItemsForward() const -> int { return giveDefaultItemsForward_; }

        auto setGiveDefaultItemsForward(int giveDefaultItemsForward) -> void {
            giveDefaultItemsForward_ = giveDefaultItemsForward;
        }

        auto executeGiveDefaultItems(int playerId) const -> bool {
            if (getGiveDefaultItemsForward() == -1) {
                return false;
            }
            ExecuteForward(getGiveDefaultItemsForward(), playerId);
            return true;
        }
    };
}
