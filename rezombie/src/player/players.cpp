#include "rezombie/player/players.h"
#include <vhooks/vhooks.h>

namespace rz
{
    using namespace cssdk;
    using namespace vhooks;

    class PlayerVirtuals : public PlayerBase {
        static VirtualHook playerOnCreate;
        static VirtualHook botOnCreate;

      protected:
        auto create() -> void;
    };

    VirtualHook PlayerVirtuals::playerOnCreate("player", HookIndex::OnCreate, &PlayerVirtuals::create);
    VirtualHook PlayerVirtuals::botOnCreate("bot", HookIndex::OnCreate, &PlayerVirtuals::create);

    auto PlayerVirtuals::create() -> void {
        playerOnCreate.Call(this);
        Players[this].init(this);
    }
}
