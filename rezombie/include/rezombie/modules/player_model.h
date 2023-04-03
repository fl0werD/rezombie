#pragma once

#include "rezombie/core/module.h"
#include "rezombie/player/player_model.h"
#include "rezombie/util.h"
#include <utility>

namespace rz
{
    using namespace player;
    using namespace core;
    using namespace metamod::engine;

    class PlayerModelModule : public Module<PlayerModel> {
      public:
        PlayerModelModule() : Module<PlayerModel>("player_model") {}

        auto add(std::string handle) -> int {
            return Module::add(new PlayerModel(std::move(handle)));
        }

        auto precache() -> void override {
            forEach([](auto& item) {
                item->forEachModel([](auto& model) {
                    int modelIndex = precachePlayerModelIfNotEmpty(model.getPath());
                    if (modelIndex != -1) {
                        model.setModelIndex(modelIndex);
                    }
                });
            });
        }

        static auto precachePlayerModelIfNotEmpty(const std::string& modelPath) -> int {
            if (modelPath.empty()) {
                return -1;
            }
            std::string modelTextures = modelPath;
            str::ReplaceAll(modelTextures, std::string_view{".mdl"}, std::string_view{"T.mdl"});
            const auto fullPathModelTextures = str::BuildPathName("%s", modelTextures);
            if (FileExists(fullPathModelTextures.c_str())) {
                // PrecacheModel(modelTextures.c_str());
            }
            return PrecacheModel(modelPath.c_str());
        }
    };

    inline PlayerModelModule playerModelModule;
}
