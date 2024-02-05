#pragma once

#include <core/regamedll_api.h>
#include <vhooks/vhooks.h>

namespace rz
{
    using namespace cssdk;
    using namespace vhooks;

    void RegisterWeaponHooks();

    enum class GiveType : int {
        Append,
        Replace,
        DropAndReplace
    };

    class WeaponVirtuals : public PlayerWeaponBase {
        static VirtualHook spawn;
        static VirtualHook addToPlayer;
        static VirtualHook getItemInfo;
        static VirtualHook deploy;
        static VirtualHook holster;
        static VirtualHook attachToPlayer;
        static VirtualHook updateClientData;
        static VirtualHook maxSpeed;
        static VirtualHook itemSlot;
        static VirtualHook primaryAttack;
        static VirtualHook secondaryAttack;
        static VirtualHook reload;
        static VirtualHook idle;
        static VirtualHook postFrame;
        static VirtualHook create;
        static VirtualHook destroy;

      protected:
        auto HolderSpawn() -> void;
        auto HolderAddToPlayer(PlayerBase* basePlayer) -> qboolean;
        auto HolderGetItemInfo(ItemInfo* info) -> qboolean;
        auto HolderDeploy() -> qboolean;
        auto HolderHolster(int skipLocal) -> void;
        auto HolderAttachToPlayer(PlayerBase* basePlayer) -> void;
        auto HolderUpdateClientData(PlayerBase* basePlayer) -> qboolean;
        auto HolderMaxSpeed() -> float;
        auto HolderItemSlot() -> InventorySlot;
        auto HolderPrimaryAttack() -> void;
        auto HolderSecondaryAttack() -> void;
        auto HolderReload() -> void;
        auto HolderIdle() -> void;
        auto HolderPostFrame() -> void;
        auto HolderCreate() -> void;
        auto HolderDestroy() -> void;
    };
}
