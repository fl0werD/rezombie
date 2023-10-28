#pragma once

#include "rezombie/core/object.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/models/modules/models.h"
#include "rezombie/player/modules/nightvision.h"
#include "rezombie/player/modules/player_props.h"
#include "rezombie/player/modules/player_sounds.h"
#include "rezombie/player/modules/player_class.h"
#include "rezombie/weapons/modules/weapon.h"
#include "player_class.h"
#include "cssdk/dll/player.h"

namespace rz
{
    using namespace cssdk;

    class PlayerSubclass : public Object {
        int classId_ = 0;
        std::string description_{};
        std::string hudColor_{"white"};
        int propsId_ = 0;
        int modelId_ = 0;
        int soundsId_ = 0;
        int meleeId_ = 0;

      public:
        PlayerSubclass(
            std::string handle,
            int classId
        ) : Object(std::move(handle)),
            classId_(classId) {
            //setProps(Props.add(handle));
            setModel(Models.add(getHandle(), "models/player.mdl"));
            setSounds(PlayerSounds.add(getHandle()));
            setMelee(Weapons.add(getHandle(), WeaponType::Melee));
            const auto& classRef = Classes[classId];
            if (!classRef) {
                return;
            }
            const auto& playerClass = classRef->get();
            setHudColor(playerClass.getHudColor());
            /*const auto& propsRef = Props[playerClass.getProps()];
            if (propsRef) {
                auto props = propsRef->get();
                auto props2 = std::addressof(props);
                props2->setHandle(handle);
                setProps(Props.add(props2));
            }*/
        }

        auto getClass() const { return classId_; }

        auto getDescription() const -> const std::string& { return description_; }

        auto setDescription(std::string description) -> void { description_ = std::move(description); }

        auto getHudColor() const -> const std::string& { return hudColor_; }

        auto setHudColor(std::string hudColor) -> void { hudColor_ = std::move(hudColor); }

        auto getProps() const { return propsId_; }

        auto setProps(int propsId) -> void { propsId_ = propsId; }

        auto getModel() const { return modelId_; }

        auto setModel(int modelId) -> void { modelId_ = modelId; }

        auto getSounds() const { return soundsId_; }

        auto setSounds(int soundsId) -> void { soundsId_ = soundsId; }

        auto getMelee() const { return meleeId_; }

        auto setMelee(int meleeId) -> void { meleeId_ = meleeId; }
    };
}
