#pragma once

#include "rezombie/core/base_object.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/modules/player_model.h"
#include "rezombie/modules/player_nightvision.h"
#include "rezombie/modules/player_props.h"
#include "rezombie/modules/player_sound.h"
#include "rezombie/modules/weapon.h"
#include "rezombie/player/player_class.h"
#include <cssdk/dll/player.h>

namespace rz::player
{
    using namespace cssdk;

    class PlayerSubclass : public BaseObject
    {
      private:
        int classIndex_ = 0;
        std::string description_;
        Color24 hudColor_ = {255, 255, 255};
        int propsIndex_ = 0;
        int modelIndex_ = 0;
        int soundIndex_ = 0;
        int meleeIndex_ = 0;
        int nightVisionIndex_ = 0;

      public:
        PlayerSubclass(std::string handle, int classIndex) : BaseObject(std::move(handle)), classIndex_(classIndex)
        {
            setProps(playerPropsModule.add(handle));
            setModel(playerModelModule.add(handle));
            setSound(playerSoundModule.add(handle));
            setMelee(weaponModule.add(handle, WeaponType::Melee));
            setNightVision(playerNightVisionModule.add(handle));
        }

        auto getClass() const
        {
            return classIndex_;
        }

        auto getDescription() const -> const std::string&
        {
            return description_;
        }

        auto setDescription(std::string description) -> void
        {
            description_ = std::move(description);
        }

        auto getHudColor() const -> const Color24&
        {
            return hudColor_;
        }

        auto setHudColor(Color24 hudColor) -> void
        {
            hudColor_ = hudColor;
        }

        auto getProps() const
        {
            return propsIndex_;
        }

        auto setProps(int propsIndex) -> void
        {
            propsIndex_ = propsIndex;
        }

        auto getModel() const
        {
            return modelIndex_;
        }

        auto setModel(int modelIndex) -> void
        {
            modelIndex_ = modelIndex;
        }

        auto getSound() const
        {
            return soundIndex_;
        }

        auto setSound(int soundIndex) -> void
        {
            soundIndex_ = soundIndex;
        }

        auto getMelee() const
        {
            return meleeIndex_;
        }

        auto setMelee(int meleeIndex) -> void
        {
            meleeIndex_ = meleeIndex;
        }

        auto getNightVision() const
        {
            return nightVisionIndex_;
        }

        auto setNightVision(int nightVisionIndex) -> void
        {
            nightVisionIndex_ = nightVisionIndex;
        }
    };
}
