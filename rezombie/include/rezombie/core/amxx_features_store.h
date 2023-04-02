#pragma once

#include "rezombie/amxmodx/common.h"
#include "rezombie/amxmodx/game_rules.h"
#include "rezombie/amxmodx/player.h"
#include "rezombie/amxmodx/player_class.h"
#include "rezombie/amxmodx/player_model.h"
#include "rezombie/amxmodx/player_props.h"
#include "rezombie/amxmodx/player_subclass.h"
#include "rezombie/core/amxx_base_feature.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace rz
{
    class AmxxFeaturesStore
    {
      private:
        std::vector<AmxxBaseFeature*> features_;

        AmxxFeaturesStore() = default;

      public:
        AmxxFeaturesStore(const AmxxFeaturesStore&) = delete;
        AmxxFeaturesStore(AmxxFeaturesStore&&) = delete;
        auto operator=(const AmxxFeaturesStore&) -> void = delete;
        auto operator=(AmxxFeaturesStore&&) -> void = delete;

        static auto& instance()
        {
            static AmxxFeaturesStore instance;
            return instance;
        }

        static auto add(AmxxBaseFeature* module) -> void
        {
            instance().features_.emplace_back(module);
        }

        static auto restore() -> void
        {
            instance().features_.clear();
            add(&amxxCommon);
            add(&amxxGameRules);
            add(&amxxPlayer);
            add(&amxxPlayerClass);
            add(&amxxPlayerSubclass);
            add(&amxxPlayerProps);
            add(&amxxPlayerModel);
        }

        template <typename F>
        static auto forEach(F&& action) -> void
        {
            std::for_each(instance().features_.begin(), instance().features_.end(), std::forward<F>(action));
        }

        static auto RegisterForwards() -> void
        {
            forEach(
              [](const auto& feature)
              {
                  feature->RegisterForwards();
              }
            );
        }

        static auto RegisterNatives() -> void
        {
            forEach(
              [](const auto& feature)
              {
                  feature->RegisterNatives();
              }
            );
        }
    };
}
