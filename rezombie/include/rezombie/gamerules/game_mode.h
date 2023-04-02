#pragma once

#include "rezombie/core/base_object.h"
#include <string>

namespace rz
{
    class GameMode : public BaseObject
    {
      private:
        std::string name_ = "";
        int dropChance_ = 20;
        int minimumPlayers_ = 0;
        int roundTime_ = 0;

      public:
        GameMode(std::string handle) : BaseObject(std::move(handle))
        {
        }

        auto getDropChance() const
        {
            return dropChance_;
        }

        auto setDropChance(int dropChance)
        {
            dropChance_ = dropChance;
        }

        auto getMinPlayers() const
        {
            return minimumPlayers_;
        }

        auto setMinPlayers(int minimumPlayers)
        {
            minimumPlayers_ = minimumPlayers;
        }

        auto getRoundTime() const
        {
            return roundTime_;
        }

        auto setRoundTime(int roundTime)
        {
            roundTime_ = roundTime;
        }
    };
}
