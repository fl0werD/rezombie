#pragma once

#include "rezombie/core/base_object.h"
#include "metamod/engine.h"
#include <algorithm>
#include <utility>

namespace rz::player
{
    class PlayerModelHeader {
      private:
        std::string path_;
        bool isDefaultHitboxes_ = false;
        int body_ = 0;
        int modelIndex_ = -1;

      public:
        explicit PlayerModelHeader(std::string path, bool isDefaultHitboxes = false, int body = 0) :
            path_(std::move(path)),
            isDefaultHitboxes_(isDefaultHitboxes),
            body_(body) {}

        auto getPath() const -> const std::string& { return path_; }

        auto setPath(std::string path) { path_ = std::move(path); }

        auto isDefaultHitboxes() const -> bool { return isDefaultHitboxes_; }

        auto getBody() const -> int { return body_; }

        auto setBody(int body) { body_ = body; }

        auto getModelIndex() const -> int { return modelIndex_; }

        auto setModelIndex(int modelIndex) { modelIndex_ = modelIndex; }
    };

    class PlayerModel : public BaseObject {
      private:
        std::vector<PlayerModelHeader> models_;
        bool isDefault_;

        auto addDefault(PlayerModelHeader&& playerModelHeader) -> void {
            isDefault_ = true;
            models_.emplace_back(std::forward<PlayerModelHeader>(playerModelHeader));
        }

      public:
        PlayerModel(std::string handle) : BaseObject(std::move(handle)) {
            addDefault(PlayerModelHeader("models/player/gign/gign.mdl", true));
        }

        auto add(std::string path, bool isDefaultHitboxes = false, int body = 0) -> void {
            // check exists
            if (isDefault_) {
                models_.clear();
                isDefault_ = false;
            }
            models_.emplace_back(std::move(path), isDefaultHitboxes, body);
        }

        auto getRandom(int modelHeaderIndex = -1) const -> const PlayerModelHeader& {
            if (modelHeaderIndex == -1) {
                modelHeaderIndex = metamod::engine::RandomLong(0, models_.size() - 1);
            }
            return models_[modelHeaderIndex];
        }

        template<typename F>
        auto forEachModel(F&& block) -> void {
            std::for_each(models_.begin(), models_.end(), std::forward<F>(block));
        }
    };
}
