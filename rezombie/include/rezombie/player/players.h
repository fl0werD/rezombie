#pragma once

#include "rezombie/player/player.h"
#include <core/type_conversion.h>
#include <cssdk/common/const.h>
#include <cssdk/engine/global_vars.h>
#include <cssdk/public/rehlds/rehlds_interfaces.h>

namespace rz
{
    using namespace cssdk;
    using namespace core::type_conversion;

    class Players {
        Player players_[MAX_CLIENTS];

      public:
        template<typename F>
        auto forEachConnected(const F& block) -> void {
            for (auto& player: players_) {
                if (!player.isValid() || player.isDormant()) {
                    continue;
                }
                block(player);
            }
        }

        auto begin() -> Player* { return players_; }

        auto end() -> Player* { return players_ + g_global_vars->max_clients; }

        auto cbegin() const -> const Player* { return players_; }

        auto cend() const -> const Player* { return players_ + g_global_vars->max_clients; }

        auto& operator[](int index) { return players_[index - 1]; }

        auto& operator[](Edict* edict) { return players_[IndexOfEntity(edict) - 1]; }

        auto& operator[](const Edict* edict) { return players_[IndexOfEntity(edict) - 1]; }

        auto& operator[](EntityBase* player) { return players_[player->EdictIndex() - 1]; }

        auto& operator[](PlayerBase* player) { return players_[player->EdictIndex() - 1]; }

        auto& operator[](IGameClient* client) { return players_[client->GetId()]; }
    };

    inline Players Players;
}
