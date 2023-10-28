#pragma once

#include "rezombie/colors/api/colors.h"
#include "rezombie/gamerules/api/game_rules.h"
#include "rezombie/gamerules/api/modes.h"
#include "rezombie/player/api/player.h"
#include "rezombie/player/api/player_class.h"
#include "rezombie/player/api/player_subclass.h"
#include "rezombie/player/api/player_props.h"
#include "rezombie/player/api/flashlight.h"
#include "rezombie/player/api/nightvision.h"
#include "rezombie/currency/api/currency.h"
#include "rezombie/currency/api/price.h"
#include "rezombie/models/api/model.h"
#include "rezombie/models/api/models_pack.h"
#include "rezombie/weapons/api/weapon.h"
#include "rezombie/weapons/api/melee.h"
#include "rezombie/items/api/items.h"
#include "rezombie/map/api/map_cameras.h"
#include "rezombie/map/api/environment.h"
#include "rezombie/map/api/fog.h"
#include "rezombie/modelpreview/api/model_preview.h"
#include "rezombie/thirdcamera/api/third_camera.h"
#include "rezombie/messages/api/user_message.h"
#include "rezombie/core/api/amxx_base_feature.h"
#include <algorithm>
#include <vector>

namespace rz
{
    class AmxxFeaturesStore {
        std::vector<AmxxBaseFeature*> features_ = {
            &ColorsApi,
            &CurrencyApi,
            &PriceApi,
            &GameRulesApi,
            &ModesApi,
            &PlayerApi,
            &PlayerClassApi,
            &PlayerSubclassApi,
            &PlayerPropsApi,
            &ModelsApi,
            &ModelsPackApi,
            &WeaponApi,
            &amxxMelee,
            &FlashlightApi,
            &NightVisionApi,
            &ItemApi,
            &EnvironmentApi,
            &FogApi,
            &ModelPreviewApi,
            &ThirdCameraApi,
            &MapCamerasApi,
            &UserMessageApi
        };

      public:
        AmxxFeaturesStore() = default;

        template<typename F>
        auto forEach(F&& action) -> void {
            std::for_each(features_.cbegin(), features_.cend(), std::forward<F>(action));
        }

        auto registerForwards() -> void {
            forEach([](const auto& feature) {
                feature->registerForwards();
            });
        }

        auto registerNatives() -> void {
            forEach([](const auto& feature) {
                feature->registerNatives();
            });
        }
    };

    inline AmxxFeaturesStore ApiStore;
}
