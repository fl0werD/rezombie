#pragma once

#include "rezombie/core/module.h"
#include "rezombie/entity/models/model.h"

namespace rz
{
    using namespace core;
    using namespace metamod::engine;

    class ModelModule : public Module<Model> {

      public:
        int defaultPlayerModelId_ = 0;

        ModelModule() : Module("models") {}

        auto add(std::string handle, std::string path, int body = 0, int skin = 0) -> int {
            return Module::add(
                new Model(
                    std::move(handle),
                    std::move(path),
                    body,
                    skin
                )
            );
        }

        auto precache() -> void override {
            defaultPlayerModelId_ = add("default_player_model", "models/player.mdl");
            forEach([this](auto& item) {
                int index = precacheModel(item->getPath());
                if (index != -1) {
                    item->setPrecacheId(index);
                }
                precacheModelTextures(item->getTexturesPath());
            });
        }

        auto getDefaultPlayerModel() const -> int { return defaultPlayerModelId_; }

        static auto precacheModelTextures(const std::string& texturesPath) -> void {
            if (texturesPath.empty()) {
                return;
            }
            PrecacheModel(texturesPath.c_str());
        }
    };

    inline ModelModule Models;

    auto setModel(EntityVars* vars, int modelId) -> bool;
    auto setModel(EntityState* state, int modelId) -> bool;
}
