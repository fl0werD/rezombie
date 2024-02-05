#include "rezombie/player/players.h"
#include <vhooks/vhooks.h>

namespace rz
{
    using namespace cssdk;
    using namespace vhooks;

    class PlayerVirtuals : public PlayerBase {
        static VirtualHook playerCreate;
        static VirtualHook playerDestroy;
        static VirtualHook botCreate;
        static VirtualHook botDestroy;

      protected:
        auto create() -> void;
        auto destroy() -> void;
    };

    VirtualHook PlayerVirtuals::playerCreate("player", HookIndex::OnCreate, &PlayerVirtuals::create);
    VirtualHook PlayerVirtuals::botCreate("bot", HookIndex::OnCreate, &PlayerVirtuals::create);

    auto PlayerVirtuals::create() -> void {
        playerCreate.Call(this);
        link = new Extras();
        Players[this].init(this);
    }

    VirtualHook PlayerVirtuals::playerDestroy("player", HookIndex::OnDestroy, &PlayerVirtuals::destroy);
    VirtualHook PlayerVirtuals::botDestroy("bot", HookIndex::OnDestroy, &PlayerVirtuals::destroy);

    auto PlayerVirtuals::destroy() -> void {
        delete link;
        playerCreate.Call(this);
    }
}
