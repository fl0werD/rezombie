#include "rezombie/player/players.h"
#include <vhooks/vhooks.h>

namespace rz::player
{
    using namespace cssdk;
    using namespace vhooks;

    class PlayerVirtuals : public PlayerBase {
      private:
        static VirtualHook playerOnCreate;
        static VirtualHook botOnCreate;

      protected:
        auto onCreate() -> void;
    };

    VirtualHook PlayerVirtuals::playerOnCreate(
        "player",
        HookIndex::OnCreate,
        &PlayerVirtuals::onCreate
    );
    VirtualHook PlayerVirtuals::botOnCreate(
        "bot",
        HookIndex::OnCreate,
        &PlayerVirtuals::onCreate
    );

    auto PlayerVirtuals::onCreate() -> void {
        playerOnCreate.Call(this);
        players[this].init(this);
    }
}
