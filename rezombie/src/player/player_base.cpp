#pragma once

#include "rezombie/player/player.h"

namespace rz::player
{
    using namespace cssdk;

    auto Player::isAlive() const -> bool
    {
        return base_->IsAlive();
    }

    auto Player::isBot() const -> bool
    {
        return base_->IsBot();
    }

    auto Player::isDormant() const -> bool
    {
        return base_->IsDormant();
    }

    auto Player::getGunPosition() const -> Vector
    {
        return base_->GetGunPosition();
    }

    auto Player::getBloodColor() const -> int
    {
        return base_->blood_color_value;
    }

    auto Player::setBloodColor(int bloodColor) -> void
    {
        base_->blood_color_value = bloodColor;
    }

    auto Player::getTeam() const -> Team
    {
        return static_cast<Team>(base_->team);
    }

    auto Player::getTeamId() const -> int
    {
        return toInt(base_->team);
    }

    auto Player::setTeam(Team team) -> void
    {
        base_->team = static_cast<TeamName>(team);
    }

    auto Player::getAccount() const -> int
    {
        return base_->account;
    }

    auto Player::setAccount(int account) -> void
    {
        base_->account = account;
    }

    auto Player::getMenu() const -> MenuName
    {
        return base_->menu;
    }

    auto Player::setMenu(MenuName menu) -> void
    {
        base_->menu = menu;
    }

    auto Player::getJoiningState() const -> JoinState
    {
        return base_->joining_state;
    }

    auto Player::setJoiningState(JoinState joinState) -> void
    {
        base_->joining_state = joinState;
    }

    auto Player::getNumSpawns() const -> int
    {
        return base_->num_spawns;
    }

    auto Player::setNumSpawns(int numSpawns) -> void
    {
        base_->num_spawns = numSpawns;
    }

    auto Player::setNextSprayTime(float nextTime) -> void
    {
        base_->next_decal_time = nextTime;
    }

    auto Player::getDeaths() const -> int
    {
        return base_->deaths;
    }

    auto Player::setDeaths(int deaths) -> void
    {
        base_->deaths = deaths;
    }

    auto Player::setKevlar(ArmorType armorType) -> void
    {
        base_->kevlar = armorType;
    }

    auto Player::getHelmet() const -> bool
    {
        return base_->kevlar == ArmorType::VestHelm;
    }

    auto Player::isKilled() const -> bool
    {
        return !base_->not_killed;
    }

    auto Player::setKilled(bool isKilled) -> void
    {
        base_->not_killed = !isKilled;
    }

    auto Player::isHeadshotKilled() const -> bool
    {
        return base_->headshot_killed;
    }

    auto Player::isJustConnected() const -> bool
    {
        return base_->just_connected;
    }

    auto Player::isDisconnected() const -> bool
    {
        return base_->has_disconnected;
    }

    auto Player::setDisconnected(bool isDisconnected) -> void
    {
        base_->has_disconnected = isDisconnected;
    }

    auto Player::getCanShoot() const -> bool
    {
        return base_->can_shoot;
    }

    auto Player::setCanShoot(int canShoot) -> void
    {
        base_->can_shoot = canShoot;
    }

    auto Player::getHideHud() const -> int
    {
        return base_->hide_hud;
    }

    auto Player::setHideHud(int hideFlags) -> void
    {
        base_->hide_hud = hideFlags;
    }

    auto Player::setNextAttack(float nextAttack) -> void
    {
        base_->next_attack = nextAttack;
    }

    auto Player::getPlayerItems(int slot) const -> PlayerItemBase*
    {
        return base_->player_items[slot];
    }

    auto Player::setPlayerItems(int slot, PlayerItemBase* item) -> void
    {
        base_->player_items[slot] = item;
    }

    auto Player::getActiveItem() const -> PlayerItemBase*
    {
        return base_->active_item;
    }

    auto Player::setActiveItem(PlayerItemBase* item) -> void
    {
        base_->active_item = item;
    }

    auto Player::getLastItem() const -> PlayerItemBase*
    {
        return base_->last_item;
    }

    auto Player::setLastItem(PlayerItemBase* item) -> void
    {
        base_->last_item = item;
    }

    auto Player::getAmmo(int ammoIndex) const -> int
    {
        return base_->ammo[ammoIndex];
    }

    auto Player::setAmmo(int ammoIndex, int amount) -> void
    {
        base_->ammo[ammoIndex] = amount;
    }

    auto Player::getObserverTarget() const -> Edict*
    {
        return base_->observer_target.Get();
    }

    auto Player::getButtonLast() const -> int
    {
        return base_->button_last;
    }

    auto Player::setButtonLast(int button) -> void
    {
        base_->button_last = button;
    }

    auto Player::getButtonPressed() const -> int
    {
        return base_->button_pressed;
    }

    auto Player::setButtonPressed(int button) -> void
    {
        base_->button_pressed = button;
    }

    auto Player::getButtonReleased() const -> int
    {
        return base_->button_released;
    }

    auto Player::setButtonReleased(int button) -> void
    {
        base_->button_released = button;
    }

    auto Player::setAnimExtension(const char* animExtension) -> void
    {
        strcpy_s(base_->anim_extension, animExtension);
    }

    auto Player::ResetMaxSpeed() -> void
    {
        base_->ResetMaxSpeed();
    }

    auto Player::RemovePlayerItem(PlayerItemBase* item) -> qboolean
    {
        return base_->RemovePlayerItem(item);
    }

    auto Player::RoundRespawn() -> void
    {
        base_->RoundRespawn();
    }
}
