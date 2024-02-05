#pragma once

#include "rezombie/core/module.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/entity/player/player_class.h"

namespace rz
{
    class PlayerClassModule : public Module<PlayerClass> {
      public:
        PlayerClassModule() : Module("player_class") {}

        auto add(std::string handle, Team team) -> int {
            return Module::add(new PlayerClass(std::move(handle), team));
        }

        auto precache() -> void override {
            constexpr auto byTeam = +[](Team team) {
                return [team](const PlayerClass* item) {
                    return item->getTeam() == team;
                };
            };
            GameRules.setDefaultPlayerClass(Team::Human, find(byTeam(Team::Human)));
            GameRules.setDefaultPlayerClass(Team::Zombie, find(byTeam(Team::Zombie)));
        }
    };

    inline PlayerClassModule Classes;
}
