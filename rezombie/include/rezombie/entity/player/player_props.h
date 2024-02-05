#pragma once

#include "rezombie/core/object.h"
#include "cssdk/public/regamedll/regamedll_consts.h"

namespace rz
{
    using namespace cssdk;

    class PlayerProps : public Object {
        int health_ = 100;
        int baseHealth_ = 0;
        int speed_ = 0;
        float gravity_ = 1.f;
        int armor_ = 0;
        bool helmet_ = false;
        bool footSteps_ = true;
        bool weaponsInteraction_ = true;
        float damageBodyVelMod_ = 0.5f;
        float damageHeadVelMod_ = 0.65f;
        int knockback_ = 0; // float?
        byte bloodColor_ = BLOOD_COLOR_RED;
        Rendering renderMode_ = Rendering::Normal;
        int renderAmount_ = 0;
        std::string renderColor_ = {};
        RenderingFx renderFx_ = RenderingFx::None;

      public:
        explicit PlayerProps(std::string handle) : Object(std::move(handle)) {}

        auto getHealth() const { return health_; }

        auto setHealth(int health) -> void { health_ = health; }

        auto getBaseHealth() const { return baseHealth_; }

        auto setBaseHealth(int baseHealth) -> void { baseHealth_ = baseHealth; }

        auto getSpeed() const { return speed_; }

        auto setSpeed(int speed) -> void { speed_ = speed; }

        auto getGravity() const { return gravity_; }

        auto setGravity(float gravity) -> void { gravity_ = gravity; }

        auto getArmor() const { return armor_; }

        auto setArmor(int armor) -> void { armor_ = armor; }

        auto getHelmet() const { return helmet_; }

        auto setHelmet(bool helmet) -> void { helmet_ = helmet; }

        auto getFootSteps() const { return footSteps_; }

        auto setFootSteps(int footSteps) -> void { footSteps_ = footSteps; }

        auto getWeaponsInteraction() const { return weaponsInteraction_; }

        auto setWeaponsInteraction(int weaponsInteraction) -> void { weaponsInteraction_ = weaponsInteraction; }

        auto getBloodColor() const { return bloodColor_; }

        auto setBloodColor(byte bloodColor) -> void { bloodColor_ = bloodColor; }

        auto getRenderMode() const { return renderMode_; }

        auto setRenderMode(Rendering renderMode) -> void { renderMode_ = renderMode; }

        auto getRenderAmount() const { return renderAmount_; }

        auto setRenderAmount(int amount) -> void { renderAmount_ = amount; }

        auto getRenderColor() const -> const std::string& { return renderColor_; }

        auto setRenderColor(const std::string& color) -> void { renderColor_ = color; }

        auto getRenderFx() const { return renderFx_; }

        auto setRenderFx(RenderingFx fx) -> void { renderFx_ = fx; }
    };
}
