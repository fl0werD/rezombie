#pragma once

#include "rezombie/player/player.h"

namespace rz::player
{
    using namespace cssdk;

    auto Player::GiveDefaultItems() -> void
    {
        cstrike_->GiveDefaultItems();
    }

    auto Player::RemoveAllItems(bool removeSuit) -> void
    {
        cstrike_->RemoveAllItems(removeSuit);
    }

    auto Player::GiveItem(const char* itemName) -> EntityBase*
    {
        return cstrike_->GiveNamedItemEx(itemName);
    }

    auto Player::DropPlayerItem(const char* itemName) -> void
    {
        cstrike_->DropPlayerItem(itemName);
    }

    auto Player::SwitchWeapon(PlayerItemBase* weapon) -> bool
    {
        return cstrike_->SwitchWeapon(weapon);
    }

    auto Player::AddAccount(int amount, RewardType type, bool trackChange) -> void
    {
        cstrike_->AddAccount(amount, type, trackChange);
    }

    auto Player::SetAnimation(PlayerAnim playerAnim) -> void
    {
        cstrike_->SetAnimation(playerAnim);
    }

    auto Player::TeamChangeUpdate() -> void
    {
        cstrike_->TeamChangeUpdate();
    }

    auto Player::Reset() -> void
    {
        cstrike_->Reset();
    }

    auto Player::SpawnEquip() -> void
    {
        cstrike_->OnSpawnEquip(true, false);
    }
}
