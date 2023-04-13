#pragma once

#include "rezombie/core/base_object.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/modules/weapon.h"
#include "rezombie/modules/player_model.h"
#include "rezombie/modules/player_nightvision.h"
#include "rezombie/modules/player_props.h"
#include "rezombie/modules/player_sound.h"
#include "cssdk/dll/player.h"

namespace rz::player
{
    using namespace cssdk;

    class PlayerClass : public BaseObject {
      private:
        Team team_;
        Color24 hudColor_ = {255, 255, 255};
        int propsIndex_ = 0;
        int modelIndex_ = 0;
        int soundIndex_ = 0;
        int meleeIndex_ = 0;
        int nightVisionIndex_ = 0;

      public:
        PlayerClass(std::string handle, Team team) : BaseObject(std::move(handle)), team_(team) {
            setProps(playerPropsModule.add(handle));
            setModel(playerModelModule.add(handle));
            setSound(playerSoundModule.add(handle));
            setMelee(weaponModule.add(handle, WeaponType::Melee));
            setNightVision(playerNightVisionModule.add(handle));
        }

        auto getTeam() const -> Team { return team_; }

        auto getHudColor() const -> const Color24& { return hudColor_; }

        auto setHudColor(Color24 hudColor) -> void { hudColor_ = hudColor; }

        auto getProps() const { return propsIndex_; }

        auto setProps(int propsIndex) -> void { propsIndex_ = propsIndex; }

        auto getModel() const { return modelIndex_; }

        auto setModel(int modelIndex) -> void { modelIndex_ = modelIndex; }

        auto getSound() const { return soundIndex_; }

        auto setSound(int soundIndex) -> void { soundIndex_ = soundIndex; }

        auto getMelee() const { return meleeIndex_; }

        auto setMelee(int meleeIndex) -> void { meleeIndex_ = meleeIndex; }

        auto getNightVision() const { return nightVisionIndex_; }

        auto setNightVision(int nightVisionIndex) -> void { nightVisionIndex_ = nightVisionIndex; }
    };
}
