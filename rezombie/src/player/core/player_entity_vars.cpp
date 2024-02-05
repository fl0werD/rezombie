#pragma once

#include "rezombie/player/player.h"
#include "rezombie/colors/colors.h"

namespace rz
{
    using namespace cssdk;

    auto Player::getHealth() const -> int {
        return static_cast<int>(vars_->health);
    }

    auto Player::setHealth(int health) -> void {
        vars_->health = static_cast<float>(health);
    }

    auto Player::getMaxHealth() const -> int {
        return static_cast<int>(vars_->max_health);
    }

    auto Player::setMaxHealth(int maxHealth) -> void {
        vars_->max_health = static_cast<float>(maxHealth);
    }

    auto Player::getArmor() const -> int {
        return static_cast<int>(vars_->armor_value);
    }

    auto Player::setArmor(int armor) -> void {
        vars_->armor_value = static_cast<float>(armor);
    }

    auto Player::getMaxSpeed() const -> int {
        return static_cast<int>(vars_->max_speed);
    }

    auto Player::setMaxSpeed(int maxSpeed) -> void {
        vars_->max_speed = static_cast<float>(maxSpeed);
    }

    auto Player::getGravity() const {
        return vars_->gravity;
    }

    auto Player::setGravity(float gravity) -> void {
        vars_->gravity = gravity;
    }

    auto Player::getGroundEntity() const -> Edict* {
        return vars_->ground_entity;
    }

    auto Player::getFlags() const -> int {
        return vars_->flags;
    }

    auto Player::setFlags(int flags) -> void {
        vars_->flags = flags;
    }

    auto Player::getDeadFlag() const -> DeathState {
        return vars_->dead_flag;
    }

    auto Player::setDeadFlag(DeathState deadFlag) -> void {
        vars_->dead_flag = deadFlag;
    }

    auto Player::setViewModel(unsigned int viewModel) -> void {
        vars_->view_model = static_cast<int>(viewModel);
    }

    auto Player::setWeaponModel(unsigned int weaponModel) -> void {
        vars_->weapon_model = static_cast<int>(weaponModel);
    }

    auto Player::setRenderMode(Rendering mode) -> void {
        vars_->render_mode = mode;
    }

    auto Player::setRenderAmount(int amount) -> void {
        vars_->render_amount = static_cast<float>(amount);
    }

    auto Player::setRenderColor(const std::string& color) -> void {
        const auto valueRef = Colors.parse(color);
        if (!valueRef) {
            return;
        }
        const auto value = *valueRef;
        vars_->render_color = Vector(value.getRed(), value.getGreen(), value.getBlue());
    }

    auto Player::setRenderFx(RenderingFx fx) -> void {
        vars_->render_fx = fx;
    }

    auto Player::getNextThink() const -> float {
        return vars_->next_think;
    }

    auto Player::setNextThink(float time) -> void {
        vars_->next_think = time;
    }

    auto Player::getMoveType() const -> MoveTypeEntity {
        return vars_->move_type;
    }

    auto Player::setMoveType(MoveTypeEntity moveType) -> void {
        vars_->move_type = moveType;
    }

    auto Player::getSkin() const -> int {
        return vars_->skin;
    }

    auto Player::setSkin(int skin) -> void {
        vars_->skin = skin;
    }

    auto Player::getBody() const -> int {
        return vars_->body;
    }

    auto Player::setBody(int body) -> void {
        vars_->body = body;
    }

    auto Player::getEffects() const -> int {
        return vars_->effects;
    }

    auto Player::setEffects(int effects) -> void {
        vars_->effects = effects;
    }

    auto Player::getFrags() const -> int {
        return static_cast<int>(vars_->frags);
    }

    auto Player::setFrags(int frags) -> void {
        vars_->frags = static_cast<float>(frags);
    }

    auto Player::getOrigin() const -> Vector {
        return vars_->origin;
    }

    auto Player::setOrigin(Vector origin) -> void {
        vars_->origin = origin;
    }

    auto Player::getAngles() const -> Vector {
        return vars_->angles;
    }

    auto Player::setAngles(Vector angles) -> void {
        vars_->angles = angles;
    }

    auto Player::getVelocity() const -> Vector& {
        return vars_->velocity;
    }

    auto Player::setVelocity(Vector velocity) -> void {
        vars_->velocity = velocity;
    }

    auto Player::getViewAngle() const -> Vector& {
        return vars_->viewing_angle;
    }

    auto Player::setViewAngle(Vector viewAngle) -> void {
        vars_->viewing_angle = viewAngle;
    }

    auto Player::setFixAngleMode(FixAngleMode fixAngleMod) -> void {
        vars_->fix_angle = fixAngleMod;
    }

    auto Player::getViewOfs() const -> Vector {
        return vars_->view_ofs;
    }

    auto Player::setViewOfs(Vector viewOfs) -> void {
        vars_->view_ofs = viewOfs;
    }

    auto Player::setModelIndex(int modelIndex) -> void {
        vars_->model_index = modelIndex;
    }

    auto Player::getPunchAngle() const -> Vector& {
        return vars_->punch_angle;
    }

    auto Player::setPunchAngle(Vector punchAngle) -> void {
        vars_->punch_angle = punchAngle;
    }

    auto Player::getNetName() const {
        return vars_->net_name;
    }

    auto Player::getWeapons() const -> int {
        return vars_->weapons;
    }

    auto Player::setWeapons(int weapons) -> void {
        vars_->weapons = weapons;
    }

    auto Player::getImpulse() const -> int {
        return vars_->impulse;
    }

    auto Player::setImpulse(int impulse) -> void {
        vars_->impulse = impulse;
    }

    auto Player::getWeaponAnim() const -> int {
        return vars_->weapon_anim;
    }

    auto Player::setWeaponAnim(int weaponAnim) -> void {
        vars_->weapon_anim = weaponAnim;
    }

    auto Player::isInDuck() const -> qboolean {
        return vars_->in_duck;
    }

    auto Player::getDuckTime() const -> float {
        return vars_->duck_time;
    }

    auto Player::getButton() const -> int {
        return vars_->button;
    }

    auto Player::setButton(int button) -> void {
        vars_->button = button;
    }

    auto Player::getOldButtons() const -> int {
        return vars_->old_buttons;
    }

    auto Player::setOldButtons(int oldButtons) -> void {
        vars_->old_buttons = oldButtons;
    }

    auto Player::getWaterLevel() const -> int {
        return vars_->water_level;
    }

    auto Player::setWaterLevel(int waterLevel) -> void {
        vars_->water_level = waterLevel;
    }

    auto Player::getObserverMode() const -> ObserverMode {
        return static_cast<ObserverMode>(vars_->i_user1);
    }

    auto Player::setIUser1(ObserverMode observerMode) -> void {
        vars_->i_user1 = toInt(observerMode);
    }

    auto Player::getIUser3() const -> int {
        return vars_->i_user3;
    }

    auto Player::setIUser3(int iUser3) -> void {
        vars_->i_user3 = iUser3;
    }
}
